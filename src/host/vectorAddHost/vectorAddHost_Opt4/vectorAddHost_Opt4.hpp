#ifndef _VECTORADDHOST_OPT4_HPP_
#define _VECTORADDHOST_OPT4_HPP_

#include <iostream>
#include <atomic>

#include "../../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../../utilities/event_timer/event_timer.hpp"
#include "../../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"
#include "../../../configParams/configParams.hpp"
#include "../../../kernel/vectorAddKernel/vectorAddKernel.hpp"

using globalConfiguration_typeData::typeData;
using globalConfiguration_typeData::typeData_fixed;

class VectorAddHost_Opt4 {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        VectorAddHost_Opt4();
        ~VectorAddHost_Opt4();

        static bool exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE);

        static float searchPropertyValue(const string& texto, const string& subcadena);
        static void threadFunction_CPUPowerMeasure(int *PAPIevent_PowerMeasure, atomic<double> &CPU_powerMeasure);
        static void threadFunction_DevicePowerMeasure();

        static void ensamble_dataToBuffers(VectorAddKernel& data, typeData_fixed *temp_A, typeData_fixed *temp_B, typeData_fixed *temp_resultDevice);
        static void ensamble_buffersToData(VectorAddKernel& data, typeData_fixed *temp_resultDevice);
        static bool compareResults(VectorAddKernel& data);

};
#endif