#ifndef DOITGENDATA_HPP
#define DOITGENDATA_HPP

#include <vector>
#include <iostream>

#define PRINTRESULT true
#define SIZE_R 5
#define SIZE_Q 6
#define SIZE_P 7

class DoitgenData{

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:

        double A[SIZE_R][SIZE_Q][SIZE_P];               //SIZE_R x SIZE_Q x SIZE_P
        double C4[SIZE_P][SIZE_P];                      //SIZE_P x SIZE_P
        double resultCPU[SIZE_R][SIZE_Q][SIZE_P];       //SIZE_R x SIZE_Q x SIZE_P
        double resultDevice[SIZE_R][SIZE_Q][SIZE_P];    //SIZE_R x SIZE_Q x SIZE_P



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
        double (&getA())[SIZE_R][SIZE_Q][SIZE_P];
        void setA(double (&newA)[SIZE_R][SIZE_Q][SIZE_P]);

        double (&getC4())[SIZE_P][SIZE_P];
        void setC4(double (&newA)[SIZE_P][SIZE_P]);

        double (&getResultCPU())[SIZE_R][SIZE_Q][SIZE_P];
        void setResultCPU(double (&newA)[SIZE_R][SIZE_Q][SIZE_P]);

        double (&getResultDevice())[SIZE_R][SIZE_Q][SIZE_P];
        void setResultDevice(double (&newA)[SIZE_R][SIZE_Q][SIZE_P]);
        




    private:



};

#endif
