#include "doitgenHost.hpp"

using namespace std;

DoitgenHost::DoitgenHost(){};
DoitgenHost::~DoitgenHost(){};



bool DoitgenHost::doitgenHost_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    
    bool result = false;
    if(exec.get_host() == "doitgenHost_noOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"doitgenHost_noOpt\" host version");
        result = DoitgenHost_noOpt::doitgenHost_noOpt_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
        
    }else if(exec.get_host() == "doitgenHost_mapOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"doitgenHost_mapOpt\" host version");
        result = DoitgenHost_mapOpt ::doitgenHost_mapOpt_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else{
        fileWriter_logFile.writeln("ERROR..: Host name unknow.");
        return false;
    }

    return result;
}
