
#include <stdio.h>
#include <stdlib.h>
#define SIZE_R 15
#define SIZE_Q 14
#define SIZE_P 16

extern "C"{

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    void doitgenKernel(double *inD_A, double *inD_C4, double *outD_A) {

        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_C4 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A offset = slave bundle = gmem

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_C4 bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = SIZE_R bundle = control
        #pragma HLS INTERFACE s_axilite port = SIZE_Q bundle = control
        #pragma HLS INTERFACE s_axilite port = SIZE_P bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        double A[SIZE_R][SIZE_Q][SIZE_P];  //SIZE_R x SIZE_Q x SIZE_P
        double C4[SIZE_P][SIZE_P];         //SIZE_P x SIZE_P
        double sum[SIZE_P];                //SIZE_P
        bool result=true;

        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++){
                    A[r][q][p]=inD_A[(r*SIZE_R)+(q*SIZE_Q)+p];
                }
            }
        }

        for (int p1 = 0; p1 < SIZE_P; p1++) {
            for (int p2 = 0; p2 < SIZE_P; p2++) {
                C4[p1][p2]=inD_C4[(p1*SIZE_Q)+p2];
            }
        }


        if(!result){
            return;
        }

        for (int r = 0; r < SIZE_R; r++){
            for (int q = 0; q < SIZE_Q; q++){
                for (int p = 0; p < SIZE_P; p++){
                
                    for (int s = 0; s < SIZE_P; s++){
                        sum[p] += A[r][q][s] * C4[s][p];
                    }
                }

                for (int p = 0; p < SIZE_P; p++){
                    outD_A[(r*SIZE_R)+(q*SIZE_Q)+p] = sum[p];
                }
            }
        }
        
        return;
    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************

}
