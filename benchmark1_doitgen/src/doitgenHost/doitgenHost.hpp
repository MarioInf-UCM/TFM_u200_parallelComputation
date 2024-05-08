#ifndef DOITGENHOST_HPP
#define DOITGENHOST_HPP

#include <iostream>
#include <vector>
#include "../config/config.hpp"
#include "../doitgenData/doitgenData.hpp"
#include "../utilities/event_timer/event_timer.hpp"
#include "../utilities/xilinx_ocl_helper/xilinx_ocl_helper.hpp"


class DoitgenHost {


    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:

        //********************************
        //* CONSTRUCTORS AND DESTRUCTORS *
        //********************************
        DoitgenHost();
        ~DoitgenHost();

        static int doitgenHost_exec();


    private:

        static void printArrays(DoitgenData data);
        static void kernel_doitgen_CPU(DoitgenData *data);
        static bool compareResults(DoitgenData data);

};
#endif