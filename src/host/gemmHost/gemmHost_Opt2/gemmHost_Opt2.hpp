#ifndef _GEMMHOST_OPT2_HPP_
#define _GEMMHOST_OPT2_HPP_

#include <iostream>
#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"
#include "../../../kernel/gemmKernel/gemmKernel.hpp"
#include "../../../configParams/configParams.hpp"

using globalConfiguration_typeData::typeData;

class GemmHost_Opt2 {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        GemmHost_Opt2();
        ~GemmHost_Opt2();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
         
        static bool initParameter(Execution exec, unsigned int &SIZE_I, unsigned int &SIZE_J, unsigned int &SIZE_K);
        static void emsamble_dataToBuffers(GemmKernel& data, vector<typeData> &temp_A, vector<typeData>& temp_B, vector<typeData>& temp_C, vector<typeData>& temp_resultDevice);
        static void emsamble_buffersToData(GemmKernel& data, vector<typeData>& temp_resultDevice);
        static bool compareResults(GemmKernel& data);
};
#endif