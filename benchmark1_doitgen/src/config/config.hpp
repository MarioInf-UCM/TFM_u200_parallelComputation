#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

    //DEFINE TYPE OF DATA
    //******************************
    #if defined(TYPEDATA_INT)
        typedef int typeData;
    #elif defined(TYPEDATA_FP)
        typedef float typeData;
    #else
        typedef int typeData;
    #endif

           
    //DEFINE FLAG TO PRONT RESULT
    //******************************
    #if !defined(PRINTRESULT)
        #define PRINTRESULT false
    #endif

 
    //DEFINE DATASET SIZE
    //******************************
    #if !defined(MINI_DATASET) && !defined(SMALL_DATASET) && !defined(MEDIUM_DATASET) && !defined(LARGE_DATASET) && !defined(EXTRALARGE_DATASET)
        #define MEDIUM_DATASET
    #endif

    #if !defined(SIZE_R) && !defined(SIZE_Q) && !defined(SIZE_P)
        #ifdef MINI_DATASET
            #define SIZE_R 8
            #define SIZE_Q 10
            #define SIZE_P 12
        #endif

        #ifdef SMALL_DATASET
            #define SIZE_R 20
            #define SIZE_Q 25
            #define SIZE_P 30
        #endif

        #ifdef MEDIUM_DATASET
            #define SIZE_R 40
            #define SIZE_Q 50
            #define SIZE_P 60
        #endif

        #ifdef LARGE_DATASET
            #define SIZE_R 140
            #define SIZE_Q 150
            #define SIZE_P 160
        #endif

        #ifdef EXTRALARGE_DATASET
            #define SIZE_R 220
            #define SIZE_Q 250
            #define SIZE_P 270
        #endif
    #endif


#endif