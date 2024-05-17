#include "doitgenHost.hpp"

using namespace std;

bool DoitgenHost::doitgenHost_exec(Execution exec, FileWriter_service fileWriter_logFile){
    
    if(exec.get_kernelPackage().find("noOpt") != string::npos){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"noOpt\" host version");
        doitgenHost_noOpt_exec(exec, fileWriter_logFile);
        
    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else{
        return false;
    }

    return true;
}
