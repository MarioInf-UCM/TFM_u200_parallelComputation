
#include <stdio.h>
#include <stdlib.h>
#define SIZE_R 15
#define SIZE_Q 14
#define SIZE_P 16

extern "C"{

    //*************************************
    // MAIN KERNEL FUNCTION - START
    //*************************************
    void doitgenKernel(double *inD_A, double *inD_C4, double *outD_A, int nr, int nq, int np) {

        #pragma HLS INTERFACE m_axi port = inD_A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = inD_C4 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = outD_A offset = slave bundle = gmem

        #pragma HLS INTERFACE s_axilite port = inD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = inD_C4 bundle = control
        #pragma HLS INTERFACE s_axilite port = outD_A bundle = control
        #pragma HLS INTERFACE s_axilite port = nr bundle = control
        #pragma HLS INTERFACE s_axilite port = nq bundle = control
        #pragma HLS INTERFACE s_axilite port = np bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        double A[SIZE_R][SIZE_Q][SIZE_P];  //SIZE_R x SIZE_Q x SIZE_P
        double C4[SIZE_P][SIZE_P];         //SIZE_P x SIZE_P
        double sum[SIZE_P];                //SIZE_P
        bool result=true;

        for (int r = 0; r < nr; r++) {
            for (int q = 0; q < nq; q++) {
                for (int p = 0; p < np; p++){
                    A[r][q][p]=inD_A[(r*nr)+(q*nq)+p];
                }
            }
        }

        for (int p1 = 0; p1 < np; p1++) {
            for (int p2 = 0; p2 < np; p2++) {
                C4[p1][p2]=inD_C4[(p1*nq)+p2];
            }
        }


        if(!result){
            return;
        }

        for (int r = 0; r < nr; r++){
            for (int q = 0; q < nq; q++){
                for (int p = 0; p < np; p++){
                
                    for (int s = 0; s < np; s++){
                        sum[p] += A[r][q][s] * C4[s][p];
                    }
                }

                for (int p = 0; p < np; p++){
                    outD_A[(r*nr)+(q*nq)+p] = sum[p];
                }
            }
        }
        
        return;
    }
    //*************************************
    // MAIN KERNEL FUNCTION - END
    //*************************************

}
