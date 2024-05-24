#include <iostream>
#include <unistd.h>
#include "config/config.hpp"
#include "doitgenHost/doitgenHost.hpp"
#include "utilities/dateAndTime/dateAndTime.hpp"
#include "service/json_service/json_service.hpp"
#include "service/json_service/jsonConfiguration/jsonConfiguration.hpp"
#include "service/externProgramsConnection_service/externProgramsConnection_service.hpp"

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

    fileWriter_logFile = FileWriter_service(jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_logFile(), jsonConfiguration.get_verbose());
    fileWriter_logFile.writeln("Contenido del fichero de configuración:");
    fileWriter_logFile.writeln(jsonConfiguration.displayInfo("\t"));

    for(int i=0 ; i<jsonConfiguration.get_testList().size() ; i++){
        fileWriter_statsFile = FileWriter_service(jsonConfiguration.get_outDir() + outForderID + "/" + jsonConfiguration.get_testList()[i].get_statsFile(), jsonConfiguration.get_verbose());
        fileWriter_statsFile.writeln("x,CPU time execution,Device time execution,Send buffers to device time,Receive buffers from device time", false);
    
        for(int j=0 ; j<jsonConfiguration.get_testList()[i].get_executionList().size() ; j++){
            for(int k=0 ; k<jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions() ; k++){
                fileWriter_logFile.writeln("\n*************\n"
                                            "** Running test " + to_string(i+1) + "/" + to_string(jsonConfiguration.get_testList().size()) +
                                            " with execution "+ to_string(j+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList().size()) +
                                            " and repetition "+ to_string(k+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions()) + "\n" +
                                            "*", true);
                
                result = DoitgenHost::doitgenHost_exec(jsonConfiguration.get_testList()[i].get_executionList()[j], fileWriter_logFile, fileWriter_statsFile);
                if(!result){
                    numFailures++;
                }
                fileWriter_logFile.writeln("*\n**\n*************\n", true);
            }
        }

        if(jsonConfiguration.get_testList()[i].get_generatePictures()){
            result = externProgramConnection.execute_generatePyctures(jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_testList()[i].get_statsFile(), 
                                                                jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_testList()[i].get_statsFile());
            if(!result){
                fileWriter_logFile.writeln("ERROR..: Couldn't generate the image of " + jsonConfiguration.get_outDir()+ outForderID + "/" + jsonConfiguration.get_testList()[i].get_statsFile() );
            }
        }

    }



    if(numFailures==0){
        fileWriter_logFile.writeln("\033[1;32m**********************************************\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;32m* ALL EXECUTIONS HAVE FINALIZED SUCCESSFULLY *\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;32m**********************************************\033[0m\n", true);
    }else{
        fileWriter_logFile.writeln("\033[1;31m**********************************************\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;31m  " + to_string(numFailures) + "HAVE FAILED\033[0m\n", true);
        fileWriter_logFile.writeln("\033[1;31m**********************************************\033[0m\n", true);
    }

    return 0;
}

