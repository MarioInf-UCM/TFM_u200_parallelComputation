#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <omp.h>
#include "doitgenHost_noOpt.hpp"

using namespace std;
using globalConfiguration_typeData::typeData;

using cl::Event;
using cl::CommandQueue;
using cl::Kernel;
using cl::Buffer;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
DoitgenHost_noOpt::DoitgenHost_noOpt(){}
DoitgenHost_noOpt::~DoitgenHost_noOpt(){}

//*************************************
// MAIN FUNCTION - START
//*************************************
bool DoitgenHost_noOpt::doitgenHost_noOpt_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    fileWriter_logFile.writeln("Executing host function \"DoitgenHost::doitgenHost_noOpt_exec\". Execution configuration:\n" + exec.displayInfo("\t"));

    unsigned int SIZE_R=0, SIZE_Q=0, SIZE_P=0;    
    bool result = initParameter(exec, SIZE_R, SIZE_Q, SIZE_P);
    if(!result){
        cout << "\033[1;31mERROR..:Entry params unexpected. Fanalizating execution.\033[0m\n"  << endl;
        return false;
    }
    DoitgenKernel data = DoitgenKernel(SIZE_R, SIZE_Q, SIZE_P);
    ostringstream stringToPrint;
    EventTimer event;
    Event event_sp;

    //STEP 1 - START: Initializaton OpenCL and load kernels"
    fileWriter_logFile.writeln("STEP 1 - START: Initializaton OpenCL and load kernels");
    event.add("Initializaton OpenCL and load kernels");

    xilinx::example_utils::XilinxOclHelper xocl;
    xocl.initialize(exec.get_kernelPackage());
    CommandQueue q = xocl.get_command_queue();
    Kernel ker = xocl.get_kernel(exec.get_kernel());

    event.finish();
    fileWriter_logFile.write("STEP 1 - END: Initializaton OpenCL and load kernels (" + event.getInfoEvents(0));
    //STEP 1 - END: Initializaton OpenCL and load kernels"


    //STEP 2 - START: Running kernel in CPU"
    fileWriter_logFile.writeln("STEP 2 - START: Running kernel in CPU");
    event.add("Running kernel in CPU");

    data.kernel_doitgen_CPU();

    event.finish();
    fileWriter_logFile.write("STEP 2 - END: Running kernel in CPU (" + event.getInfoEvents(1));
    //STEP 2 - END: Running kernel in CPU"


    //STEP 3 - START: Running kernel in CPU optimizated"
    fileWriter_logFile.writeln("STEP 3 - START: Running kernel in CPU optimizated");
    event.add("Running kernel in CPU optimizated");

    data.kernel_doitgen_CPU_opt();

    event.finish();
    fileWriter_logFile.write("STEP 3 - END: Running kernel in CPU optimizated (" + event.getInfoEvents(2));
    //STEP 3 - END: Running kernel in CPU optimizated"


    //STEP 4 - START: Creating buffer 
    fileWriter_logFile.writeln("STEP 4 - START: Creating buffer");
    event.add("Creating buffers");

    vector<typeData> temp_A = vector<typeData>();
    vector<typeData> temp_C4 = vector<typeData>();
    vector<typeData> temp_resultDevice = vector<typeData>();
    emsamble_dataToBuffers(data, temp_A, temp_C4, temp_resultDevice);

    Buffer sendBuff_A(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_A.size() * sizeof(typeData),
                        temp_A.data(),
                        NULL);

    Buffer sendBuff_C4(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_C4.size() * sizeof(typeData),
                        temp_C4.data(),
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_resultDevice.size() * sizeof(typeData),
                        temp_resultDevice.data(),
                        NULL);

    ker.setArg(0, sendBuff_A);
    ker.setArg(1, sendBuff_C4);
    ker.setArg(2, recvBuff_resultDevice);

    event.finish();
    fileWriter_logFile.write("STEP 4 - END: Creating buffer (" + event.getInfoEvents(3)); 
    //STEP 4 - END: Creating buffer 


    //STEP 5 - START: Transmision data to device
    fileWriter_logFile.writeln("STEP 5 - START: Transmision data to device");
    event.add("Transmision data to device");

    q.enqueueMigrateMemObjects({sendBuff_A, sendBuff_C4}, 0, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    event.finish();
    fileWriter_logFile.write("5 - END: Transmision data to device (" + event.getInfoEvents(4));
    //STEP 5 - END: Transmision data to device 


    //STEP 6 - START: Device execution
    fileWriter_logFile.writeln("STEP 6 - START: Device execution");
    event.add("Device execution");

    q.enqueueTask(ker, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    event.finish();
    fileWriter_logFile.write("STEP 6 - END: Device execution (" + event.getInfoEvents(5));
    //STEP 6 - END: Device execution


    //STEP 7 - START: Transmision data from device
    fileWriter_logFile.writeln("STEP 7 - START: Transmision data from device");
    event.add("Transmision data from device ");

    q.enqueueMigrateMemObjects({recvBuff_resultDevice}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);
    q.finish();

    emsamble_buffersToData(data, temp_resultDevice);
    event.finish();
    fileWriter_logFile.write("STEP 7 - END: Transmision data from device (" + event.getInfoEvents(6));
    //STEP 7 - END: Transmision data from device 


    if(exec.get_printResults()){
        fileWriter_logFile.write(data.printAll());
    }
    if(compareResults(data)){
        fileWriter_logFile.write("\033[1;32m***WELL, The results match***\033[0m\n");
        result = true;
    }else{
        fileWriter_logFile.write("\033[1;31m***BAD, The results don't match***\033[0m\n");
        result = false;
    }
    fileWriter_logFile.writeln("--------------- Key execution times ---------------");
    fileWriter_logFile.write(event.getInfoEvents());
    fileWriter_logFile.writeln("---------------------------------------------------");
    results.push_back(data.get_SIZE_P() * data.get_SIZE_Q() * data.get_SIZE_R()); 
    results.push_back(stod(event.getTimeEvents(1)));    //CPU execution time
    results.push_back(stod(event.getTimeEvents(2)));    //CPU execution time optimizated
    results.push_back(stod(event.getTimeEvents(5)));    //Device execution time 
    results.push_back(stod(event.getTimeEvents(4)));    //Send data to device
    results.push_back(stod(event.getTimeEvents(6)));    //Recieve data from device
    
  return result;
}
//*************************************
// MAIN FUNCTION - END
//*************************************



bool DoitgenHost_noOpt::initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P){
    if(exec.get_dataSize() == "mini"){
        SIZE_R=DOITGEN_R_MINI;
        SIZE_Q=DOITGEN_Q_MINI;
        SIZE_P=DOITGEN_P_MINI;
    }else if(exec.get_dataSize() == "small"){
        SIZE_R=DOITGEN_R_SMALL;
        SIZE_Q=DOITGEN_Q_SMALL;
        SIZE_P=DOITGEN_P_SMALL;
    }else if(exec.get_dataSize() == "medium"){
        SIZE_R=DOITGEN_R_MEDIUM;
        SIZE_Q=DOITGEN_Q_MEDIUM;
        SIZE_P=DOITGEN_P_MEDIUM;
    }else if(exec.get_dataSize() == "large"){
        SIZE_R=DOITGEN_R_LARGE;
        SIZE_Q=DOITGEN_Q_LARGE;
        SIZE_P=DOITGEN_P_LARGE;
    }else if(exec.get_dataSize() == "extralarge"){
        SIZE_R=DOITGEN_R_EXTRALARGE;
        SIZE_Q=DOITGEN_Q_EXTRALARGE;
        SIZE_P=DOITGEN_P_EXTRALARGE;
    }else{
        return false;
    }  
    return true;
}



