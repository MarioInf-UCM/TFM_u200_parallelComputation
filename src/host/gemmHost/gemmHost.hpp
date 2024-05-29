#ifndef _GEMMHOST_HPP_
#define _GEMMHOST_HPP_

#include <iostream>
#include <vector>
#include "../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../service/fileWriter_service/fileWriter_service.hpp"
#include "gemmHost_noOpt/gemmHost_noOpt.hpp"


class GemmHost {


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
        GemmHost();
        ~GemmHost();

        static bool gemmHost_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:

};
#endif