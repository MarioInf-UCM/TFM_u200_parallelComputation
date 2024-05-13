#ifndef __JSON_INTERFACE_H_
#define __JSON_INTERFACE_H_

#include <string>
//#include <jsoncpp/json/json.h>
#include "jsonConfiguration/jsonConfiguration.hpp"


using namespace std;

class Json_interface {

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:
        string fileURL;



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:

        //********************************
        //* CONSTRUCTORS AND DESTRUCTORS *
        //********************************
        Json_interface();
        Json_interface(string jsonFileURL);
        ~Json_interface();
        

        //*********************
        //* GENERAL FUNCTIONS *
        //*********************
        JsonConfiguration getJSONConfiguration_FromFile();



        //*************************
        //* GET AND SET FUNCTIONS *
        //*************************
        string getFileURL();
        void setFileURL(string data);
    
    private:


};

#endif