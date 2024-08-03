#include <stdio.h>
#include <cmath>
#include <stdlib.h>

// TYPEDATA COMPILER VARIABLE
//**********************************
typedef float typeData;


// DATASIZE COMPILER VARIABLE
//**********************************
#ifdef MINI_DATASET
    #define SIZE (1024 * 1024 * 2)
#elif defined(SMALL_DATASET)
    #define SIZE (1024 * 1024 * 4)
#elif defined(MEDIUM_DATASET)
    #define SIZE (1024 * 1024 * 8)
#elif defined(LARGE_DATASET)
    #define SIZE (1024 * 1024 * 16)
#elif defined(EXTRALARGE_DATASET)
    #define SIZE (1024 * 1024 * 32)
#else
    #define SIZE 0
#endif
        

// READING BY CHUNKS
//**********************************
#define WIDTH_OF_ACCESS 512
#define SIZE_OF_TYPEDATA sizeof(typeData)
#define VECTOR_SIZE (WIDTH_OF_ACCESS/SIZE_OF_TYPEDATA)
#define STREAM_SIZE 1024


extern "C"{

    void read_data(typeData *inD, typeData *local, int offset, int chunk_size) {
        for (int j = 0; j < chunk_size; j++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS UNROLL factor=8
            local[j] = inD[offset + j];
        }
    }

    void write_data(typeData *outD, typeData *local, int offset, int chunk_size) {
        for (int j = 0; j < chunk_size; j++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS UNROLL factor=8
            outD[offset + j] = local[j];
        }
    }

    void process_data(typeData *localA, typeData *localB, typeData *result, int chunk_size) {
        for (int j = 0; j < chunk_size; j++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS UNROLL factor=8
            result[j] = localA[j] + localB[j];
        }
    }

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    #ifdef MINI_DATASET
        void ker_vectorAdd_Opt3_mini(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_vectorAdd_Opt3_small(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_vectorAdd_Opt3_medium(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_vectorAdd_Opt3_large(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_vectorAdd_Opt3_extralarge(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #else
        void ker_vectorAdd_Opt3(typeData *inD_vA, typeData *inD_vB, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_vA offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_vB offset = slave bundle = gmem1
        #pragma HLS INTERFACE m_axi port = outD_result offset = slave bundle = gmem2

        #pragma HLS INTERFACE s_axilite port = inD_vA bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_vB bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData inD_vA_local[STREAM_SIZE];
        typeData inD_vB_local[STREAM_SIZE];
        typeData result_local[STREAM_SIZE];
        int actualChunkSize = 0;

        for (int i = 0; i < SIZE; i += STREAM_SIZE) {
            actualChunkSize = STREAM_SIZE;
            if ((i + STREAM_SIZE) > SIZE){
                actualChunkSize = SIZE - i;
            }

            #pragma HLS DATAFLOW
            #pragma HLS stream variable=inD_vA_local depth=STREAM_SIZE
            #pragma HLS stream variable=inD_vB_local depth=STREAM_SIZE
            #pragma HLS stream variable=result_local depth=STREAM_SIZE

            read_data(inD_vA, inD_vA_local, i, actualChunkSize);
            read_data(inD_vB, inD_vB_local, i, actualChunkSize);
            process_data(inD_vA_local, inD_vB_local, result_local, actualChunkSize);
            write_data(outD_result, result_local, i, actualChunkSize);
        }

        return;
    }
}