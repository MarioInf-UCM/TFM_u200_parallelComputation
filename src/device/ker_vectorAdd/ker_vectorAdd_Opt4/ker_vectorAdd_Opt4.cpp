#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <ap_fixed.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
#define TYPEDATA_BITS_SIZE 16
#define TYPEDATA_BITS_INT 7
typedef ap_fixed<TYPEDATA_BITS_SIZE, TYPEDATA_BITS_INT> typeData;


// DATASIZE COMPILATOR VARIABLE
//**********************************
#ifdef MINI_DATASET
    #define SIZE 10
#elif defined(SMALL_DATASET)
    #define SIZE 100
#elif defined(MEDIUM_DATASET)
    #define SIZE 2000
#elif defined(LARGE_DATASET)
    #define SIZE 50000
#elif defined(EXTRALARGE_DATASET)
    #define SIZE 800000
#else
    #define SIZE 0
#endif
        

// READING BY CHUNKS
//**********************************
#define WIDTH_OF_ACCESS 512
#define SIZE_OF_TYPEDATA sizeof(typeData)
#define VECTOR_SIZE (WIDTH_OF_ACCESS/SIZE_OF_TYPEDATA)



extern "C"{

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    #ifdef MINI_DATASET
        void ker_vectorAdd_Opt4_mini(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_vectorAdd_Opt4_small(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_vectorAdd_Opt4_medium(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_vectorAdd_Opt4_large(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_vectorAdd_Opt4_extralarge(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #else
        void ker_vectorAdd_Opt4(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_vA offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_vB offset = slave bundle = gmem1
        #pragma HLS INTERFACE m_axi port = outD_result offset = slave bundle = gmem2

        #pragma HLS INTERFACE s_axilite port = inD_vA bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_vB bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData inD_vA_local[VECTOR_SIZE];
        typeData inD_vB_local[VECTOR_SIZE];
        int actualChunkSize=VECTOR_SIZE;

        #pragma HLS DATAFLOW
        #pragma HLS stream variable = inD_vA_local depth = 64
        #pragma HLS stream variable = inD_vB_local depth = 64

        for(int pos=0 ; pos<SIZE ; pos+=VECTOR_SIZE){
            
            block_initialRading:
            for (int i = 0; ( i<VECTOR_SIZE && (pos+i)<SIZE) ; i++) {
                #pragma HLS PIPELINE II=2
                #pragma HLS LOOP_TRIPCOUNT min=1  max=VECTOR_SIZE 
                #pragma HLS UNROLL factor=8
                inD_vA_local[i] = inD_vA[pos+i];
                inD_vB_local[i] = inD_vB[pos+i]; 
            }

            block_add:
            for (int i = 0; ( i<VECTOR_SIZE && (pos+i)<SIZE) ; i++) {
                #pragma HLS PIPELINE II=1
                #pragma HLS LOOP_TRIPCOUNT min=1  max=VECTOR_SIZE 
                #pragma HLS UNROLL factor=8
                outD_result[pos+i] = inD_vA_local[i] + inD_vB_local[i];
            }    
        }

    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************
    
}