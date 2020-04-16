/**
 * Implementation of:
 * Stojanovski et al. (2012). "Parallelization of Dynamic Programming  in Nussinov RNA Folding Algorithm on the CUDA GPU."
 *     Proc. ICT Innovations. https://doi.org/10.1007/978-3-642-28664-3_26
 *
 * Note that we skipped implementing kernel1, as it wasn't shown to have a noticeable effect in the original paper.
 * However, note that Chang et al. (2010) had proposed *three* kernels, not just two.
 *
 * Note also that this version does not shift the transpose by one location and then over-allocate the matrix to ensure
 * 16B alignment. The efficacy of the shifting depends on the alignment and the alignment is a global consideration that
 * should be done for all algorithms.
 */

#pragma once

#include <vector>

namespace util {

/**
 * Initialises a block of num_streams already-allocated cudaStream_t objects.
 * Should only be called once, prior to using any cudaStream_t in streams.
 */
void init_streams( cudaStream_t *streams, uint32_t const num_streams )
{
    for( auto i = 0u; i < num_streams; ++i )
    {
        cudaStreamCreate( &streams[ i ] );
    }
}

} // namespace util
namespace stoja {

/**
 * Returns the larger value of `a` and `b`
 */
template < typename T >
__device__ __host__ T max( T a, T b )
{
    return a > b ? a : b;
}

/**
 * Returns the largest value among `a`, `b`, `c`, and `d`
 */
template < typename T >
__device__ __host__ T max( T a, T b, T c, T d )
{
    return max( max( a, b )
              , max( c, d ) );
}

/**
 * Returns the smaller value of `a` and `b`
 */
template < typename T >
__device__ __host__ T min( T a, T b )
{
    return a < b ? a : b;
}

/**
 * Determines whether the i'th and j'th base pairs in sequence are sufficiently
 * far apart and match (A-U or G-C)
 */
__device__ bool match( char const * sequence, uint32_t const i, uint32_t const j )
{
    return (   j - i > 3
           and ( ( sequence[ i ] == 'A' and sequence[ j ] == 'U' )
              or ( sequence[ i ] == 'G' and sequence[ j ] == 'C' ) ) );
}

/**
 * Uses a multi-warp parallel reduction to move into position data[ 0 ] the
 * largest value found between data[ 0 ] and data[ n - 1 ].
 *
 * @note All even indexes in data up to n-1 are potentially overwritten
 */
template < typename T >
__device__ void reduce( T * data, uint32_t const n )
{
    auto const tid = threadIdx.x; // only ever one block per grid, as per paper
    if( tid >= n ) return;

    // Iterate until thread 0 accesses index n / 2.
    for( auto step = 2u; step < 2 * n; step *= 2 )
    {
        if( tid % step != 0 ) return; // kill off half the threads at every step

        data[ tid ] = max( data[ tid ], data[ tid + step / 2 ] );
        __syncthreads(); // keep warps in steplock
    }
}

/**
 * Calculate cell (i,j) of table, given an input RNA sequence of length n
 *
 * @pre table must be already populated for all values in row i up to column j and
 * in column j for all rows down to i. Moreover, the column must have been transposed
 * and copied into row j for better access patterns 
 */
template < typename T >
__device__ T calc_max( T const * table, uint32_t i, uint32_t j, char const * sequence, int n )
{
    extern __shared__ int workspace[];

    auto const th_id = threadIdx.x; // only one block per grid
    auto const width = j - i - 1; // the width of row i up to, but not including column j

    // Init shared memory to zeroes
    workspace[ th_id ] = 0;

    // Calculate the O(n) component of the Nussinov recurrence.
    // Populates shared memory with largest found by each thread.
    // Step tiles by blockDim.x as each thread in block will handle one value of each tile
    for( auto tile = 0u; tile < width; tile += blockDim.x )
    {
        if( tile + th_id < width ) // avoid accessing beyond end of row/col
        {
            auto const k = i + 1 + tile + th_id; // give each thread one value from tile
            workspace[ th_id ] = max( workspace[ th_id ]
                                    , table[ n * i + k ] + table[ n * j + k + 1 ] );
        }
    }
    __syncthreads(); // ensure all warps have finished before reducing result

    reduce( workspace, min( blockDim.x, width ) ); // move max to workspace[ 0 ]

    // Return the largest of the four Nussinov recurrence values
    return max( workspace[ 0 ]
              , table[ ( n * i ) + ( j - 1 ) ]
              , table[ ( n * ( i + 1 ) ) + j ]
              , match( sequence, i, j ) ? table[ n * ( i + 1 ) + ( j - 1 ) ] + 1 : 0 );
}

/**
 * The kernel called "kernel 2" in Stojanovski (2012). Computes and stores in table 
 * the dynamic programming result for a given cell (i,j). The result is duplicated in
 * cell (j,i) for better access patterns in subsequent calls to `kernel2()`.
 *
 * The main idea in the paper is that "kernel 2" should use one GPU block/grid to compute one
 * dynamic programming cell. "kernel 1" uses one block to compute multiple grid cells, which
 * exposes even less parallelism and is not implemented here. Fundamentally, they have not
 * considered enough options in their design:
 * > The first question is: “How many threads would be responsible for computing the value of
 * > one element?”. The proposed solutions are: 1) a thread; 2) a block of threads; 3) a grid.
 * But, naturally, a thread, block, or grid could cooperate to calculate several cells; the cell
 * does not have to be the atomic unit of computation. 
 */
template < typename T >
__global__ void kernel2( T * table, char const * sequence, int n, uint32_t i, uint32_t j )
{
    table[ i * n + j ] = table[ j * n + i ] = calc_max( table, i, j, sequence, n );
}


/**
 * Calculates the dynamic programming matrix as per the Nussinov algorithm on the given RNA sequence of length n
 * and stores the result in table.
 *
 * @see Nussinov et al. (1978) "Algorithm for Loop Matching." _SIAM Journal on Applied Mathematics_ 35(1), 68–82.
 */
template < typename T >
void nussinovCuda( T * table, char const * sequence, uint32_t const n )
{
    // Paper explicitly states launching a kernel with only 1 block that contains 128 threads. This
    // is stated in the methodology section, not the experiments. Probably these numbers were tuned for
    // their particular GPU, which is now ten years out of date. They don't make a lot of sense today,
    // but kernel2() which devotes an entire grid to computing one dynamic programming cell will also
    // struggle to saturate a GPU today. These numbers could be tuned, but won't likely address the underlying
    // limitation that this algorithm doesn't not expose enough parallelism for a next-gen GPU.
    auto const blocks_per_grid = 1u;
    auto const block_size = 128u;

    auto const shared_mem_size = block_size * sizeof( T ); // need one T for each active thread
    auto const num_streams = 128u; // limit for Compute Capability 7.0

    // Use CUDA streams to launch concurrent kernels
    std::vector< cudaStream_t > streams( num_streams );
    util::init_streams( streams.data(), num_streams );

    // Calculate each dynamic programming cell (j, i+j).
    // As inner loop iterates a diagonal, there are no dependencies therein and each inner loop iteration can be executed
    // concurrently using the asynchronous CUDA streams defined above.
    // Starts at i=1 because the base case/main diagonal, i=0, is all zeroes.
    for( auto i = 1u; i < n; ++i )
    {
        for( auto j = 0; j < n - i; ++j )
        {
            stoja::kernel2<<< blocks_per_grid
                            , block_size
                            , shared_mem_size
                            , streams[ j % num_streams ] >>>( table, sequence, n, j, i + j );
        }
        cudaDeviceSynchronize();
    }
}

} // namespace stoja
