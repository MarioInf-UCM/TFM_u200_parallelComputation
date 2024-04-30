#ifndef DOITGENDATA_HPP
#define DOITGENDATA_HPP

#include <vector>
#include <iostream>
#include "../config/config.hpp"

class DoitgenData{

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:

        typeData A[SIZE_R][SIZE_Q][SIZE_P];               //SIZE_R x SIZE_Q x SIZE_P
        typeData C4[SIZE_P][SIZE_P];                      //SIZE_P x SIZE_P
        typeData resultCPU[SIZE_R][SIZE_Q][SIZE_P];       //SIZE_R x SIZE_Q x SIZE_P
        typeData resultDevice[SIZE_R][SIZE_Q][SIZE_P];    //SIZE_R x SIZE_Q x SIZE_P



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:

        //********************************
        //* CONSTRUCTORS AND DESTRUCTORS *
        //********************************
        DoitgenData();
        ~DoitgenData();


        void initData_A();
        void initData_C4();
        void initData_resultCPU();
        void initData_resultDevice();
        
        void printData_A();
        void printData_C4();
        void printData_resultCPU();
        void printData_resultDevice();


        //*************************
        //* GET AND SET FUNCTIONS *
        //*************************
        typeData (&getA())[SIZE_R][SIZE_Q][SIZE_P];
        void setA(typeData (&newA)[SIZE_R][SIZE_Q][SIZE_P]);

        typeData (&getC4())[SIZE_P][SIZE_P];
        void setC4(typeData (&newA)[SIZE_P][SIZE_P]);

        typeData (&getResultCPU())[SIZE_R][SIZE_Q][SIZE_P];
        void setResultCPU(typeData (&newA)[SIZE_R][SIZE_Q][SIZE_P]);

        typeData (&getResultDevice())[SIZE_R][SIZE_Q][SIZE_P];
        void setResultDevice(typeData (&newA)[SIZE_R][SIZE_Q][SIZE_P]);
        
};

#endif
