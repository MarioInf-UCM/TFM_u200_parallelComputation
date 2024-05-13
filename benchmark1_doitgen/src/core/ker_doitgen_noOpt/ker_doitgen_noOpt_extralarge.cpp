#include <stdio.h>
#include <stdlib.h>


#if defined(TYPEDATA_INT)
    typedef int typeData;
#elif defined(TYPEDATA_FP)
    typedef float typeData;
#else
    typedef float typeData;
#endif

#define SIZE_R 220
#define SIZE_Q 250
#define SIZE_P 270


extern "C"{

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    void ker_doitgen_noOpt_extralarge(typeData *inD_A, typeData *inD_C4, typeData *outD_A) {

        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_C4 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A offset = slave bundle = gmem 

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_C4 bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        typeData sum[SIZE_P];

        //***************************************
        // KERNEL EXECUTION - START
        //***************************************
        for (int r = 0; r < SIZE_R; r++){
            for (int q = 0; q < SIZE_Q; q++){
                for (int p = 0; p < SIZE_P; p++){
                
                    for (int s = 0; s < SIZE_P; s++){
                        sum[p] = 0.0;
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
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************

}
