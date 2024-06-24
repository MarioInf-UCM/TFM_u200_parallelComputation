#include <stdio.h>
#include <cmath>
#include <stdlib.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
typedef float typeData;


// DATASIZE COMPILATOR VARIABLE
//**********************************
#ifdef MINI_DATASET
    #define STEPS 20
    #define SIZE_N 30
#elif defined(SMALL_DATASET)
    #define STEPS 40
    #define SIZE_N 90
#elif defined(MEDIUM_DATASET)
    #define STEPS 100
    #define SIZE_N 250
#elif defined(LARGE_DATASET)
    #define STEPS 500
    #define SIZE_N 1300
#elif defined(EXTRALARGE_DATASET)
    #define STEPS 1000
    #define SIZE_N 2800
#else
    #define STEPS 0
    #define SIZE_N 0
#endif
        


extern "C"{

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    #ifdef MINI_DATASET
        void ker_jacobi2d_Opt1_mini(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_jacobi2d_Opt1_small(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_jacobi2d_Opt1_medium(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_jacobi2d_Opt1_large(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_jacobi2d_Opt1_extralarge(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #else
        void ker_jacobi2d_Opt1(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A max_read_burst_length = 64 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_B max_read_burst_length = 64 offset = slave bundle = gmem1
        #pragma HLS INTERFACE m_axi port = outD_result max_write_burst_length = 64 offset = slave bundle = gmem2 

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_B bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData inD_A_local[SIZE_N*SIZE_N];
        typeData inD_B_local[SIZE_N*SIZE_N];
        typeData outD_result_local[SIZE_N*SIZE_N];

        #pragma HLS DATAFLOW
        //#pragma HLS stream variable = inD_A_local depth = 64
        //#pragma HLS stream variable = inD_B_local depth = 64
        //#pragma HLS stream variable = outD_result_local depth = 64



        readingInitData:
        for (int i = 0 ; i < SIZE_N*SIZE_N ; i++) {
        #pragma HLS pipeline
        #pragma HLS LOOP_TRIPCOUNT min = SIZE_N*SIZE_N max = SIZE_N*SIZE_N
            inD_A_local[i] = inD_A[i];
            inD_B_local[i] = inD_B[i];
        }

        for (int t=0; t<STEPS ; t++){
            #pragma HLS LOOP_TRIPCOUNT min=STEPS max=STEPS
            
            for (int i = 1; i < SIZE_N - 1 ; i++){
                #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N

                InnerLoopA:
                for (int j = 1; j < SIZE_N - 1 ; j++){
                    #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
                    #pragma HLS UNROLL factor=8
                    #pragma HLS PIPELINE II=2
                    inD_B_local[(i*SIZE_N)+j] = 0.2 * (inD_A_local[(i*SIZE_N)+j] + inD_A_local[(i*SIZE_N)+(j-1)] + inD_A_local[(i*SIZE_N)+(j+1)] + inD_A_local[((i+1)*SIZE_N)+j] + inD_A_local[((i-1)*SIZE_N)+j]);
                    outD_result_local[(i*SIZE_N)+j] = inD_B_local[(i*SIZE_N)+j];
                }

            }
            for (int i = 1; i < SIZE_N - 1 ; i++){
                #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N

                InnerLoopB:
                for (int j = 1; j < SIZE_N - 1 ; j++){
                    #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
                    #pragma HLS UNROLL factor=8
                    #pragma HLS PIPELINE II=2
                    inD_A_local[(i*SIZE_N)+j] = 0.2 * (inD_B_local[(i*SIZE_N)+j] + inD_B_local[(i*SIZE_N)+(j-1)] + inD_B_local[(i*SIZE_N)+(j+1)] + inD_B_local[((i+1)*SIZE_N)+j] + inD_B_local[((i-1)*SIZE_N)+j]);
                    outD_result_local[(SIZE_N*SIZE_N)+(i*SIZE_N)+j] = inD_A_local[(i*SIZE_N)+j];
                }
            }
        }

        writingResultData:
        for (int i = 1; i < (SIZE_N * SIZE_N*2) - 1 ; i++){
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
            #pragma HLS UNROLL factor=8
            #pragma HLS PIPELINE II=2
            outD_result[i] = outD_result_local[i];
        }

        
    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************
    
}