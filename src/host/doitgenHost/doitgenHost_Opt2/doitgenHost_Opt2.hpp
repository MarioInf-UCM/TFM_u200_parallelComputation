#ifndef _DOITGENHOST_OPT2_HPP_
#define _DOITGENHOST_OPT2_HPP_

#include <iostream>
#include "../../../configParams/configParams.hpp"
#include "../../../kernel/doitgenKernel/doitgenKernel.hpp"
#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"

using globalConfiguration_typeData::typeData;
using globalConfiguration_typeData::typeData_fixed;


class DoitgenHost_Opt2{


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:


    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        DoitgenHost_Opt2();
        ~DoitgenHost_Opt2();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P);
        static void emsamble_dataToBuffers(DoitgenKernel& data, typeData_fixed *temp_A,  typeData_fixed *temp_C4, typeData_fixed *temp_resultDevice);
        static void emsamble_buffersToData(DoitgenKernel& data, typeData_fixed *temp_resultDevice);
        static bool compareResults(DoitgenKernel& data);
        static bool checkValue(typeData_fixed a, typeData_fixed b, typeData_fixed tolerance);

};
#endif