#include <iostream>
#include <unistd.h>
#include <cmath>
#include "configParams/configParams.hpp"
#include "host/choleskyHost/choleskyHost.hpp"
#include "host/doitgenHost/doitgenHost.hpp"
#include "host/gemmHost/gemmHost.hpp"
#include "host/jacobi2dHost/jacobi2dHost.hpp"
#include "host/vectorAddHost/vectorAddHost.hpp"
#include "utilities/dateAndTime/dateAndTime.hpp"
#include "service/json_service/json_service.hpp"
#include "service/json_service/jsonConfiguration/jsonConfiguration.hpp"
#include "service/externProgramsConnection_service/externProgramsConnection_service.hpp"

bool runExecution(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);
void printFinalMessage(unsigned int numFailures, FileWriter_service fileWriter_logFile);
using namespace std;


//*************************************
// MAIN FUNCTION - START
//*************************************
int main(int argc, char** argv){

    Json_service json_service = Json_service(argv[1]);
    JsonConfiguration jsonConfiguration = json_service.getJSONConfiguration_FromFile();
    if(jsonConfiguration.get_status() == false){
        cerr << "Closing Application..." << endl;
        return 0;
    }

    unsigned int numFailures = 0;
    bool result = false;
    string outForderID = "/exec_" +DateAndTime::getCurrentDateTime();
    FileWriter_service fileWriter_statsFile;
    FileWriter_service fileWriter_logFile;
    ExternProgramsConnection_service externProgramConnection;
    vector<double> testAverageResults = vector<double>();
    vector<double> executionResults = vector<double>();
    string tempString_toWrite="";

    fileWriter_logFile = FileWriter_service(jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_logFile(), jsonConfiguration.get_verbose());
    fileWriter_logFile.writeln("Contenido del fichero de configuración:");
    fileWriter_logFile.writeln(jsonConfiguration.displayInfo("\t"));

    for(int i=0 ; i<jsonConfiguration.get_testList().size() ; i++){
        fileWriter_statsFile = FileWriter_service(jsonConfiguration.get_outDir() + outForderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile(), jsonConfiguration.get_verbose());
        fileWriter_statsFile.writeln("x,CPU time execution,CPU time execution optimizated,Device time execution,Transmision (S+R) time,Send to device time, Recieve from device time", false);
    
        for(int j=0 ; j<jsonConfiguration.get_testList()[i].get_executionList().size() ; j++){
            testAverageResults.clear();
            
            for(int k=0 ; k<jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions() ; k++){
                fileWriter_logFile.writeln("\n*************\n"
                                            "** Running test " + to_string(i+1) + "/" + to_string(jsonConfiguration.get_testList().size()) +
                                            " with execution "+ to_string(j+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList().size()) +
                                            " and repetition "+ to_string(k+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions()) + "\n" +
                                            "*", true);
                
                executionResults.clear();
                result = runExecution(jsonConfiguration.get_testList()[i].get_executionList()[j], executionResults, fileWriter_logFile, fileWriter_statsFile);
                if(!result){
                    numFailures++;
                }
                
                for (int execResult=0 ; execResult<executionResults.size() ; execResult++){
                    if((execResult) >= testAverageResults.size()){
                        testAverageResults.push_back(executionResults[execResult]);
                    }else{
                        testAverageResults[execResult] = ( (testAverageResults[execResult]+ executionResults[execResult]) );
                    }
                }
                fileWriter_logFile.writeln("\n*\n**\n*************\n", true);
            }


            for (int execResult=0 ; execResult<executionResults.size() ; execResult++){
                testAverageResults[execResult] = ( testAverageResults[execResult] / jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions() );
            }
            tempString_toWrite=jsonConfiguration.get_testList()[i].get_executionList()[j].get_dataSize()+",";
            
            for (int execResult=0 ; execResult<executionResults.size() ; execResult++){
                if(execResult==executionResults.size()-1){
                    tempString_toWrite += to_string(testAverageResults[execResult]);
                }else{
                    tempString_toWrite += to_string(testAverageResults[execResult]) + ",";
                }
            }
            fileWriter_logFile.writeln("Average results:\nX,CPU time execution,CPU time execution optimizated,Device time execution,Transmision (S+R) time,Send to device time, Recieve from device time", true);
            fileWriter_logFile.writeln(tempString_toWrite, true);  
            fileWriter_statsFile.writeln(tempString_toWrite, false);
        }

        if(jsonConfiguration.get_testList()[i].get_generate_performanceGraphics()){
            result = externProgramConnection.execute_generatePyctures(
                jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile());
            
            if(!result){
                fileWriter_logFile.writeln("ERROR..: Couldn't generate the image of " + jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile() );
            }
        }

    }

    printFinalMessage(numFailures, fileWriter_logFile);

    return 0;
}
//*************************************
// MAIN FUNCTION - END
//*************************************


bool runExecution(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){

    bool result = false;
    if(exec.get_host().find("doitgen") != string::npos){
        result = DoitgenHost::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);

    }else if(exec.get_host().find("cholesky") != string::npos){
        result = CholeskyHost::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);   
    
    }else if(exec.get_host().find("gemm") != string::npos){
        result = GemmHost::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);   
    
    }else if(exec.get_host().find("jacobi2d") != string::npos){
        result = Jacobi2dHost::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);   
    
    }else if(exec.get_host().find("vectorAdd") != string::npos){
        result = VectorAddHost::exec(exec, results, fileWriter_logFile, fileWriter_statsFile);   
    
    }else{
        fileWriter_logFile.writeln("ERROR..: Host name unknow.");
        return false;
    }
    
    return result;
}



void printFinalMessage(unsigned int numFailures, FileWriter_service fileWriter_logFile){

    if(numFailures==0){
        fileWriter_logFile.writeln("\033[1;32m**********************************************\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;32m* ALL EXECUTIONS HAVE FINALIZED SUCCESSFULLY *\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;32m**********************************************\033[0m\n", true);
    }else{
        fileWriter_logFile.writeln("\033[1;31m**********************************************\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;31m  " + to_string(numFailures) + " EXECUTION/S HAVE FAILED\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;31m**********************************************\033[0m\n", true);
    }

}