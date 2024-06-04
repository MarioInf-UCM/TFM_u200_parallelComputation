#ifndef _DOITGENHOST_OPT_HPP_
#define _DOITGENHOST_OPT_HPP_

#include <iostream>
#include "../../../configParams/configParams.hpp"
#include "../../../kernel/doitgenKernel/doitgenKernel.hpp"
#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"

using globalConfiguration_typeData::typeData;
using globalConfiguration_typeData::typeData_fixed;


class DoitgenHost_Opt {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        DoitgenHost_Opt();
        ~DoitgenHost_Opt();

        static bool doitgenHost_Opt_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P);
        static void emsamble_dataToBuffers(DoitgenKernel& data, vector<typeData_fixed> &temp_A,  vector<typeData_fixed> &temp_C4, vector<typeData_fixed> &temp_resultDevice);
        static void emsamble_buffersToData(DoitgenKernel& data, vector<typeData_fixed> &temp_resultDevice);
        static bool compareResults(DoitgenKernel& data);

};
#endif