#ifndef _CHOLESKYKERNEL_HPP_
#define _CHOLESKYKERNEL_HPP_

#include <vector>
#include <iostream>
#include "../../configParams/configParams.hpp"

using namespace std;


class CholeskyKernel{

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:
        unsigned int SIZE_N;

        vector< vector<typeData>> A;               //SIZE_N x SIZE_N
        vector< vector<typeData>> resultCPU;       //SIZE_N x SIZE_N
        vector< vector<typeData>> resultCPU_opt;   //SIZE_N x SIZE_N
        vector< vector<typeData>> resultDevice;    //SIZE_N x SIZE_N



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:

        //********************************
        //* CONSTRUCTORS AND DESTRUCTORS *
        //********************************
        CholeskyKernel(unsigned int SIZE_N);
        ~CholeskyKernel();

        void initData_A(unsigned int SIZE_N);
        void initData_resultCPU(unsigned int SIZE_N);
        void initData_resultCPU_opt(unsigned int SIZE_N);
        void initData_resultDevice(unsigned int SIZE_N);
        
        string printData_A();
        string printData_resultCPU();
        string printData_resultCPU_opt();
        string printData_resultDevice();
        string printAll();

        void kernel_cholesky_CPU();
        void kernel_cholesky_CPU_opt();



        //*************************
        //* GET AND SET FUNCTIONS *
        //*************************
        unsigned int get_SIZE_N();
        void set_SIZE_N(unsigned int data);

        vector< vector<typeData>>& get_A();
        void set_A(vector< vector<typeData>> newList);

        vector< vector<typeData>>& get_resultCPU();
        void set_resultCPU(vector< vector<typeData>> newList);

        vector< vector<typeData>>& get_resultCPU_opt();
        void set_resultCPU_opt(vector< vector<typeData>> newList);

        vector< vector<typeData>>& get_resultDevice();
        void set_resultDevice(vector< vector<typeData>> newList);

        
};

#endif
