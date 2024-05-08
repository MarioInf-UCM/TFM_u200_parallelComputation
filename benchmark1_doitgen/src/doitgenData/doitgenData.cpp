#include <iostream>
#include <iomanip>
#include <math.h>
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
                #if defined(TYPEDATA_INT)
                    A[r][q][p] = (  (typeData) (  ((double) (((r*q + p)%SIZE_P)) * 10 ) / SIZE_P) );
                #elif defined(TYPEDATA_FP)
                    A[r][q][p] = (typeData) ((r*q + p)%SIZE_P) / SIZE_P;
                #endif
            }
        }
    }
    return;
}


void DoitgenData::initData_C4(){
    for (int p1 = 0; p1 < SIZE_P; p1++) {
        for (int p2 = 0; p2 < SIZE_P; p2++) {
            #if defined(TYPEDATA_INT)
                C4[p1][p2] = (typeData) floor(((p1*p2 % SIZE_P) / SIZE_P) *10) ;
            #elif defined(TYPEDATA_FP)
                C4[p1][p2] = (typeData) (p1*p2 % SIZE_P) / SIZE_P;
            #endif
        }
    }
    return;
}


void DoitgenData::initData_resultCPU(){
    for (int r = 0; r < SIZE_R; r++) {
        for (int q = 0; q < SIZE_Q; q++) {
            for (int p = 0; p < SIZE_P; p++){
                #if defined(TYPEDATA_INT)
                    resultCPU[r][q][p] = 0;
                #elif defined(TYPEDATA_FP)
                    resultCPU[r][q][p] = 0.0;
                #endif
            }
        }
    }
    return;
}


void DoitgenData::initData_resultDevice(){
    for (int r = 0; r < SIZE_R; r++) {
        for (int q = 0; q < SIZE_Q; q++) {
            for (int p = 0; p < SIZE_P; p++){
                #if defined(TYPEDATA_INT)
                    resultDevice[r][q][p] = 0;
                #elif defined(TYPEDATA_FP)
                    resultDevice[r][q][p] = 0.0;
                #endif
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
                #if defined(TYPEDATA_INT)
                    printf("%d  ", A[r][q][p]);
                #elif defined(TYPEDATA_FP)
                    printf("%f  ", A[r][q][p]);
                #endif
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
            #if defined(TYPEDATA_INT)
                printf("%d  ", C4[p1][p2]);
            #elif defined(TYPEDATA_FP)
                printf("%f  ", C4[p1][p2]);
            #endif
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
                #if defined(TYPEDATA_INT)
                    printf("%d  ", resultCPU[r][q][p]);
                #elif defined(TYPEDATA_FP)
                    printf("%f  ", resultCPU[r][q][p]);
                #endif                    
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
                #if defined(TYPEDATA_INT)
                    printf("%d  ", resultDevice[r][q][p]);
                #elif defined(TYPEDATA_FP)
                    printf("%f  ", resultDevice[r][q][p]);
                #endif
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
typeData (&DoitgenData::getA())[SIZE_R][SIZE_Q][SIZE_P]{
    return A;
}
void DoitgenData::setA(typeData (&data)[SIZE_R][SIZE_Q][SIZE_P]){
    for (int r = 0; r < SIZE_R; r++) {
        for (int q = 0; q < SIZE_Q; q++) {
            for (int p = 0; p < SIZE_P; p++) {
                A[r][q][p] = data[r][q][p];
            }
        }
    }
    return;       
}


typeData (&DoitgenData::getC4())[SIZE_P][SIZE_P]{
    return C4;
}
void DoitgenData::setC4(typeData (&data)[SIZE_P][SIZE_P]){
    for (int p1 = 0; p1 < SIZE_P; p1++) {
        for (int p2 = 0; p2 < SIZE_P; p2++) {
            C4[p1][p2] = data[p1][p2];
        }
    }  
    return;
}


typeData (&DoitgenData::getResultCPU())[SIZE_R][SIZE_Q][SIZE_P]{
    return resultCPU;
}
void DoitgenData::setResultCPU(typeData (&data)[SIZE_R][SIZE_Q][SIZE_P]){
    for (int r = 0; r < SIZE_R; r++) {
        for (int q = 0; q < SIZE_Q; q++) {
            for (int p = 0; p < SIZE_P; p++) {
                resultCPU[r][q][p] = data[r][q][p];
            }
        }
    }
    return;    
}


typeData (&DoitgenData::getResultDevice())[SIZE_R][SIZE_Q][SIZE_P]{
    return resultDevice;
}
void DoitgenData::setResultDevice(typeData (&data)[SIZE_R][SIZE_Q][SIZE_P]){
    for (int r = 0; r < SIZE_R; r++) {
        for (int q = 0; q < SIZE_Q; q++) {
            for (int p = 0; p < SIZE_P; p++) {
                resultDevice[r][q][p] = data[r][q][p];
            }
        }
    }
    return;    
}

