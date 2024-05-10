#include <iostream>
#include <unistd.h>
#include "config/config.hpp"
#include "doitgenHost/doitgenHost.hpp"

using namespace std;


//*************************************
// MAIN FUNCTION - START
//*************************************
int main(int argc, char** argv){


    #if defined(NO_OPTIMIZATION)
        DoitgenHost::doitgenHost_noOptimization_exec();
    #endif



}

