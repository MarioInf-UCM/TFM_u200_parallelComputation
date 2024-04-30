#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_


    #if defined(TYPEDATA_INT)
        typedef int typeData;
    #elif defined(TYPEDATA_FP)
        typedef float typeData;
    #else
        typedef int typeData;
    #endif

    #define PRINTRESULT true
    #define SIZE_R 5
    #define SIZE_Q 6
    #define SIZE_P 7


#endif