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

bool runExecution(Execution exec, vector<double>& resultsPerformance, vector<double>& resultsPower, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_performanceFile);
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
    string outFolderID = "/exec_" +DateAndTime::getCurrentDateTime();
    FileWriter_service fileWriter_performanceFile;
    FileWriter_service fileWriter_powerFile;
    FileWriter_service fileWriter_logFile;
    ExternProgramsConnection_service externConnec_performanceGraphics = ExternProgramsConnection_service("generateGaphics_performance.py");
    ExternProgramsConnection_service externConnec_powerGraphics = ExternProgramsConnection_service("generateGaphics_power.py");
    ExternProgramsConnection_service externConnec_performanceTable = ExternProgramsConnection_service("generateTable_performance.py");
    ExternProgramsConnection_service externConnec_powerTable = ExternProgramsConnection_service("generateTable_power.py");
    vector<double> resultsPerformance = vector<double>();
    vector<double> resultsPower = vector<double>();
    vector<double> resultsPerformance_average = vector<double>();
    vector<double> resultsPower_average = vector<double>();
    string tempString_toWrite="";

    fileWriter_logFile = FileWriter_service(jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_logFile(), jsonConfiguration.get_verbose());
    fileWriter_logFile.writeln("Contenido del fichero de configuración:");
    fileWriter_logFile.writeln(jsonConfiguration.displayInfo("\t"));

    for(int i=0 ; i<jsonConfiguration.get_testList().size() ; i++){
        fileWriter_performanceFile = FileWriter_service(jsonConfiguration.get_outDir() + outFolderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile(), jsonConfiguration.get_verbose());
        fileWriter_performanceFile.writeln("X,CPU execution time,CPU optimized execution time,Device execution time,Transmision (S+R) time,Send to device time,Recieve from device time", true);
    
        fileWriter_powerFile = FileWriter_service(jsonConfiguration.get_outDir() + outFolderID + "/" + jsonConfiguration.get_testList()[i].get_powerFile(), jsonConfiguration.get_verbose());
        fileWriter_powerFile.writeln("X,Device energy consumption,Device execution time,CPU energy consumption,CPU execution time,CPU optimized energy consumption,CPU optimized execution time,CPU-1,CPU-2", true);
    

        for(int j=0 ; j<jsonConfiguration.get_testList()[i].get_executionList().size() ; j++){
            resultsPerformance_average.clear();
            resultsPower_average.clear();


            //Executing cool executions
            for(int k=0 ; k<jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions_cool() ; k++){
                fileWriter_logFile.writeln("\033[1;34m\n*************\n"
                                            "** Running test " + to_string(i+1) + "/" + to_string(jsonConfiguration.get_testList().size()) +
                                            " with execution "+ to_string(j+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList().size()) +
                                            " and cool repetition "+ to_string(k+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions_cool()) + "\n" +
                                            "*\033[0m", true);
                resultsPerformance.clear();   
                result = runExecution(jsonConfiguration.get_testList()[i].get_executionList()[j], resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);
                fileWriter_logFile.writeln("\033[1;34m\n*\n**\n*************\n\033[0m", true);
            }


            //Executing hot executions            
            for(int k=0 ; k<jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions_hot() ; k++){
                fileWriter_logFile.writeln("\033[1;34m\n*************\n"
                                            "** Running test " + to_string(i+1) + "/" + to_string(jsonConfiguration.get_testList().size()) +
                                            " with execution "+ to_string(j+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList().size()) +
                                            " and hot repetition "+ to_string(k+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions_hot()) + "\n" +
                                            "*\033[0m", true);
                
                resultsPerformance.clear();
                resultsPower.clear();
                result = runExecution(jsonConfiguration.get_testList()[i].get_executionList()[j], resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);
                if(!result){
                    numFailures++;
                }
                
                for (int execResult=0 ; execResult<resultsPerformance.size() ; execResult++){
                    if((execResult) >= resultsPerformance_average.size()){
                        resultsPerformance_average.push_back(resultsPerformance[execResult]);
                    }else{
                        resultsPerformance_average[execResult] = ( (resultsPerformance_average[execResult]+ resultsPerformance[execResult]) );
                    }
                }

                for (int execResult=0 ; execResult<resultsPower.size() ; execResult++){
                    if((execResult) >= resultsPower_average.size()){
                        resultsPower_average.push_back(resultsPower[execResult]);
                    }else{
                        resultsPower_average[execResult] = ( (resultsPower_average[execResult]+ resultsPower[execResult]) );
                    }
                }
                fileWriter_logFile.writeln("\033[1;34m\n*\n**\n*************\n\033[0m", true);
            }


            //Calculating average results 
            for (int execResult=0 ; execResult<resultsPerformance.size() ; execResult++){
                resultsPerformance_average[execResult] = ( resultsPerformance_average[execResult] / jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions_hot() );
            }
            tempString_toWrite=jsonConfiguration.get_testList()[i].get_executionList()[j].get_dataSize()+",";
            for (int execResult=0 ; execResult<resultsPerformance.size() ; execResult++){
                if(execResult==resultsPerformance.size()-1){
                    tempString_toWrite += to_string(resultsPerformance_average[execResult]);
                }else{
                    tempString_toWrite += to_string(resultsPerformance_average[execResult]) + ",";
                }
            }
            fileWriter_logFile.writeln("\nAverage performance results:\nX,CPU execution time,CPU optimized execution time,Device execution time,Transmision (S+R) time,Send to device time, Recieve from device time", true);
            fileWriter_logFile.writeln(tempString_toWrite, true);  
            fileWriter_performanceFile.writeln(tempString_toWrite, false);


            for (int execResult=0 ; execResult<resultsPower.size() ; execResult++){
                resultsPower_average[execResult] = ( resultsPower_average[execResult] / jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions_hot() );
            }
            tempString_toWrite=jsonConfiguration.get_testList()[i].get_executionList()[j].get_dataSize()+",";
            for (int execResult=0 ; execResult<resultsPower.size() ; execResult++){
                if(execResult==resultsPower.size()-1){
                    tempString_toWrite += to_string(resultsPower_average[execResult]);
                }else{
                    tempString_toWrite += to_string(resultsPower_average[execResult]) + ",";
                }
            }
            fileWriter_logFile.writeln("\nAverage energy consumption results:\nX,Device energy consumption,Device execution time,CPU energy consumption,CPU execution time,CPU optimized energy consumption,CPU optimized execution time,CPU-1,CPU-2", true);
            fileWriter_logFile.writeln(tempString_toWrite, true);  
            fileWriter_powerFile.writeln(tempString_toWrite, false);            
        }

        //Generating graphics and tables
        if(jsonConfiguration.get_testList()[i].get_generate_performanceGraphics()){
            result = externConnec_performanceGraphics.executeCommand(
                jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile());
            if(!result){
                fileWriter_logFile.writeln("ERROR..: Couldn't generate the image of " + jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile() );
            }
            result = externConnec_performanceTable.executeCommand(
                jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile());
            if(!result){
                fileWriter_logFile.writeln("ERROR..: Couldn't generate the table of " + jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_performanceFile() );
            }
        }
    
        if(jsonConfiguration.get_testList()[i].get_generate_powerGraphics()){
            result = externConnec_powerGraphics.executeCommand(
                jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_powerFile());
            if(!result){
                fileWriter_logFile.writeln("ERROR..: Couldn't generate the image of " + jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_powerFile() );
            }
            result = externConnec_powerTable.executeCommand(
                jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_powerFile());
            if(!result){
                fileWriter_logFile.writeln("ERROR..: Couldn't generate the image of " + jsonConfiguration.get_outDir()+ outFolderID + "/" + jsonConfiguration.get_testList()[i].get_powerFile() );
            }           
        }

    }

    printFinalMessage(numFailures, fileWriter_logFile);

    return 0;
}
//*************************************
// MAIN FUNCTION - END
//*************************************


bool runExecution(Execution exec, vector<double>& resultsPerformance, vector<double>& resultsPower, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_performanceFile){

    bool result = false;
    if(exec.get_host().find("doitgen") != string::npos){
        //result = DoitgenHost::exec(exec, resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);

    }else if(exec.get_host().find("cholesky") != string::npos){
        //result = CholeskyHost::exec(exec, resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);   
    
    }else if(exec.get_host().find("gemm") != string::npos){
        //result = GemmHost::exec(exec, resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);   
    
    }else if(exec.get_host().find("jacobi2d") != string::npos){
        //result = Jacobi2dHost::exec(exec, resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);   
    
    }else if(exec.get_host().find("vectorAdd") != string::npos){
        result = VectorAddHost::exec(exec, resultsPerformance, resultsPower, fileWriter_logFile, fileWriter_performanceFile);   
    
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