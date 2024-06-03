#include "choleskyHost.hpp"

using namespace std;


CholeskyHost::CholeskyHost(){};
CholeskyHost::~CholeskyHost(){};



bool CholeskyHost::choleskyHost_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    
    bool result = false;
    if(exec.get_host() == "choleskyHost_noOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"choleskyHost_noOpt\" host version");
        result = CholeskyHost_noOpt::choleskyHost_noOpt_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
        
    }else if(exec.get_host() == "choleskyHost_mapOpt"){
        //fileWriter_logFile.writeln("KernelPackage selected matches with \"choleskyHost_mapOpt\" host version");
        //result = CholeskyHost ::choleskyHost_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else if(exec.get_kernelPackage().find("") != string::npos){

    }else{
        return false;
    }

    return result;
}
