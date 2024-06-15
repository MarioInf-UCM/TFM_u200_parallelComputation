#ifndef _JACOBI2DHOST_OPT2_HPP_
#define _JACOBI2DHOST_OPT2_HPP_

#include <iostream>

#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"
#include "../../../configParams/configParams.hpp"
#include "../../../kernel/jacobi_2dKernel/jacobi_2dKernel.hpp"

using globalConfiguration_typeData::typeData;

class Jacobi2dHost_Opt2 {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        Jacobi2dHost_Opt2();
        ~Jacobi2dHost_Opt2();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &STEPS, unsigned int &SIZE_N);
        static void ensamble_dataToBuffers(Jacobi_2dKernel& data, vector<typeData> &temp_A, vector<typeData> &temp_B, vector<typeData>& temp_resultDevice);
        static void ensamble_buffersToData(Jacobi_2dKernel& data, vector<typeData>& temp_resultDevice);
        static bool compareResults(Jacobi_2dKernel& data);
 
};
#endif