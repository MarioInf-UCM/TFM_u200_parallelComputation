#ifndef _CONFIGPARAMS_HPP_
#define _CONFIGPARAMS_HPP_

    //DEFINE TYPE OF DATA
    //******************************
    #if defined(TYPEDATA_INT)
        typedef int typeData;
    #elif defined(TYPEDATA_FP)
        typedef float typeData;
    #else
        typedef float typeData;
    #endif



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

#endif