#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <pthread.h>
#include <math.h>
#include <assert.h>

#include "CycleTimer.h"
#include "sqrt_ispc.h"

using namespace ispc;

extern void sqrt_avx(int N, float startGuess, float *values, float *output);
extern void sqrtSerial(int N, float startGuess, float* values, float* output);

static void verifyResult(int N, float* result, float* gold) {
    for (int i=0; i<N; i++) {
        if (fabs(result[i] - gold[i]) > 1e-4) {
            printf("Error: [%d] Got %f expected %f\n", i, result[i], gold[i]);
        }
    }
}

inline void good_input(int N, float *values)
{
    for (int i = 0; i < N; i++)
        values[i] = 3.0f-.001f;
}
inline void bad_input(int N, float *values)
{
    int vector_len = 16;

    for (int i = 0; i < N; i++)
        values[i] = 1.0f;
    for (int i = 0; i < N; i += vector_len)
        values[i] = 3.0f-.001f;
}

int main() {

    const unsigned int N = 20 * 1000 * 1000;
    const float initialGuess = 1.0f;

    //float* values = new float[N];
    //float* output = new float[N];
    float *values = (float *) aligned_alloc(32, N*sizeof(float));
    assert(values != NULL);
    float *output = (float *) aligned_alloc(32, N*sizeof(float));
    assert(output != NULL);
    float* gold = new float[N];

    //bad_input(N, values);
    for (unsigned int i=0; i<N; i++)
    {
        // random input values
        values[i] = .001f + 2.998f * static_cast<float>(rand()) / RAND_MAX;
        // DONE: Try different input values here.
        //values[i] = 1.0f;
        //values[i] = 3.0f - .001f;
        output[i] = 0.f;
    }
    //std::sort(values, values+N);

    // generate a gold version to check results
    for (unsigned int i=0; i<N; i++)
        gold[i] = sqrt(values[i]);

    //
    // And run the serial implementation 3 times, again reporting the
    // minimum time.
    //
    double minSerial = 1e30;
    for (int i = 0; i < 5; ++i) {
        double startTime = CycleTimer::currentSeconds();
        sqrtSerial(N, initialGuess, values, output);
        double endTime = CycleTimer::currentSeconds();
        minSerial = std::min(minSerial, endTime - startTime);
    }

    printf("[sqrt serial]:\t\t[%.3f] ms\n", minSerial * 1000);

    verifyResult(N, output, gold);

    //
    // Compute the image using the ispc implementation; report the minimum
    // time of three runs.
    //
    double minISPC = 1e30;
    for (int i = 0; i < 5; ++i) {
        double startTime = CycleTimer::currentSeconds();
        sqrt_ispc(N, initialGuess, values, output);
        double endTime = CycleTimer::currentSeconds();
        minISPC = std::min(minISPC, endTime - startTime);
    }

    printf("[sqrt ispc]:\t\t[%.3f] ms\n", minISPC * 1000);

    verifyResult(N, output, gold);

    // Clear out the buffer
    for (unsigned int i = 0; i < N; ++i)
        output[i] = 0;

    //
    // Tasking version of the ISPC code
    //
    double minTaskISPC = 1e30;
    for (int i = 0; i < 3; ++i) {
        double startTime = CycleTimer::currentSeconds();
        sqrt_ispc_withtasks(N, initialGuess, values, output);
        double endTime = CycleTimer::currentSeconds();
        minTaskISPC = std::min(minTaskISPC, endTime - startTime);
    }

    printf("[sqrt task ispc]:\t[%.3f] ms\n", minTaskISPC * 1000);

    verifyResult(N, output, gold);

    // sqrt() with raw AVX
    double minAVXSqrt = 1e30;
    for (int i = 0; i < 3; ++i) {
        double startTime = CycleTimer::currentSeconds();
        sqrt_avx(N, initialGuess, values, output);
        double endTime = CycleTimer::currentSeconds();
        minAVXSqrt = std::min(minAVXSqrt, endTime - startTime);
    }

    printf("[sqrt with raw AVX]\t[%.3f] ms\n", minAVXSqrt * 1000);

    verifyResult(N, output, gold);

    printf("\t\t\t\t(%.2fx speedup from ISPC)\n", minSerial/minISPC);
    printf("\t\t\t\t(%.2fx speedup from task ISPC)\n", minSerial/minTaskISPC);
    printf("\t\t\t\t(%.2fx speedup from raw AVX)\n", minSerial/minAVXSqrt);

    //delete[] values;
    //delete[] output;
    free(values);
    free(output);
    delete[] gold;

    return 0;
}
