#ifndef _DOITGENHOST_OPT0_1_HPP_
#define _DOITGENHOST_OPT0_1_HPP_

#include <iostream>
#include "../../../kernel/doitgenKernel/doitgenKernel.hpp"
#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"
#include "../../../configParams/configParams.hpp"

using globalConfiguration_typeData::typeData;


class DoitgenHost_Opt0_1 {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        DoitgenHost_Opt0_1();
        ~DoitgenHost_Opt0_1();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P);
        static void emsamble_dataToBuffers(DoitgenKernel& data, vector<typeData>& temp_A,  vector<typeData>& temp_C4, vector<typeData>& temp_resultDevice);
        static void emsamble_buffersToData(DoitgenKernel& data, vector<typeData>& temp_resultDevice);
        static bool compareResults(DoitgenKernel& data);

};
#endif