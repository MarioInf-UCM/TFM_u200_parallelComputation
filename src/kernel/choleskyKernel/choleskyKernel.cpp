#include <iostream>
#include <iomanip>
#include <cmath> 
#include "choleskyKernel.hpp"

using namespace std;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
CholeskyKernel::CholeskyKernel(unsigned int SIZE_N):
    SIZE_N(SIZE_N),
    A(vector< vector<typeData>>()),
    resultCPU(vector< vector<typeData>>()),
    resultCPU_opt(vector< vector<typeData>>()),
    resultDevice(vector< vector<typeData>>())
{
    initData_A(SIZE_N);
    initData_resultCPU(SIZE_N);
    initData_resultCPU_opt(SIZE_N);
    initData_resultDevice(SIZE_N);
}
CholeskyKernel::~CholeskyKernel(){}



//*********************
//* GENERAL FUNCTIONS *
//*********************
void CholeskyKernel::initData_A(unsigned int SIZE_N){
    for (int n1 = 0; n1 < get_SIZE_N(); n1++) {
        get_A().push_back(vector<typeData>(get_SIZE_N()));
        for (int n2 = 0; n2 <= get_SIZE_N(); n2++){
            get_A()[n1].push_back(0.0f);
        }
        for (int n2 = 0; n2 <= n1; n2++){
            get_A()[n1][n2] = (typeData)(-n2 % SIZE_N) / SIZE_N + 1;
        }
        for (int n2 = n1+1; n2 < SIZE_N; n2++) {
            get_A()[n1][n2] = 0.0f;
        }
        get_A()[n1][n1] = 1.0f;
    }
    return;
}


void CholeskyKernel::initData_resultCPU(unsigned int SIZE_N){
    for (int n1 = 0; n1 < get_SIZE_N(); n1++) {
        get_resultCPU().push_back(vector<typeData>(get_SIZE_N()));
        for (int n2 = 0; n2 <= get_SIZE_N(); n2++){
            get_resultCPU()[n1].push_back(0.0f);
        }
    }
    return;
}


void CholeskyKernel::initData_resultCPU_opt(unsigned int SIZE_N){
    for (int n1 = 0; n1 < get_SIZE_N(); n1++) {
        get_resultCPU_opt().push_back(vector<typeData>(get_SIZE_N()));
        for (int n2 = 0; n2 <= get_SIZE_N(); n2++){
            get_resultCPU_opt()[n1].push_back(0.0f);
        }
    }
    return;
}


void CholeskyKernel::initData_resultDevice(unsigned int SIZE_N){
    for (int n1 = 0; n1 < get_SIZE_N(); n1++) {
        get_resultDevice().push_back(vector<typeData>(get_SIZE_N()));
        for (int n2 = 0; n2 <= get_SIZE_N(); n2++){
            get_resultDevice()[n1].push_back(0.0f);
        }
    }
    return;
}



string CholeskyKernel::printData_A(){
    string result="";
    for (int n1 = 0; n1 < get_A().size(); n1++) {
        for (int n2 = 0; n2 <= get_A()[n1].size(); n2++){
            result += to_string(get_A()[n1][n2]) + "  ";
        }
        result += "\n";
    }
    return result;
}



string CholeskyKernel::printData_resultCPU(){
    string result="";
    for (int n1 = 0; n1 < get_resultCPU().size(); n1++) {
        for (int n2 = 0; n2 <= get_resultCPU()[n1].size(); n2++){
            result += to_string(get_resultCPU()[n1][n2]) + "  ";
        }
        result += "\n";
    }
    return result;
}


string CholeskyKernel::printData_resultCPU_opt(){
    string result="";
    for (int n1 = 0; n1 < get_resultCPU_opt().size(); n1++) {
        for (int n2 = 0; n2 <= get_resultCPU_opt()[n1].size(); n2++){
            result += to_string(get_resultCPU_opt()[n1][n2]) + "  ";
        }
        result += "\n";
    }
    return result;
}


string CholeskyKernel::printData_resultDevice(){
    string result="";
    for (int n1 = 0; n1 < get_resultDevice().size(); n1++) {
        for (int n2 = 0; n2 <= get_resultDevice()[n1].size(); n2++){
            result += to_string(get_resultDevice()[n1][n2]) + "  ";
        }
        result += "\n";
    }
    return result;
}


