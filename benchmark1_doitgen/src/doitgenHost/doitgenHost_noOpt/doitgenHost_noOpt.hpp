#ifndef _DOITGENHOST_NOOPT_HPP_
#define _DOITGENHOST_NOOPT_HPP_

#include <iostream>
#include "../../config/config.hpp"
#include "../../doitgenData/doitgenData.hpp"
#include "../../service/fileWriter_service/fileWriter_service.hpp"
#include "../../service/json_service/jsonConfiguration/execution/execution.hpp"
#include "../../utilities/event_timer/event_timer.hpp"
#include "../../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"


class DoitgenHost_noOpt {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        DoitgenHost_noOpt();
        ~DoitgenHost_noOpt();

        static bool doitgenHost_noOpt_exec(Execution exec, FileWriter_service fileWriter_logFile);


    private:
        static bool initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P);
        static void kernel_doitgen_CPU(DoitgenData& data);
        static bool compareResults(DoitgenData& data);
        static void printKeyResults(EventTimer event);
        static string printArrays(DoitgenData& data);
        static void emsamble_dataToBuffers(DoitgenData& data, vector<typeData>& temp_A,  vector<typeData>& temp_C4, vector<typeData>& temp_resultDevice);
        static void emsamble_buffersToData(DoitgenData& data, vector<typeData>& temp_resultDevice);

};
#endif