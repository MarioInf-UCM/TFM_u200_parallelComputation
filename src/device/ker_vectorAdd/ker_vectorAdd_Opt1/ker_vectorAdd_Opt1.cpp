#include <stdio.h>
#include <cmath>
#include <stdlib.h>

// TYPEDATA COMPILATOR VARIABLE
//**********************************
typedef float typeData;


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
        void ker_vectorAdd_Opt1_mini(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_vectorAdd_Opt1_small(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_vectorAdd_Opt1_medium(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_vectorAdd_Opt1_large(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_vectorAdd_Opt1_extralarge(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #else
        void ker_vectorAdd_Opt1(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_vA offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_vB offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_result offset = slave bundle = gmem 

        #pragma HLS INTERFACE s_axilite port = inD_vA bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_vB bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData inD_vA_local[VECTOR_SIZE];
        typeData inD_vB_local[VECTOR_SIZE];
        int actualChunkSize=VECTOR_SIZE;


        for(int pos=0 ; pos<SIZE ; pos+=VECTOR_SIZE){

            for (int i = 0; ( i<VECTOR_SIZE && (pos+i)<SIZE) ; i++) {
                inD_vA_local[i] = inD_vA[pos+i];
                inD_vB_local[i] = inD_vB[pos+i]; 
            }

            for (int i = 0; ( i<VECTOR_SIZE && (pos+i)<SIZE) ; i++) {
                outD_result[pos+i] = inD_vA_local[i] + inD_vB_local[i];
            }    
        }

    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************
    
}