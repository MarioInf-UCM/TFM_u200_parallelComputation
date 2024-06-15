#include "gemmHost.hpp"

using namespace std;

GemmHost::GemmHost(){};
GemmHost::~GemmHost(){};



bool GemmHost::exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){

    bool result = false;
    string hostName = exec.get_host();
    for (char& c : hostName) {
        c = tolower(c);
    } 

    if(hostName == Helper::getNameOfType(typeid(GemmHost_Opt0_1).name())){
        fileWriter_logFile.writeln("KernelPackage selected matches with" + Helper::getNameOfType(typeid(GemmHost_Opt0_1).name()) + "host version");
        result = GemmHost_Opt0_1::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);
        
    }else if(hostName == Helper::getNameOfType(typeid(GemmHost_Opt2).name())){
        fileWriter_logFile.writeln("KernelPackage selected matches with " + Helper::getNameOfType(typeid(GemmHost_Opt2).name()) + " host version");
        result = GemmHost_Opt2::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else{
        fileWriter_logFile.writeln("ERROR..: Doitgen host name unknow (" + hostName + ").");
        return false;
    }

    return result;
}
