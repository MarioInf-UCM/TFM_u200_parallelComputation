#ifndef _JACOBI2DHOST_HPP_
#define _JACOBI2DHOST_HPP_

#include <iostream>
#include <vector>
#include "../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../utilities/helper/helper.hpp"
#include "jacobi2dHost_Opt0_1/jacobi2dHost_Opt0_1.hpp"
#include "jacobi2dHost_Opt2/jacobi2dHost_Opt2.hpp"
#include "jacobi2dHost_Opt2/jacobi2dHost_Opt2.hpp"

class Jacobi2dHost {


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
        Jacobi2dHost();
        ~Jacobi2dHost();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:

};
#endif