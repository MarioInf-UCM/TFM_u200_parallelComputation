#include "jacobi_2dHost.hpp"

using namespace std;


Jacobi_2dHost::Jacobi_2dHost(){};
Jacobi_2dHost::~Jacobi_2dHost(){};



bool Jacobi_2dHost::jacobi_2dHost_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    
    bool result = false;
    if(exec.get_host() == "jacobi_2dHost_noOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"jacobi_2dHost_noOpt\" host version");
        result = Jacobi_2dHost_noOpt::Jacobi_2dHost_noOpt_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
    }else{
        return false;
    }

    return result;
}
