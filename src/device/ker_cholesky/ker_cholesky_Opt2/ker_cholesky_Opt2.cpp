#include <stdio.h>
#include <hls_math.h>
#include <stdlib.h>
#include <ap_fixed.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
#define TYPEDATA_BITS_SIZE 32
#define TYPEDATA_BITS_INT 17
typedef ap_fixed<TYPEDATA_BITS_SIZE, TYPEDATA_BITS_INT> typeData;




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
        void ker_cholesky_Opt2_mini(typeData *inD_A, typeData *outD_A)
    #elif defined(SMALL_DATASET)
        void ker_cholesky_Opt2_small(typeData *inD_A, typeData *outD_A)
    #elif defined(MEDIUM_DATASET)
        void ker_cholesky_Opt2_medium(typeData *inD_A, typeData *outD_A)
    #elif defined(LARGE_DATASET)
        void ker_cholesky_Opt2_large(typeData *inD_A, typeData *outD_A)
    #elif defined(EXTRALARGE_DATASET)
        void ker_cholesky_Opt2_extralarge(typeData *inD_A, typeData *outD_A)
    #else
        void ker_cholesky_Opt2(typeData *inD_A, typeData *outD_A)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A offset = slave bundle = gmem1

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control


        #pragma HLS DATAFLOW

        for (int i = 0; i < SIZE_N; i++) {
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
            #pragma HLS UNROLL factor=8

            for (int j = 0; j < i; j++) {
                #pragma HLS LOOP_TRIPCOUNT min=0 max=SIZE_N
                #pragma HLS UNROLL factor=8
            
                for (int k = 0; k < j; k++) {
                    #pragma HLS PIPELINE II=1
                    outD_A[(i*SIZE_N) + j] -= inD_A[(i*SIZE_N) + k] * inD_A[(j*SIZE_N) + k];
                }

                outD_A[(i*SIZE_N) + j] /= inD_A[(j*SIZE_N) + j];
            }

            for (int k = 0; k < i; k++) {
                #pragma HLS LOOP_TRIPCOUNT min=0 max=SIZE_N
                #pragma HLS UNROLL factor=8
                #pragma HLS PIPELINE II=1
                outD_A[(i*SIZE_N) + i] -= inD_A[(i*SIZE_N) + k] * inD_A[(i*SIZE_N) + k];
            }
            
            outD_A[(i*SIZE_N) + i] = sqrt_fixed(inD_A[(i*SIZE_N) + i]);
        }
        
    }
    
}