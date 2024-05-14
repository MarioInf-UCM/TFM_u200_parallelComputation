#include <iostream>
#include "execution.hpp"


//*******************************
// CONSTRUCTORS DEFINITION ZONE *
//*******************************
Execution::Execution():
    kernelPackage(""),
    kernel(""),
    numExecutions(0)
{}
Execution::Execution(string kernelPackage, string kernel, int numExecutions):
    kernelPackage(kernelPackage),
    kernel(kernel),
    numExecutions(numExecutions)
{}
Execution::~Execution(){}



//************************************
// DEFINICIÓN DE MÉTODOS FUNCIONALES *
//************************************
string Execution::displayInfo(){
    string info = "Kernel Package: " + get_kernelPackage() + "\n";
    info += "Kernel: " + get_kernel() + "\n";
    info += "Number of Executions: " + to_string(get_numExecutions()) + "\n";
    return info;
}



//*************************
//* GET AND SET FUNCTIONS *
//*************************
string Execution::get_kernelPackage() const{ return kernelPackage; }
void Execution::set_kernelPackage(string data){ kernelPackage=data; }

string Execution::get_kernel() const{ return kernel; }
void Execution::set_kernel(string data){ kernel=data; }

int Execution::get_numExecutions() const{ return numExecutions; }
void Execution::set_numExecutions(int data){numExecutions=data; }