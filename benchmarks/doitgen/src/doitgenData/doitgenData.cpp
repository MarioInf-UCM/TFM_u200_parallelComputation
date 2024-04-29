#include <iostream>
#include <iomanip>
#include "doitgenData.hpp"

using namespace std;

    //********************************
    //* CONSTRUCTORS AND DESTRUCTORS *
    //********************************
    DoitgenData::DoitgenData(){
        initData_A();
        initData_C4();
        initData_resultCPU();
        initData_resultDevice();
    }
    DoitgenData::~DoitgenData(){}



    //*********************
    //* GENERAL FUNCTIONS *
    //*********************
    void DoitgenData::initData_A(){
        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++){
                    A[r][q][p] = (double) ((r*q + p)%SIZE_P) / SIZE_P;
                }
            }
        }
        return;
    }


    void DoitgenData::initData_C4(){
        for (int p1 = 0; p1 < SIZE_P; p1++) {
            for (int p2 = 0; p2 < SIZE_P; p2++) {
                C4[p1][p2] = (double) (p1*p2 % SIZE_P) / SIZE_P;
            }
        }
        return;
    }


    void DoitgenData::initData_resultCPU(){
        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++){
                    resultCPU[r][q][p] = 0.0;
                }
            }
        }
        return;
    }


    void DoitgenData::initData_resultDevice(){
        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++){
                    resultDevice[r][q][p] = 0.0;
                }
            }
        }
        return;
    }


    void DoitgenData::printData_A(){
        for (int r = 0; r < SIZE_R; r++) {
            cout << "\nARRAY A, DIMENSION R=" << r+1 << " of " << SIZE_R << endl;
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++) {
                    printf("%f  ", A[r][q][p]);
                }
                cout << endl;
            }
            cout << endl;
        }
        return;
    }


    void DoitgenData::printData_C4(){
        for (int p1 = 0; p1 < SIZE_P; p1++) {
            for (int p2 = 0; p2 < SIZE_P; p2++) {
                printf("%f  ", C4[p1][p2]);
            }
            cout << endl;
        }
        cout << endl;
        return;
    }

    
    void DoitgenData::printData_resultCPU(){
        for (int r = 0; r < SIZE_R; r++) {
            cout << "\nARRAY RESULTCPU, DIMENSION R=" << r+1 << " of " << SIZE_R << endl;
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++) {
                    printf("%f  ", resultCPU[r][q][p]);
                }
                cout << endl;
            }
            cout << endl;
        }
        return;
    }


    void DoitgenData::printData_resultDevice(){
        for (int r = 0; r < SIZE_R; r++) {
            cout << "\nARRAY RESULTDEVICE, DIMENSION R=" << r+1 << " of " << SIZE_R << endl;
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++) {
                    printf("%f  ", resultDevice[r][q][p]);
                }
                cout << endl;
            }
            cout << endl;
        }
        return;
    }



    //*************************
    //* GET AND SET FUNCTIONS *
    //*************************
    double (&DoitgenData::getA())[SIZE_R][SIZE_Q][SIZE_P]{
        return A;
    }
    void DoitgenData::setA(double (&data)[SIZE_R][SIZE_Q][SIZE_P]){
        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++) {
                    A[r][q][p] = data[r][q][p];
                }
            }
        }
        return;       
    }


    double (&DoitgenData::getC4())[SIZE_P][SIZE_P]{
        return C4;
    }
    void DoitgenData::setC4(double (&data)[SIZE_P][SIZE_P]){
        for (int p1 = 0; p1 < SIZE_P; p1++) {
            for (int p2 = 0; p2 < SIZE_P; p2++) {
                C4[p1][p2] = data[p1][p2];
            }
        }  
        return;
    }


    double (&DoitgenData::getResultCPU())[SIZE_R][SIZE_Q][SIZE_P]{
        return resultCPU;
    }
    void DoitgenData::setResultCPU(double (&data)[SIZE_R][SIZE_Q][SIZE_P]){
        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++) {
                    resultCPU[r][q][p] = data[r][q][p];
                }
            }
        }
        return;    
    }


    double (&DoitgenData::getResultDevice())[SIZE_R][SIZE_Q][SIZE_P]{
        return resultDevice;
    }
    void DoitgenData::setResultDevice(double (&data)[SIZE_R][SIZE_Q][SIZE_P]){
        for (int r = 0; r < SIZE_R; r++) {
            for (int q = 0; q < SIZE_Q; q++) {
                for (int p = 0; p < SIZE_P; p++) {
                    resultDevice[r][q][p] = data[r][q][p];
                }
            }
        }
        return;    
    }

