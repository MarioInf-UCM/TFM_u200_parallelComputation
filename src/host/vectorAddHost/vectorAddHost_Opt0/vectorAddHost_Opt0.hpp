#ifndef _VECTORADDHOST_OPT0_HPP_
#define _VECTORADDHOST_OPT0_HPP_

#include <iostream>

#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"
#include "../../../configParams/configParams.hpp"
#include "../../../kernel/vectorAddKernel/vectorAddKernel.hpp"

using globalConfiguration_typeData::typeData;

class VectorAddHost_Opt0 {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        VectorAddHost_Opt0();
        ~VectorAddHost_Opt0();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE);
        static void ensamble_dataToBuffers(VectorAddKernel& data, vector<typeData> &temp_A, vector<typeData> &temp_B, vector<typeData>& temp_resultDevice);
        static void ensamble_buffersToData(VectorAddKernel& data, vector<typeData>& temp_resultDevice);
        static bool compareResults(VectorAddKernel& data);
 
};
#endif