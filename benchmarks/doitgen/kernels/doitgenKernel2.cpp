#include "../src/utilities/polybench.hpp"
#include "../src/doitgen.hpp"


extern "C"{

    void kernel_doitgen(int nr, int nq, int np,
                DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
                DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
                DATA_TYPE POLYBENCH_1D(sum,NP,np))
    {

    #pragma HLS INTERFACE m_axi port = A offset = slave bundle = gmem
    #pragma HLS INTERFACE m_axi port = C4 offset = slave bundle = gmem
    #pragma HLS INTERFACE m_axi port = sum offset = slave bundle = gmem

    #pragma HLS INTERFACE s_axilite port = A bundle = control
    #pragma HLS INTERFACE s_axilite port = C4 bundle = control
    #pragma HLS INTERFACE s_axilite port = sum bundle = control
    #pragma HLS INTERFACE s_axilite port = nr bundle = control
    #pragma HLS INTERFACE s_axilite port = nq bundle = control
    #pragma HLS INTERFACE s_axilite port = np bundle = control
    #pragma HLS INTERFACE s_axilite port = NR bundle = control
    #pragma HLS INTERFACE s_axilite port = NQ bundle = control
    #pragma HLS INTERFACE s_axilite port = NP bundle = control


    int r, q, p, s;

    for (r = 0; r < _PB_NR; r++){
        for (q = 0; q < _PB_NQ; q++){
        for (p = 0; p < _PB_NP; p++){
            sum[p] = SCALAR_VAL(0.0);
            for (s = 0; s < _PB_NP; s++){
                sum[p] += A[r][q][s] * C4[s][p];
            }
        }
        for (p = 0; p < _PB_NP; p++){
            A[r][q][p] = sum[p];
        }
        }
    }

    }


}