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
string Execution::displayInfo(string initLineString){
    string info = initLineString + "{\n";
    info += initLineString + "Host: " + get_host() + "\n";
    info += initLineString + "Kernel Package: " + get_kernelPackage() + "\n";
    info += initLineString + "Kernel: " + get_kernel() + "\n";
    info += initLineString + "Data Size: " + get_dataSize() + "\n";
    info += initLineString + "Print Results?: " + (get_printResults()? "true":"false") + "\n";
    info += initLineString + "Number of Executions: " + to_string(get_numExecutions()) + "\n";
    info += initLineString + "}\n";
    return info;
}



//*************************
//* GET AND SET FUNCTIONS *
//*************************
string Execution::get_host() const{ return host; }
void Execution::set_host(string data){ host=data; }

string Execution::get_kernelPackage() const{ return kernelPackage; }
void Execution::set_kernelPackage(string data){ kernelPackage=data; }

string Execution::get_kernel() const{ return kernel; }
void Execution::set_kernel(string data){ kernel=data; }

string Execution::get_dataSize() const{ return dataSize; }
void Execution::set_dataSize(string data){ dataSize=data; }

bool Execution::get_printResults() const{ return printResults; }
void Execution::set_printResults(bool data){ printResults = data; }

int Execution::get_numExecutions() const{ return numExecutions; }
void Execution::set_numExecutions(int data){numExecutions=data; }