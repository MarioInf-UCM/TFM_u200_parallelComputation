#ifndef _CHOLESKYHOST_NOOPT_HPP_
#define _CHOLESKYHOST_NOOPT_HPP_

#include <iostream>

#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"
#include "../../../configParams/configParams.hpp"
#include "../../../kernel/choleskyKernel/choleskyKernel.hpp"

using globalConfiguration_typeData::typeData;

class CholeskyHost_noOpt {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        CholeskyHost_noOpt();
        ~CholeskyHost_noOpt();

        static bool choleskyHost_noOpt_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE_N);
        static void emsamble_dataToBuffers(CholeskyKernel& data, vector<typeData>& temp_A, vector<typeData>& temp_resultDevice);
        static void emsamble_buffersToData(CholeskyKernel& data, vector<typeData>& temp_resultDevice);
        static bool compareResults(CholeskyKernel& data);
 
};
#endif