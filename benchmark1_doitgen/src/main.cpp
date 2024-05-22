#include <iostream>
#include <unistd.h>
#include "config/config.hpp"
#include "doitgenHost/doitgenHost.hpp"
#include "utilities/dateAndTime/dateAndTime.hpp"
#include "service/json_service/json_service.hpp"
#include "service/json_service/jsonConfiguration/jsonConfiguration.hpp"

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
    FileWriter_service fileWriter_logFile = FileWriter_service(jsonConfiguration.get_outDir() + "/exec_" + DateAndTime::getCurrentDateTime() + "/" + jsonConfiguration.get_logFile(),
                                                                jsonConfiguration.get_verbose());
    fileWriter_logFile.writeln("Contenido del fichero de configuración:");
    fileWriter_logFile.writeln(jsonConfiguration.displayInfo("\t"));

    for(int i=0 ; i<jsonConfiguration.get_testList().size() ; i++){
        for(int j=0 ; j<jsonConfiguration.get_testList()[i].get_executionList().size() ; j++){
            for(int k=0 ; k<jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions() ; k++){

                fileWriter_logFile.writeln("\n*************", true);
                fileWriter_logFile.writeln("** Running test " + to_string(i+1) + "/" + to_string(jsonConfiguration.get_testList().size()) +
                                            " with execution "+ to_string(j+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList().size()) +
                                            " and repetition "+ to_string(k+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList()[j].get_numExecutions()), true);
                fileWriter_logFile.writeln("*", true);
                
                result = DoitgenHost::doitgenHost_exec(jsonConfiguration.get_testList()[i].get_executionList()[j], fileWriter_logFile);
                if(!result){
                    numFailures++;
                }

                fileWriter_logFile.writeln("*", true);
                fileWriter_logFile.writeln("**", true);
                fileWriter_logFile.writeln("*************\n", true);
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

