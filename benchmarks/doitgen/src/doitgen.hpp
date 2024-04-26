#ifndef DOITGEN_HPP
#define DOITGEN_HPP

#include <vector>
#include <iostream>

#define PRINTRESULT true
#define SIZE_R 15
#define SIZE_Q 14
#define SIZE_P 16

class Doitgen {

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:

        double A[SIZE_R][SIZE_Q][SIZE_P];               //SIZE_R x SIZE_Q x SIZE_P
        double C4[SIZE_P][SIZE_P];                      //SIZE_P x SIZE_P
        double sum[SIZE_P];                             //SIZE_P
        double resultDevice[SIZE_R][SIZE_Q][SIZE_P];    //SIZE_R x SIZE_Q x SIZE_P


    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        Doitgen();
        ~Doitgen();





};

#endif
