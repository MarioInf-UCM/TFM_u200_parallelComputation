#include <stdio.h>
#include <cmath>
#include <stdlib.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
typedef float typeData;



// DATASIZE COMPILATOR VARIABLE
//**********************************
#ifdef MINI_DATASET
    #define NI 20
    #define NJ 25
    #define NK 30
#elif defined(SMALL_DATASET)
    #define NI 60
    #define NJ 70
    #define NK 80
#elif defined(MEDIUM_DATASET)
    #define NI 200
    #define NJ 220
    #define NK 240
#elif defined(LARGE_DATASET)
    #define NI 1000
    #define NJ 1100
    #define NK 1200
#elif defined(EXTRALARGE_DATASET)
    #define NI 2000
    #define NJ 2300
    #define NK 2600
#else
    #define NI 0
    #define NJ 0
    #define NK 0
#endif



extern "C"{

    #ifdef MINI_DATASET
        void ker_gemm_Opt0_mini(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_gemm_Opt0_small(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_gemm_Opt0_medium(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_gemm_Opt0_large(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_gemm_Opt0_extralarge(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #else
        void ker_gemm_Opt0(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_B offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_C offset = slave bundle = gmem 
        #pragma HLS INTERFACE m_axi port = outD_result offset = slave bundle = gmem 

        #pragma HLS INTERFACE s_axilite port = inD_alpha bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_beta bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_B bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_C bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        for (int i=0 ; i<NI ; i++){
            for (int j=0; j<NJ; j++){
                outD_result[(i*NJ)+j] = inD_C[(i*NJ)+j] * inD_beta;
                for (int k=0; k<NK; ++k){
                    outD_result[(i*NJ)+j] += inD_alpha * inD_A[(i*NK)+k] * inD_B[(k*NJ)+j];
                }
            }
        }

        return;
    }

}