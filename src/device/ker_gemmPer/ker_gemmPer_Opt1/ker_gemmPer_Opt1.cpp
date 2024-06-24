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
        void ker_gemmPer_Opt1_mini(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_gemmPer_Opt1_small(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_gemmPer_Opt1_medium(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_gemmPer_Opt1_large(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_gemmPer_Opt1_extralarge(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #else
        void ker_gemmPer_Opt1(typeData inD_alpha, typeData inD_beta, typeData *inD_A, typeData *inD_B, typeData *inD_C, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A max_read_burst_length = 64 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_B max_read_burst_length = 64 offset = slave bundle = gmem1
        #pragma HLS INTERFACE m_axi port = inD_C max_read_burst_length = 64 offset = slave bundle = gmem2 
        #pragma HLS INTERFACE m_axi port = outD_result max_write_burst_length = 64 offset = slave bundle = gmem3 

        #pragma HLS INTERFACE s_axilite port = inD_alpha bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_beta bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_B bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_C bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control



        typeData inD_alpha_local = inD_alpha;
        typeData inD_beta_local = inD_beta;
        typeData inD_A_local[NI*NK];
        typeData inD_B_local[NK*NJ];
        typeData inD_C_local[NI*NJ];
        typeData outD_result_local[NI*NJ];

        #pragma HLS DATAFLOW
        //#pragma HLS stream variable = inD_A_local depth = 64
        //#pragma HLS stream variable = inD_B_local depth = 64
        //#pragma HLS stream variable = inD_C_local depth = 64
        //#pragma HLS stream variable = outD_result_local depth = 64


        readingInitDataA:
        for (int i = 0 ; i < NI*NK ; i++) {
        #pragma HLS pipeline
        #pragma HLS LOOP_TRIPCOUNT min = NI*NK max = NI*NK
            inD_A_local[i] = inD_A[i];
        }

        readingInitDataB:
        for (int i = 0 ; i < NK*NJ ; i++) {
        #pragma HLS pipeline
        #pragma HLS LOOP_TRIPCOUNT min = NK*NJ max = NK*NJ
            inD_B_local[i] = inD_B[i];
        }

        readingInitDataC:
        for (int i = 0 ; i < NI*NJ ; i++) {
        #pragma HLS pipeline
        #pragma HLS LOOP_TRIPCOUNT min = NI*NJ max = NI*NJ
            inD_C_local[i] = inD_C[i];
            outD_result_local[i] = 0.0;
        }



        mainLoop:
        for (int i = 0; i < NI; i++) {
            #pragma HLS LOOP_TRIPCOUNT min=NI max=NI

            for (int j = 0; j < NJ; j++){
                #pragma HLS LOOP_TRIPCOUNT min=NJ max=NJ
                #pragma HLS UNROLL factor=8
                #pragma HLS PIPELINE II=1
                outD_result[(i*NJ)+j] *= inD_beta;
            }

            for (int k = 0; k < NK; k++){
                #pragma HLS LOOP_TRIPCOUNT min=NK max=NK

                for (int j = 0; j < NJ; j++){
                    #pragma HLS LOOP_TRIPCOUNT min=NJ max=NJ
                    #pragma HLS UNROLL factor=8
                    #pragma HLS PIPELINE II=1
                    outD_result[(i*NJ)+j] += inD_alpha * inD_A[(i*NK)+k] * inD_B[(k*NJ)+j];
                }
            }
        }
    


        writingResultData:
        for (int i=0; i<NI; i++){
            #pragma HLS LOOP_TRIPCOUNT min=NI max=NI

            for (int j=0; j<NJ; ++j){
                #pragma HLS LOOP_TRIPCOUNT min=NJ max=NJ
                #pragma HLS UNROLL factor=8
                #pragma HLS PIPELINE II=1
                outD_result[(i*NJ)+j] = outD_result_local[(i*NJ)+j];
            }
        }


        return;
    }
    
}