#include <iostream>
#include <unistd.h>
#include "config/config.hpp"
#include "doitgenHost/doitgenHost.hpp"
#include "service/json_service/json_service.hpp"
#include "service/json_service/jsonConfiguration/jsonConfiguration.hpp"

using namespace std;


//*************************************
// MAIN FUNCTION - START
//*************************************
int main(int argc, char** argv){

    Json_service json_Interface = Json_service(argv[1]);
    JsonConfiguration jsonConfiguration = json_Interface.getJSONConfiguration_FromFile();
    if(jsonConfiguration.get_status() == false){
        cerr << "Closing Application..." << endl;
        return 0;
    }

    cout << jsonConfiguration.displayInfo() << endl;

    //DoitgenHost::doitgenHost_noOpt_exec();



}

