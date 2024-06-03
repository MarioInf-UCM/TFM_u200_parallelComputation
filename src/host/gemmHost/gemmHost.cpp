#include "gemmHost.hpp"

using namespace std;

GemmHost::GemmHost(){};
GemmHost::~GemmHost(){};



bool GemmHost::gemmHost_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    
    bool result = false;
    if(exec.get_host() == "gemmHost_noOpt"){
        fileWriter_logFile.writeln("KernelPackage selected matches with \"gemmHost_noOpt\" host version");
        result = GemmHost_noOpt::gemmHost_noOpt_exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else{
        return false;
    }

    return result;
}
