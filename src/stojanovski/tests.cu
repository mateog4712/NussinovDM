#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "stojanovski.cuh"


template < typename T >
__global__ void call_reduce( T * data, uint32_t const n )
{
    stoja::reduce( data, n );
}

struct wrap_reduce
{
    template < typename T >
    void operator () ( T * data, uint32_t const n ) const
    {
        auto const size = n * sizeof( T );

        T * dev_data;
        cudaMalloc( (void **)&dev_data, size );
        REQUIRE( cudaGetLastError() == cudaSuccess );
        cudaMemcpy( dev_data, data, size, cudaMemcpyHostToDevice );
        REQUIRE( cudaGetLastError() == cudaSuccess );

        call_reduce<<< 1, n >>>( dev_data, n );
        REQUIRE( cudaGetLastError() == cudaSuccess );

        cudaMemcpy( data, dev_data, size, cudaMemcpyDeviceToHost );
        REQUIRE( cudaGetLastError() == cudaSuccess );
        cudaFree( dev_data );
        REQUIRE( cudaGetLastError() == cudaSuccess );
    }
};



SCENARIO( "The min and max of several values is computed", "[max][util]")
{
  using namespace stoja;

  GIVEN( "Two uint32_t vals" )
  {
  	uint32_t x, y;

    WHEN( "The points are identical" )
    {
      x = 7u;
      y = 7u;

      THEN( "The max function returns their shared value" )
      {
        REQUIRE( max( x, y ) == 7u );
      }

      THEN( "The min function returns their shared value" )
      {
        REQUIRE( min( x, y ) == 7u );
      }
    }

    WHEN( "The first value is larger" )
    {
      x = 8u;
      y = 7u;

      THEN( "The max function returns the first value" )
      {
        REQUIRE( max( x, y ) == x );
      }

      THEN( "The min function returns the second value" )
      {
        REQUIRE( min( x, y ) == y );
      }
    }

    WHEN( "The first value is smaller" )
    {
      x = 6u;
      y = 7u;

      THEN( "The max function returns the second value" )
      {
        REQUIRE( max( x, y ) == y );
      }

      THEN( "The min function returns the first value" )
      {
        REQUIRE( min( x, y ) == x );
      }
    }
  }

    GIVEN( "Four uint32_t vals" )
    {
        uint32_t w, x, y, z;

        WHEN( "The points are identical" )
        {
            w = 7u;
            x = 7u;
            y = 7u;
            z = 7u;

            THEN( "The max function returns their shared value" )
            {
                REQUIRE( max( w, x, y, z ) == 7u );
            }
        }

        WHEN( "The points are all distinct" )
        {
            w = 7u;
            x = 8u;
            y = 9u;
            z = 6u;

            THEN( "The max function returns the largest" )
            {
                REQUIRE( max( w, x, y, z ) == y );
            }
        }

        WHEN( "The values are grouped in pairs" )
        {
            w = 7u;
            x = 7u;
            y = 9u;
            z = 9u;

            THEN( "The max function returns the largest" )
            {
                REQUIRE( max( w, x, y, z ) == z );
            }
        }
    }
}

SCENARIO( "The reduce function obtains max of a list", "[reduce][util]")
{
    GIVEN( "An array of uint32_t values" )
    {
        WHEN( "The array contains 8 unique values" )
        {
            uint32_t input[] = { 1u, 2u, 3u, 4u, 5u, 8u, 7u, 6u };
            auto const n = sizeof( input ) / sizeof( input[ 0 ] );

            THEN( "The reduce function moves the largest to the first index" )
            {
                wrap_reduce{}( input, n );
                REQUIRE( input[ 0 ] == 8u );
            }
        }

        WHEN( "The array is not a power of 2" )
        {
            auto const max = 9u;
            uint32_t input[] = { 1u, 2u, 3u, 4u, 5u, 6u, max };
            auto const n = sizeof( input ) / sizeof( input[ 0 ] );

            THEN( "The reduce function moves the largest to the first index" )
            {
                wrap_reduce{}( input, n );
                REQUIRE( input[ 0 ] == max );
            }
        }

        WHEN( "The array requires 2 warps to cooperate" )
        {
            auto const max = 10u;
            uint32_t input[] = { 1u, 2u, 3u, 4u, 5u, 8u, 7u, 6u,
                                 1u, 2u, 3u, 4u, 5u, 8u, 7u, 6u, 
                                 1u, 2u, 3u, 4u, 5u, 8u, 7u, 6u,
                                 1u, 2u, 3u, 4u, 5u, 8u, 7u, 6u,
                                 1u, 2u, max,4u, 5u, 2u, 7u, 6u };

            auto const n = sizeof( input ) / sizeof( input[ 0 ] );

            THEN( "The reduce function moves the largest to the first index" )
            {
                wrap_reduce{}( input, n );
                REQUIRE( input[ 0 ] == max );
            }
        }
    }
}
