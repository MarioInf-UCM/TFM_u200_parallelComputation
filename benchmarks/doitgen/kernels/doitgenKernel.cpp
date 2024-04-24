#include "../src/utilities/polybench.hpp"
#include "../src/doitgen.hpp"


extern "C"{

    void doitgenKernel(double ***A, double **C4, double *sum, int nr, int nq, int np) {

        #pragma HLS INTERFACE m_axi port = A offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = C4 offset = slave bundle = gmem
        #pragma HLS INTERFACE m_axi port = sum offset = slave bundle = gmem

        #pragma HLS INTERFACE s_axilite port = A bundle = control
        #pragma HLS INTERFACE s_axilite port = C4 bundle = control
        #pragma HLS INTERFACE s_axilite port = sum bundle = control
        #pragma HLS INTERFACE s_axilite port = nr bundle = control
        #pragma HLS INTERFACE s_axilite port = nq bundle = control
        #pragma HLS INTERFACE s_axilite port = np bundle = control
        #pragma HLS INTERFACE s_axilite port = return bundle = control

        for (int r = 0; r < nr; r++){
            for (int q = 0; q < nq; q++){
                for (int p = 0; p < np; p++){
                
                    for (int s = 0; s < np; s++){
                        sum[p] += A[r][q][s] * C4[s][p];
                    }
                }

                for (int p = 0; p < np; p++){
                    A[r][q][p] = sum[p];
                }
            }
        }
        
        return;
    }

}