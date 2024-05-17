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

    FileWriter_service fileWriter_logFile = FileWriter_service(jsonConfiguration.get_outDir() + "/exec_" + DateAndTime::getCurrentDateTime() + "/" + jsonConfiguration.get_logFile(), true);
    fileWriter_logFile.writeln("Contenido del fichero de configuración:");
    fileWriter_logFile.writeln(jsonConfiguration.displayInfo("\t"));

    for(int i=0 ; i<jsonConfiguration.get_testList().size() ; i++){
        for(int j=0 ; j<jsonConfiguration.get_testList()[i].get_executionList().size() ; j++){

            fileWriter_logFile.writeln("*************");
            fileWriter_logFile.writeln("** Running test " + to_string(i+1) + "/" + to_string(jsonConfiguration.get_testList().size()) +
                                        " with execution "+ to_string(j+1) + "/" + to_string(jsonConfiguration.get_testList()[i].get_executionList().size()) );
            fileWriter_logFile.writeln("*");
            
            DoitgenHost::doitgenHost_exec(jsonConfiguration.get_testList()[i].get_executionList()[j], fileWriter_logFile);

            fileWriter_logFile.writeln("*");
            fileWriter_logFile.writeln("**");
            fileWriter_logFile.writeln("*************");
        }
    }

    return 0;
}

