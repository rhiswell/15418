#include <stdio.h>
#include "immintrin.h"

#define VECTOR_WIDTH 8

int htab[VECTOR_WIDTH][VECTOR_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},};

void sqrt_avx(int N,
        float initialGuess,
        float values[],
        float output[])
{
    int last_part_len = N % VECTOR_WIDTH;
    int i;
    // Hope the GCC will unroll this loop for SIMD instructions
    for (i = 0; i < N-last_part_len; i += VECTOR_WIDTH) {
        // Load 8 floats into a vector register
        __m256 x = _mm256_load_ps(values+i);
        // There is a builtin sqrt instruction
        __m256 result = _mm256_sqrt_ps(x);
        // Store result into output buffer
        _mm256_store_ps(output+i, result);
    }

    if (last_part_len) {
        // Gen mask
        __m256i mask = _mm256_set_epi32(
                htab[last_part_len][0],
                htab[last_part_len][1],
                htab[last_part_len][2],
                htab[last_part_len][3],
                htab[last_part_len][4],
                htab[last_part_len][5],
                htab[last_part_len][6],
                htab[last_part_len][7]);
        // All 1.0f
        __m256 x = _mm256_set1_ps(1.0f);
        x = _mm256_maskload_ps(values+i, mask);
        __m256 result = _mm256_sqrt_ps(x);
        _mm256_maskstore_ps(output+i, mask, result);
    }
}

