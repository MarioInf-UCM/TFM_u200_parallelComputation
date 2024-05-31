#include <stdio.h>
#include <cmath>
#include <stdlib.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
#if defined(TYPEDATA_INT)
    typedef int typeData;
#elif defined(TYPEDATA_FP)
    typedef float typeData;
#else
    typedef float typeData;
#endif



// DATASIZE COMPILATOR VARIABLE
//**********************************
#ifdef MINI_DATASET
    #define SIZE_N 40
#elif defined(SMALL_DATASET)
    #define SIZE_N 120
#elif defined(MEDIUM_DATASET)
    #define SIZE_N 400
#elif defined(LARGE_DATASET)
    #define SIZE_N 2000
#elif defined(EXTRALARGE_DATASET)
    #define SIZE_N 4000
#else
    #define SIZE_N 0
#endif


extern "C"{

    #ifdef MINI_DATASET
        void ker_cholesky_noOpt_mini(typeData *inD_A, typeData *outD_A)
    #elif defined(SMALL_DATASET)
        void ker_cholesky_noOpt_small(typeData *inD_A, typeData *outD_A)
    #elif defined(MEDIUM_DATASET)
        void ker_cholesky_noOpt_medium(typeData *inD_A, typeData *outD_A)
    #elif defined(LARGE_DATASET)
        void ker_cholesky_noOpt_large(typeData *inD_A, typeData *outD_A)
    #elif defined(EXTRALARGE_DATASET)
        void ker_cholesky_noOpt_extralarge(typeData *inD_A, typeData *outD_A)
    #else
        void ker_cholesky_noOpt(typeData *inD_A, typeData *outD_A)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A offset = slave bundle = gmem 

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        for (int i = 0; i < SIZE_N; i++) {
            #pragma HLS pipeline
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
            for (int j = 0; j < i; j++) {
                for (int k = 0; k < j; k++) {
                    outD_A[(i*SIZE_N) + j] -= inD_A[(i*SIZE_N) + k] * inD_A[(j*SIZE_N) + k];
                }
                outD_A[(i*SIZE_N) + j] /= inD_A[(j*SIZE_N) + j];
            }
            for (int k = 0; k < i; k++) {
                outD_A[(i*SIZE_N) + i] -= inD_A[(i*SIZE_N) + k] * inD_A[(i*SIZE_N) + k];
            }
            outD_A[(i*SIZE_N) + i] = sqrt(inD_A[(i*SIZE_N) + i]);
        }
        
    }
    
}