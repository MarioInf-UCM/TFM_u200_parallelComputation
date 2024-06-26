#ifndef _CONFIGPARAMS_HPP_
#define _CONFIGPARAMS_HPP_

#include <ap_fixed.h>
    //DEFINE TYPE OF DATA
    //******************************
    namespace globalConfiguration_typeData{
        typedef float typeData;
        #define TYPEDATA_BITS_SIZE 16
        #define TYPEDATA_BITS_INT 7
        typedef ap_fixed<TYPEDATA_BITS_SIZE, TYPEDATA_BITS_INT> typeData_fixed;
    }



    //DEFINE PARAMS FOR DOITGEN KERNEL
    //**************************************
    #if !defined(DOITGEN_R) && !defined(DOITGEN_Q) && !defined(DOITGEN_P)
        #define DOITGEN_R_MINI 8
        #define DOITGEN_Q_MINI 10
        #define DOITGEN_P_MINI 12

        #define DOITGEN_R_SMALL 20
        #define DOITGEN_Q_SMALL 25
        #define DOITGEN_P_SMALL 30

        #define DOITGEN_R_MEDIUM 40
        #define DOITGEN_Q_MEDIUM 50
        #define DOITGEN_P_MEDIUM 60

        #define DOITGEN_R_LARGE 140
        #define DOITGEN_Q_LARGE 150
        #define DOITGEN_P_LARGE 160

        #define DOITGEN_R_EXTRALARGE 220
        #define DOITGEN_Q_EXTRALARGE 250
        #define DOITGEN_P_EXTRALARGE 270
    #endif


    //DEFINE PARAMS FOR CHOLESKY KERNEL
    //**************************************
    #if !defined(CHOLESKY_N)
        #define CHOLESKY_N_MINI 40
        #define CHOLESKY_N_SMALL 120
        #define CHOLESKY_N_MEDIUM 400
        #define CHOLESKY_N_LARGE 2000
        #define CHOLESKY_N_EXTRALARGE 4000
    #endif


    //DEFINE PARAMS FOR GEMM KERNEL
    //**********************************
    #define GEMM_ALPHA 1.5
    #define GEMM_BETA 1.2

    #if !defined(GEMM_NI) && !defined(GEMM_NJ) && !defined(GEMM_NK)
        #define GEMM_I_MINI 20
        #define GEMM_J_MINI 25
        #define GEMM_K_MINI 30

        #define GEMM_I_SMALL 60
        #define GEMM_J_SMALL 70
        #define GEMM_K_SMALL 80

        #define GEMM_I_MEDIUM 200
        #define GEMM_J_MEDIUM 220
        #define GEMM_K_MEDIUM 240

        #define GEMM_I_LARGE 1000
        #define GEMM_J_LARGE 1100
        #define GEMM_K_LARGE 1200

        #define GEMM_I_EXTRALARGE 2000
        #define GEMM_J_EXTRALARGE 2300
        #define GEMM_K_EXTRALARGE 2600
    #endif


    //DEFINE PARAMS FOR JACOBI_2D KERNEL
    //**********************************
    # if !defined(TSTEPS) && !defined(N)
        #define JACOBI2D_TSTEPS_MINI 20
        #define JACOBI2D_N_MINI 30
        
        #define JACOBI2D_TSTEPS_SMALL 40
        #define JACOBI2D_N_SMALL 90
        
        #define JACOBI2D_TSTEPS_MEDIUM 100
        #define JACOBI2D_N_MEDIUM 250
        
        #define JACOBI2D_TSTEPS_LARGE 500
        #define JACOBI2D_N_LARGE 1300
        
        #define JACOBI2D_TSTEPS_EXTRALARGE 1000
        #define JACOBI2D_N_EXTRALARGE 2800
    #endif


    //DEFINE PARAMS FOR VECTORADD KERNEL
    //**********************************
    # if !defined(VECTORADD_SIZE)
        #define VECTORADD_SIZE_MINI 10
        #define VECTORADD_SIZE_SMALL 100
        #define VECTORADD_SIZE_MEDIUM 2000
        #define VECTORADD_SIZE_LARGE 50000
        #define VECTORADD_SIZE_EXTRALARGE 800000
    #endif
        



#endif