string CholeskyKernel::printAll(){
    string result="";
    result += "RESULTS ARRAY A\n";
    result += "=====================\n";
    result += printData_A() + "\n";

    result += "RESULTS FROM CPU\n";
    result += "=====================\n";
    result += printData_resultCPU() + "\n";

    result += "RESULTS FROM CPU OPTIMIZED\n";
    result += "=====================\n";
    result += printData_resultCPU_opt() + "\n";

    result += "RESULTS FROM DEVICE\n";
    result += "=====================\n";
    result += printData_resultDevice() + "\n";

  return result;
}



void CholeskyKernel::kernel_cholesky_CPU(){
    
    for (int i = 0; i < get_SIZE_N(); i++) {
        for (int j = 0; j < i; j++) {
            for (int k = 0; k < j; k++) {
            get_resultCPU()[i][j] -= get_A()[i][k] * get_A()[j][k];
            }
            get_resultCPU()[i][j] /= get_A()[j][j];
        }
        for (int k = 0; k < i; k++) {
            get_resultCPU()[i][i] -= get_A()[i][k] * get_A()[i][k];
        }
        get_resultCPU()[i][i] = sqrt(get_A()[i][i]);
    }

  return;
}


void CholeskyKernel::kernel_cholesky_CPU_opt() {
/*
    const auto& A = get_A();
    const auto& C4 = get_C4();
    const unsigned int SIZE_R = get_SIZE_R();
    const unsigned int SIZE_Q = get_SIZE_Q();
    const unsigned int SIZE_P = get_SIZE_P();
    auto& resultCPU_opt = get_resultCPU_opt();
    vector<typeData> sum(get_SIZE_P(), 0.0);

    #pragma omp parallel for collapse(2) firstprivate(sum) shared(resultCPU_opt)
    for (int r = 0; r < SIZE_R; r++) {
        for (int q = 0; q < SIZE_Q; q++) {
            for (int p = 0; p < SIZE_P; p++) {
                sum[p] = 0.0;
                
                #pragma omp simd
                for (int s = 0; s < SIZE_P; s++) {
                    sum[p] += A[r][q][s] * C4[s][p];
                }
            }

            for (int p = 0; p < SIZE_P; p++) {
                #pragma omp atomic write
                    resultCPU_opt[r][q][p] = sum[p];
            }
        }
    }

*/
    return;
}



//*************************
//* GET AND SET FUNCTIONS *
//*************************
unsigned int CholeskyKernel::get_SIZE_N(){ return SIZE_N; }
void CholeskyKernel::set_SIZE_N(unsigned int data){ SIZE_N=data; }


vector< vector<typeData>>& CholeskyKernel::get_A(){ 
    return A; 
}
void CholeskyKernel::set_A(vector< vector<typeData>> newList){
    get_A().clear();
    for (int n1=0 ; n1<get_SIZE_N() ; n1++) {
        get_A().push_back(vector<typeData>());
        for (int n2=0 ; n2<newList.size() ; n2++) {
            get_A()[n1].push_back(newList[n1][n2]);
        }
    }
}



vector< vector<typeData>>& CholeskyKernel::get_resultCPU(){
    return resultCPU;
}
void CholeskyKernel::set_resultCPU(vector< vector<typeData>> newList){
    get_resultCPU().clear();
    for (int n1=0 ; n1<get_SIZE_N() ; n1++) {
        get_resultCPU().push_back(vector<typeData>());
        for (int n2=0 ; n2<newList.size() ; n2++) {
            get_resultCPU()[n1].push_back(newList[n1][n2]);
        }
    }
}



vector< vector<typeData>>& CholeskyKernel::get_resultCPU_opt(){
    return resultCPU_opt;
}
void CholeskyKernel::set_resultCPU_opt(vector< vector<typeData>>newList){
    get_resultCPU_opt().clear();
    for (int n1=0 ; n1<get_SIZE_N() ; n1++) {
        get_resultCPU_opt().push_back(vector<typeData>());
        for (int n2=0 ; n2<newList.size() ; n2++) {
            get_resultCPU_opt()[n1].push_back(newList[n1][n2]);
        }
    }
}



vector< vector<typeData>>& CholeskyKernel::get_resultDevice(){
    return resultDevice;
}
void CholeskyKernel::set_resultDevice(vector< vector<typeData>> newList){
    get_resultDevice().clear();
    for (int n1=0 ; n1<get_SIZE_N() ; n1++) {
        get_resultDevice().push_back(vector<typeData>());
        for (int n2=0 ; n2<newList.size() ; n2++) {
            get_resultDevice()[n1].push_back(newList[n1][n2]);
        }
    }
}
