#ifndef _DOITGENHOST_MAPOPT_HPP_
#define _DOITGENHOST_MAPOPT_HPP_

#include <iostream>
#include "../../config/config.hpp"
#include "../../doitgenData/doitgenData.hpp"
#include "../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../utilities/event_timer/event_timer.hpp"
#include "../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"


class DoitgenHost_mapOpt {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        DoitgenHost_mapOpt();
        ~DoitgenHost_mapOpt();

        static bool doitgenHost_mapOpt_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P);
        static void kernel_doitgen_CPU(DoitgenData& data);
        static bool compareResults(DoitgenData& data);
        static string getKeyResults(EventTimer event);
        static string printArrays(DoitgenData& data);
        static void emsamble_dataToBuffers(DoitgenData& data, typeData *temp_A,  typeData *temp_C4, typeData *temp_resultDevice);
        static void emsamble_buffersToData(DoitgenData& data, typeData *temp_resultDevice);

};
#endif