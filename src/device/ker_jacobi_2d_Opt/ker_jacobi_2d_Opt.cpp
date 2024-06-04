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
        void ker_jacobi_2d_Opt_mini(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_jacobi_2d_Opt_small(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_jacobi_2d_Opt_medium(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_jacobi_2d_Opt_large(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_jacobi_2d_Opt_extralarge(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #else
        void ker_jacobi_2d_Opt(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_B offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_result offset = slave bundle = gmem 

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_B bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control
    

        #pragma HLS DATAFLOW

        for (int t=0; t<STEPS ; t++){
            #pragma HLS LOOP_TRIPCOUNT min=STEPS max=STEPS
            
            for (int i = 1; i < SIZE_N - 1 ; i++){
                #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N

                for (int j = 1; j < SIZE_N - 1 ; j++){
                    #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
                    #pragma HLS UNROLL factor=8
                    #pragma HLS PIPELINE II=2
                    inD_B[(i*SIZE_N)+j] = 0.2 * (inD_A[(i*SIZE_N)+j] + inD_A[(i*SIZE_N)+(j-1)] + inD_A[(i*SIZE_N)+(j+1)] + inD_A[((i+1)*SIZE_N)+j] + inD_A[((i-1)*SIZE_N)+j]);
                    outD_result[(i*SIZE_N)+j] = inD_B[(i*SIZE_N)+j];
                }
            }
            for (int i = 1; i < SIZE_N - 1 ; i++){
                #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N

                for (int j = 1; j < SIZE_N - 1 ; j++){
                    #pragma HLS LOOP_TRIPCOUNT min=SIZE_N max=SIZE_N
                    #pragma HLS UNROLL factor=8
                    #pragma HLS PIPELINE II=2
                    inD_A[(i*SIZE_N)+j] = 0.2 * (inD_B[(i*SIZE_N)+j] + inD_B[(i*SIZE_N)+(j-1)] + inD_B[(i*SIZE_N)+(j+1)] + inD_B[((i+1)*SIZE_N)+j] + inD_B[((i-1)*SIZE_N)+j]);
                    outD_result[(SIZE_N*SIZE_N)+(i*SIZE_N)+j] = inD_A[(i*SIZE_N)+j];
                }
            }
        }
        
    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************
    
}