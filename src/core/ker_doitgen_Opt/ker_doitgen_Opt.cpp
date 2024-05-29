//#include "/opt/xilinx/Vitis_HLS/2023.2/include/ap_int.h" 
#include <ap_int.h>
#include <stdio.h>
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

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    #ifdef MINI_DATASET
        void ker_doitgen_Opt_mini(const ap_uint<DATAWIDTH> *inD_A, const ap_uint<DATAWIDTH> *inD_C4, ap_uint<DATAWIDTH> *outD_A)
    #elif defined(SMALL_DATASET)
        void ker_doitgen_Opt_small(const ap_uint<DATAWIDTH> *inD_A, const ap_uint<DATAWIDTH> *inD_C4, ap_uint<DATAWIDTH> *outD_A)
    #elif defined(MEDIUM_DATASET)
        void ker_doitgen_Opt_medium(const ap_uint<DATAWIDTH> *inD_A, const ap_uint<DATAWIDTH> *inD_C4, ap_uint<DATAWIDTH> *outD_A)
    #elif defined(LARGE_DATASET)
        void ker_doitgen_Opt_large(const ap_uint<DATAWIDTH> *inD_A, const ap_uint<DATAWIDTH> *inD_C4, ap_uint<DATAWIDTH> *outD_A)
    #elif defined(EXTRALARGE_DATASET)
        void ker_doitgen_Opt_extralarge(const ap_uint<DATAWIDTH> *inD_A, const ap_uint<DATAWIDTH> *inD_C4, ap_uint<DATAWIDTH> *outD_A)
    #else
        void ker_doitgen_Opt(ap_uint<DATAWIDTH> *inD_A, ap_uint<DATAWIDTH> *inD_C4, ap_uint<DATAWIDTH> *outD_A)
    #endif
    {
        #pragma HLS INTERFACE m_axi port=inD_A max_read_burst_length=32 offset=slave bundle=gmem
        #pragma HLS INTERFACE m_axi port=inD_C4 max_read_burst_length=32 offset=slave bundle=gmem
        #pragma HLS INTERFACE m_axi port=outD_A max_write_burst_length=32 offset=slave bundle=gmem 

        #pragma HLS INTERFACE s_axilite port=inD_A bundle=control
        #pragma HLS INTERFACE s_axilite port=inD_C4 bundle=control
        #pragma HLS INTERFACE s_axilite port=outD_A bundle=control
        #pragma HLS INTERFACE s_axilite port=return bundle=control

        ap_uint<DATAWIDTH> loc_inD_A[BUFFER_SIZE];
        ap_uint<DATAWIDTH> loc_inD_C4[BUFFER_SIZE];
        ap_uint<DATAWIDTH> sum[SIZE_P];

        for (int i = 0; i < SIZE_P; i++) {
            #pragma HLS pipeline
            #pragma HLS LOOP_TRIPCOUNT min = 1 max = 64
            loc_inD_C4[i] = inD_C4[i];
        }

        int numIterations = (SIZE_R * SIZE_P * SIZE_Q - 1) / VECTOR_SIZE + 1;
        int chunk_size = 0;
        int posTemp1 = 0, posTemp2 = 0;

        for (int iter=0; iter<numIterations ; iter+=BUFFER_SIZE) {
            #pragma HLS DATAFLOW
            #pragma HLS stream variable=loc_inD_A depth=64
            #pragma HLS stream variable=loc_inD_C4 depth=64

            chunk_size = BUFFER_SIZE;
            if ((iter + BUFFER_SIZE) > numIterations){   
                chunk_size = numIterations - iter;
            }

            for (int i = 0; i < chunk_size; i++) {
                #pragma HLS pipeline
                #pragma HLS LOOP_TRIPCOUNT min = 1 max = 64
                loc_inD_A[i] = inD_A[iter + i];
            }

            for(int posTemp1=0 ; posTemp1<chunk_size ; posTemp1++){ //for (int r = 0; r < SIZE_R; r++){
               //#pragma HLS pipeline
               //#pragma HLS LOOP_TRIPCOUNT min = 1 max = 64
                for (int q = 0; q < SIZE_Q; q++){

                    posTemp2=0;
                    for (int p = 0; p < SIZE_P; p++){
                        sum[p] = 0.0;
                        for (int s = 0; s < SIZE_P; s++){
                            sum[p] += loc_inD_A[iter+posTemp1+posTemp2] * loc_inD_C4[posTemp2];
                            posTemp2++;
                        }
                    }

                    posTemp2=0;
                    for (int p = 0; p < SIZE_P; p++){
                        outD_A[iter+posTemp1+posTemp2] = sum[p];
                        posTemp2++;
                    }

                    posTemp1++;
                }
                
            }
        }        
        return;
        
    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************

}
