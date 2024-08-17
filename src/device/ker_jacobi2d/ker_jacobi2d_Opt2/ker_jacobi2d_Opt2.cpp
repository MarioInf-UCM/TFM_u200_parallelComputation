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


#define WIDTH_OF_ACCESS 512
#define SIZE_OF_TYPEDATA sizeof(typeData)
#define VECTOR_SIZE (WIDTH_OF_ACCESS/SIZE_OF_TYPEDATA)

extern "C"{


    void readData(typeData *inD_A, typeData *inD_B, typeData *inD_A_local, typeData *inD_B_local){
        for (int i = 0 ; i < SIZE_N*SIZE_N ; i++) {
            #pragma HLS STREAM variable=inD_A_local depth=SIZE_N*SIZE_N 
            #pragma HLS STREAM variable=inD_B_local depth=SIZE_N*SIZE_N 
            #pragma HLS PIPELINE off
            inD_A_local[i] = inD_A[i];
            inD_B_local[i] = inD_B[i];
        }
        return;
    }

    void innerLoop1(typeData *inD_A_local, typeData *inD_B_local, typeData *outD_result_local, int i){
        for (int j = 1; j < SIZE_N - 1 ; j++){
            #pragma HLS STREAM variable=inD_B_local depth=SIZE_N-1
            #pragma HLS STREAM variable=outD_result_local depth=SIZE_N-1
            #pragma HLS PIPELINE off            
            inD_B_local[(i*SIZE_N)+j] = 0.2 * (inD_A_local[(i*SIZE_N)+j] + inD_A_local[(i*SIZE_N)+(j-1)] + inD_A_local[(i*SIZE_N)+(j+1)] + inD_A_local[((i+1)*SIZE_N)+j] + inD_A_local[((i-1)*SIZE_N)+j]);
            outD_result_local[(i*SIZE_N)+j] = inD_B_local[(i*SIZE_N)+j];
        }
        return;
    }

    void innerLoop2(typeData *inD_A_local, typeData *inD_B_local, typeData *outD_result_local, int i){
        for (int j = 1; j < SIZE_N - 1 ; j++){
            #pragma HLS STREAM variable=inD_A_local depth=SIZE_N-1
            #pragma HLS STREAM variable=outD_result_local depth=SIZE_N-1
            #pragma HLS PIPELINE off
            inD_A_local[(i*SIZE_N)+j] = 0.2 * (inD_B_local[(i*SIZE_N)+j] + inD_B_local[(i*SIZE_N)+(j-1)] + inD_B_local[(i*SIZE_N)+(j+1)] + inD_B_local[((i+1)*SIZE_N)+j] + inD_B_local[((i-1)*SIZE_N)+j]);
            outD_result_local[(SIZE_N*SIZE_N)+(i*SIZE_N)+j] = inD_A_local[(i*SIZE_N)+j];
        }
        return;  
    }

    void writeData(typeData *outD_result, typeData *outD_result_local) {
        for (int i = 0 ; i < SIZE_N*SIZE_N*2 ; i++) {
            #pragma HLS STREAM variable=outD_result_local depth=SIZE_N*SIZE_N*2
            #pragma HLS PIPELINE off
            outD_result[i] = outD_result_local[i];
        }
        return;
    }



    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    #ifdef MINI_DATASET
        void ker_jacobi2d_Opt2_mini(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(SMALL_DATASET)
        void ker_jacobi2d_Opt2_small(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(MEDIUM_DATASET)
        void ker_jacobi2d_Opt2_medium(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(LARGE_DATASET)
        void ker_jacobi2d_Opt2_large(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #elif defined(EXTRALARGE_DATASET)
        void ker_jacobi2d_Opt2_extralarge(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #else
        void ker_jacobi2d_Opt2(typeData *inD_A, typeData *inD_B, typeData *outD_result)
    #endif
    {
        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_B offset = slave bundle = gmem1
        #pragma HLS INTERFACE m_axi port = outD_result offset = slave bundle = gmem2 

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_B bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_result bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData inD_A_local[SIZE_N*SIZE_N];
        typeData inD_B_local[SIZE_N*SIZE_N];
        typeData outD_result_local[SIZE_N*SIZE_N*2];


        readData(inD_A, inD_B, inD_A_local, inD_B_local);

        for (int t=0; t<STEPS ; t++){
            #pragma HLS PIPELINE off
            
            for (int i = 1; i < SIZE_N - 1 ; i++){
                #pragma HLS PIPELINE off
                innerLoop1(inD_A_local, inD_B_local, outD_result_local, i);
            }
            for (int i = 1; i < SIZE_N - 1 ; i++){
                #pragma HLS PIPELINE off
                innerLoop2(inD_A_local, inD_B_local, outD_result_local, i);
            }
        }

        writeData(outD_result, outD_result_local);


        return;   
    }
}