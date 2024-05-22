#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

    //DEFINE TYPE OF DATA
    //******************************
    #if defined(TYPEDATA_INT)
        typedef int typeData;
    #elif defined(TYPEDATA_FP)
        typedef float typeData;
    #else
        typedef float typeData;
    #endif


    #if !defined(SIZE_R) && !defined(SIZE_Q) && !defined(SIZE_P)
        #define SIZE_R_MINI 8
        #define SIZE_Q_MINI 10
        #define SIZE_P_MINI 12

        #define SIZE_R_SMALL 20
        #define SIZE_Q_SMALL 25
        #define SIZE_P_SMALL 30

        #define SIZE_R_MEDIUM 40
        #define SIZE_Q_MEDIUM 50
        #define SIZE_P_MEDIUM 60

        #define SIZE_R_LARGE 140
        #define SIZE_Q_LARGE 150
        #define SIZE_P_LARGE 160

        #define SIZE_R_EXTRALARGE 220
        #define SIZE_Q_EXTRALARGE 250
        #define SIZE_P_EXTRALARGE 270
    #endif

#endif