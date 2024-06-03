#include <ap_fixed.h>
#include <stdio.h>
#include <stdlib.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
#if defined(TYPEDATA_INT)
    typedef int typeData;
#elif defined(TYPEDATA_FP)
    typedef float typeData;
#else
    #define TYPEDATA_BITS_SIZE 32
    #define TYPEDATA_BITS_INT 8
    typedef float typeData;
    //typedef ap_fixed<TYPEDATA_BITS_SIZE, TYPEDATA_BITS_INT> typeData;
#endif



// DATASIZE COMPILATOR VARIABLE
//**********************************
#ifdef MINI_DATASET
    #define SIZE_Q 8
    #define SIZE_R 10
    #define SIZE_P 12
#elif defined(SMALL_DATASET)
    #define SIZE_Q 20
    #define SIZE_R 25
    #define SIZE_P 30
#elif defined(MEDIUM_DATASET)
    #define SIZE_Q 40
    #define SIZE_R 50
    #define SIZE_P 60
#elif defined(LARGE_DATASET)
    #define SIZE_Q 140
    #define SIZE_R 150
    #define SIZE_P 160
#elif defined(EXTRALARGE_DATASET)
    #define SIZE_Q 220
    #define SIZE_R 250
    #define SIZE_P 270
#else
    #define SIZE_Q 0
    #define SIZE_R 0
    #define SIZE_P 0
#endif


#define BUFFER_SIZE 64
#define DATAWIDTH 512
#define VECTOR_SIZE (DATAWIDTH / sizeof(typeData) )



extern "C"{

    #ifdef MINI_DATASET
        void ker_doitgen_Opt_mini(typeData *inD_A, typeData *inD_C4, typeData *outD_A)
    #elif defined(SMALL_DATASET)
        void ker_doitgen_Opt_small(typeData *inD_A, typeData *inD_C4, typeData *outD_A)
    #elif defined(MEDIUM_DATASET)
        void ker_doitgen_Opt_medium(typeData *inD_A, typeData *inD_C4, typeData *outD_A)
    #elif defined(LARGE_DATASET)
        void ker_doitgen_Opt_large(typeData *inD_A, typeData *inD_C4, typeData *outD_A)
    #elif defined(EXTRALARGE_DATASET)
        void ker_doitgen_Opt_extralarge(typeData *inD_A, typeData *inD_C4, typeData *outD_A)
    #else
        void ker_doitgenOpt(typeData *inD_A, typeData *inD_C4, typeData *outD_A)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_C4 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A offset = slave bundle = gmem 

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_C4 bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData sum[SIZE_P];

        for (int r = 0; r < SIZE_R; r++){
            #pragma HLS pipeline
            #pragma HLS LOOP_TRIPCOUNT min=SIZE_R max=SIZE_R
            for (int q = 0; q < SIZE_Q; q++){
                for (int p = 0; p < SIZE_P; p++){
                    sum[p] = 0.0;
                    for (int s = 0; s < SIZE_P; s++){
                        sum[p] += inD_A[(r*SIZE_Q*SIZE_P)+(q*SIZE_P)+s] * inD_C4[(s*SIZE_P)+p];
                    }
                }

                for (int p = 0; p < SIZE_P; p++){
                    outD_A[(r*SIZE_Q*SIZE_P)+(q*SIZE_P)+p] = sum[p];
                }
            }
        }
        
        return;
        
    }

}