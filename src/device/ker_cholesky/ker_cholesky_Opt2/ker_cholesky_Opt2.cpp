#include <ap_fixed.h>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
#define TYPEDATA_BITS_SIZE 16
#define TYPEDATA_BITS_INT 7
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
        #pragma HLS INTERFACE m_axi port = inD_A max_read_burst_length = 32 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A max_write_burst_length = 32 offset = slave bundle = gmem1

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control


        typeData inD_A_local[SIZE_N*SIZE_N];
        typeData outD_A_local[SIZE_N*SIZE_N];

        #pragma HLS DATAFLOW
        //#pragma HLS stream variable = inD_A_local depth = 64
        //#pragma HLS stream variable = outD_A_local depth = 64

        readingInitDataA:
        for (int i = 0; i < SIZE_N*SIZE_N ; i++) {
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
            #pragma HLS UNROLL factor=8
            #pragma HLS PIPELINE II=1
            inD_A_local[i] = inD_A[i];
            outD_A_local[i] = 0.0;
        }

        mainLoop:
        for (int i = 0; i < SIZE_N; i++) {
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
            #pragma HLS UNROLL factor=8

            for (int j = 0; j < i; j++) {
                #pragma HLS LOOP_TRIPCOUNT min=0 max=SIZE_N
                #pragma HLS UNROLL factor=8
            
                for (int k = 0; k < j; k++) {
                    #pragma HLS PIPELINE II=1
                    outD_A_local[(i*SIZE_N) + j] -= inD_A_local[(i*SIZE_N) + k] * inD_A_local[(j*SIZE_N) + k];
                }

                outD_A_local[(i*SIZE_N) + j] /= inD_A_local[(j*SIZE_N) + j];
            }

            for (int k = 0; k < i; k++) {
                #pragma HLS LOOP_TRIPCOUNT min=0 max=SIZE_N
                #pragma HLS UNROLL factor=8
                #pragma HLS PIPELINE II=1
                outD_A_local[(i*SIZE_N) + i] -= inD_A_local[(i*SIZE_N) + k] * inD_A_local[(i*SIZE_N) + k];
            }

            outD_A_local[(i*SIZE_N) + i] = sqrt_fixed(inD_A_local[(i*SIZE_N) + i]);
        }


        writingResultData:
        for (int i = 0; i < SIZE_N*SIZE_N ; i++) {
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
            #pragma HLS UNROLL factor=8
            #pragma HLS PIPELINE II=1
            outD_A[i] = outD_A_local[i];
        }


    }
    
}