bool DoitgenHost_noOpt::compareResults(DoitgenKernel& data){
    for (int r = 0; r < data.get_SIZE_R(); r++){
      for (int q = 0; q < data.get_SIZE_Q(); q++){
        for (int p = 0; p < data.get_SIZE_P(); p++){
          if(data.get_resultCPU()[r][q][p] != data.get_resultDevice()[r][q][p]){
              return false;
          }
        }
      }
    }
    return true;
}



void DoitgenHost_noOpt::emsamble_dataToBuffers(DoitgenKernel& data, vector<typeData> &temp_A,  vector<typeData>& temp_C4, vector<typeData>& temp_resultDevice){

    temp_A.clear();
    temp_resultDevice.clear();
    for (int r = 0; r < data.get_SIZE_R(); r++) {
        for (int q = 0; q < data.get_SIZE_Q(); q++) {
            for (int p = 0; p < data.get_SIZE_P(); p++){
                temp_A.push_back( data.get_A()[r][q][p] );
                temp_resultDevice.push_back( 0.0 );
            }
        }
    }

    temp_C4.clear();
    for (int p1 = 0; p1 < data.get_SIZE_P(); p1++) {
        for (int p2 = 0; p2 < data.get_SIZE_P(); p2++) {
            temp_C4.push_back( data.get_C4()[p1][p2] );
        }
    }

  return;
}


void DoitgenHost_noOpt::emsamble_buffersToData(DoitgenKernel& data, vector<typeData>& temp_resultDevice){
    
    int i=0;
    for (int r = 0; r < data.get_SIZE_R(); r++) {
        for (int q = 0; q < data.get_SIZE_Q(); q++) {
            for (int p = 0; p < data.get_SIZE_P(); p++){
                data.get_resultDevice()[r][q][p] = temp_resultDevice[i];
                i++;
            }
        }
    }
    return;
}