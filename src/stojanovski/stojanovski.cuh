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

void init_streams( cudaStream_t *streams, uint32_t const num_streams )
{
    for( auto i = 0u; i < num_streams; ++i )
    {
        cudaStreamCreate( &streams[ i ] );
    }
}

} // namespace util
namespace stoja {

template < typename T >
__device__ __host__ T max( T a, T b )
{
    return a > b ? a : b;
}

template < typename T >
__device__ __host__ T max( T a, T b, T c, T d )
{
    return max( max( a, b )
              , max( c, d ) );
}

template < typename T >
__device__ __host__ T min( T a, T b )
{
    return a < b ? a : b;
}

__device__ bool match( char const * sequence, int i, int j )
{
    return ( ((sequence[i] == 'A' and sequence[j] == 'U' ) or (sequence[i] == 'G' and sequence[j] == 'C' )) and j-i>3);
}

template < typename T >
__device__ void reduce( T * data, int n )
{
    auto const tid = threadIdx.x;
    if( tid >= n ) return;

    for( auto step = 2u; step < 2 * n; step *= 2 )
    {
        if( tid % step != 0 ) return;

        data[ tid ] = max( data[ tid ], data[ tid + step / 2 ] );
        __syncthreads();
    }
}

template < typename T >
__device__ T calc_max( T const * table, uint32_t i, uint32_t j, char const * sequence, int n )
{
    extern __shared__ int workspace[];
    auto const th_id = threadIdx.x;
    auto const width = j - i - 1;

    // Init shared memory to zeroes
    workspace[ th_id ] = 0;

    // Populated shared memory with largest found by each thread
    for( auto tile = 0u; tile < width; tile += blockDim.x )
    {
        if( tile + th_id < width )
        {
            auto const k = i + 1 + tile + th_id;
            workspace[ th_id ] = max( workspace[ th_id ]
                                    , table[ n * i + k ] + table[ n * j + k + 1 ] );
        }
    }
    __syncthreads();

    reduce( workspace, min( blockDim.x, width ) );
    return max( workspace[ 0 ]
              , table[ ( n * i ) + ( j - 1 ) ]
              , table[ ( n * ( i + 1 ) ) + j ]
              , match( sequence, i, j ) ? table[ n * ( i + 1 ) + ( j - 1 ) ] + 1 : 0 );
}

template < typename T >
__global__ void kernel2( T * table, char const * sequence, int n, uint32_t i, uint32_t j )
{
	extern __shared__ int sh_array [];

    table[ i * n + j ] = table[ j * n + i ] = calc_max( table, i, j, sequence, n );
}


template < typename T >
void nussinovCuda( T * table, char const * sequence, uint32_t const n )
{
    auto const num_streams = 128u; // limit for Compute Capability 7.0
    auto const block_size = 128u;
    auto const shared_mem_size = block_size * sizeof( table[ 0 ] );

    std::vector< cudaStream_t > streams( num_streams );
    util::init_streams( streams.data(), num_streams );


    for( auto i = 1u; i < n; ++i )
    {
        for( auto j = 0; j < n - i; ++j )
        {
            stoja::kernel2<<< 1u, block_size, shared_mem_size, streams[ j % num_streams ] >>>( table, sequence, n, j, i + j );
        }
        cudaDeviceSynchronize();
    }
}

} // namespace stoja
