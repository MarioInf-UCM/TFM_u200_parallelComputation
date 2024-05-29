#include "gemmHost.hpp"

using namespace std;

GemmHost::GemmHost(){};
GemmHost::~GemmHost(){};



bool GemmHost::gemmHost_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    
    bool result = false;
    if(exec.get_host() == "doitgenHost_noOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"doitgenHost_noOpt\" host version");
        result = GemmHost::gemmHost_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
        
    }else if(exec.get_host() == "doitgenHost_mapOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"doitgenHost_mapOpt\" host version");
        result = GemmHost ::gemmHost_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else{
        return false;
    }

    return result;
}
