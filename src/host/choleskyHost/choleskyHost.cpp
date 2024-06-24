#include "choleskyHost.hpp"

using namespace std;


CholeskyHost::CholeskyHost(){};
CholeskyHost::~CholeskyHost(){};



bool CholeskyHost::exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    
    bool result = false;
    string hostName = exec.get_host();
    for (char& c : hostName) {
        c = tolower(c);
    } 

    if(hostName == Helper::getNameOfType(typeid(CholeskyHost_Opt0).name())){
        fileWriter_logFile.writeln("KernelPackage selected matches with" + Helper::getNameOfType(typeid(CholeskyHost_Opt0).name()) + "host version");
        result = CholeskyHost_Opt0::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
        
    }else if(hostName == Helper::getNameOfType(typeid(CholeskyHost_Opt1).name())){
        fileWriter_logFile.writeln("KernelPackage selected matches with" + Helper::getNameOfType(typeid(CholeskyHost_Opt1).name()) + "host version");
        result = CholeskyHost_Opt1::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
        
    }else if(hostName == Helper::getNameOfType(typeid(CholeskyHost_Opt2).name())){
        fileWriter_logFile.writeln("KernelPackage selected matches with " + Helper::getNameOfType(typeid(CholeskyHost_Opt2).name()) + " host version");
        result = CholeskyHost_Opt2::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else{
        fileWriter_logFile.writeln("ERROR..: Doitgen host name unknow (" + hostName + ").");
        return false;
    }

    return result;
}
