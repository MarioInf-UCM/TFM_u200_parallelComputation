#ifndef DOITGENHOST_HPP
#define DOITGENHOST_HPP

#include <iostream>
#include <vector>
#include "../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../service/fileWriter_service/fileWriter_service.hpp"
#include "doitgenHost_noOpt/doitgenHost_noOpt.hpp"


class DoitgenHost {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:

        //********************************
        //* CONSTRUCTORS AND DESTRUCTORS *
        //********************************
        DoitgenHost();
        ~DoitgenHost();

        static bool doitgenHost_exec(Execution exec, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:

};
#endif