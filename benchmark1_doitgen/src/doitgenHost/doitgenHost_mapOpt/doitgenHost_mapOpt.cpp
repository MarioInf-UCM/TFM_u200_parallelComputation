#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include "doitgenHost_mapOpt.hpp"

using namespace std;
using cl::Event;
using cl::CommandQueue;
using cl::Kernel;
using cl::Buffer;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
DoitgenHost_mapOpt::DoitgenHost_mapOpt(){}
DoitgenHost_mapOpt::~DoitgenHost_mapOpt(){}

//*************************************
// MAIN FUNCTION - START
//*************************************
bool DoitgenHost_mapOpt::doitgenHost_mapOpt_exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    fileWriter_logFile.writeln("Executing host function \"DoitgenHost::DoitgenHost_mapOpt_exec\". Execution configuration:\n" + exec.displayInfo("\t"));

    unsigned int SIZE_R=0, SIZE_Q=0, SIZE_P=0;    
    bool result = initParameter(exec, SIZE_R, SIZE_Q, SIZE_P);
    if(!result){
        cout << "\033[1;31mERROR..:Entry params unexpected. Fanalizating execution.\033[0m\n"  << endl;
        return false;
    }
    DoitgenData data = DoitgenData(SIZE_R, SIZE_Q, SIZE_P);
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

    kernel_doitgen_CPU(data);

    event.finish();
    fileWriter_logFile.write("STEP 2 - END: Running kernel in CPU (" + event.getInfoEvents(1));
    //STEP 2 - END: Running kernel in CPU"


    //STEP 3 - START: Creating and mapping buffer 
    fileWriter_logFile.writeln("STEP 3 - START: Creating and mapping buffer");
    event.add("Creating and mapping buffer");

    int sizeA_Andresult = data.get_SIZE_R() * data.get_SIZE_P() * data.get_SIZE_Q() * sizeof(typeData);
    int sizeC4 = data.get_SIZE_P() * data.get_SIZE_P() * sizeof(typeData);

    Buffer sendBuff_A(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR),
                        sizeA_Andresult,
                        NULL,
                        NULL);

    Buffer sendBuff_C4(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR),
                        sizeC4,
                        NULL,
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR),
                        sizeA_Andresult,
                        NULL,
                        NULL);

    ker.setArg(0, sendBuff_A);
    ker.setArg(1, sendBuff_C4);
    ker.setArg(2, recvBuff_resultDevice);

    typeData *temp_A = (typeData *)q.enqueueMapBuffer(sendBuff_A, CL_TRUE, CL_MAP_WRITE, 0, sizeA_Andresult);
    typeData *temp_C4 = (typeData *)q.enqueueMapBuffer(sendBuff_C4, CL_TRUE, CL_MAP_WRITE, 0, sizeC4);
    typeData *temp_resultDevice = (typeData *)q.enqueueMapBuffer(recvBuff_resultDevice, CL_TRUE, CL_MAP_WRITE | CL_MAP_READ, 0, sizeA_Andresult);
    emsamble_dataToBuffers(data, temp_A, temp_C4, temp_resultDevice);

    event.finish();
    fileWriter_logFile.write("STEP 3 - END: Creating and mapping buffer (" + event.getInfoEvents(2)); 
    //STEP 3 - END: Creating and mapping buffer 


    //STEP 4 - START: Transmision data to device
    fileWriter_logFile.writeln("STEP 4 - START: Transmision data to device");
    event.add("Transmision data to device");

    q.enqueueMigrateMemObjects({sendBuff_A, sendBuff_C4}, 0, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    event.finish();
    fileWriter_logFile.write("4 - END: Transmision data to device (" + event.getInfoEvents(3));
    //STEP 4 - END: Transmision data to device 


    //STEP 5 - START: Device execution
    fileWriter_logFile.writeln("STEP 5 - START: Device execution");
    event.add("Device execution");

    q.enqueueTask(ker, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    event.finish();
    fileWriter_logFile.write("STEP 5 - END: Device execution (" + event.getInfoEvents(4));
    //STEP 5 - END: Device execution


    //STEP 6 - START: Transmision data from device
    fileWriter_logFile.writeln("STEP 6 - START: Transmision data from device");
    event.add("Transmision data from device ");

    q.enqueueMigrateMemObjects({recvBuff_resultDevice}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);
    q.finish();

    emsamble_buffersToData(data, temp_resultDevice);
    event.finish();
    fileWriter_logFile.write("STEP 6 - END: Transmision data from device (" + event.getInfoEvents(5));
    //STEP 6 - END: Transmision data from device 


    //STEP 7 - START: Unmap memory object
    fileWriter_logFile.writeln("STEP 7 - START: Unmap memory object");

    q.enqueueUnmapMemObject(sendBuff_A, temp_A);
    q.enqueueUnmapMemObject(sendBuff_C4, temp_C4);
    q.enqueueUnmapMemObject(recvBuff_resultDevice, temp_resultDevice);

    fileWriter_logFile.writeln("STEP 7 - END: Unmap memory object");
    //STEP 7 - END: Unmap memory object


    if(compareResults(data)){
        fileWriter_logFile.write("\033[1;32m***WELL, The results match***\033[0m\n");
        fileWriter_logFile.write(getKeyResults(event));
        results.push_back(data.get_SIZE_P() * data.get_SIZE_Q() * data.get_SIZE_R()); 
        results.push_back(stod(event.getTimeEvents(1))); 
        results.push_back(stod(event.getTimeEvents(4))); 
        results.push_back(stod(event.getTimeEvents(3))); 
        results.push_back(stod(event.getTimeEvents(5)));

        if(exec.get_printResults()){
           fileWriter_logFile.write(printArrays(data));
        }
        return true;
    }else{
        fileWriter_logFile.write("\033[1;31m***BAD, The results don't match***\033[0m\n");
        return false;
    }
    
  return 0;
}
//*************************************
// MAIN FUNCTION - END
//*************************************



bool DoitgenHost_mapOpt::initParameter(Execution exec, unsigned int &SIZE_R, unsigned int &SIZE_Q, unsigned int &SIZE_P){
    if(exec.get_dataSize() == "mini"){
        SIZE_R=SIZE_R_MINI;
        SIZE_Q=SIZE_Q_MINI;
        SIZE_P=SIZE_P_MINI;
    }else if(exec.get_dataSize() == "small"){
        SIZE_R=SIZE_R_SMALL;
        SIZE_Q=SIZE_Q_SMALL;
        SIZE_P=SIZE_P_SMALL;
    }else if(exec.get_dataSize() == "medium"){
        SIZE_R=SIZE_R_MEDIUM;
        SIZE_Q=SIZE_Q_MEDIUM;
        SIZE_P=SIZE_P_MEDIUM;
    }else if(exec.get_dataSize() == "large"){
        SIZE_R=SIZE_R_LARGE;
        SIZE_Q=SIZE_Q_LARGE;
        SIZE_P=SIZE_P_LARGE;
    }else if(exec.get_dataSize() == "extralarge"){
        SIZE_R=SIZE_R_EXTRALARGE;
        SIZE_Q=SIZE_Q_EXTRALARGE;
        SIZE_P=SIZE_P_EXTRALARGE;
    }else{
        return false;
    }  
    return true;
}



void DoitgenHost_mapOpt::kernel_doitgen_CPU(DoitgenData& data){

    vector<typeData> sum = vector<typeData>(data.get_SIZE_P(), 0.0);

    for (int r = 0; r < data.get_SIZE_R(); r++){
      for (int q = 0; q < data.get_SIZE_Q(); q++){
        for (int p = 0; p < data.get_SIZE_P(); p++){
          
          for (int s = 0; s < data.get_SIZE_P(); s++){
            sum[p]=0.0;
            sum[p] += data.get_A()[r][q][s] * data.get_C4()[s][p];
          }
        }

        for (int p = 0; p < data.get_SIZE_P(); p++){
            data.get_resultCPU()[r][q][p] = sum[p];
        }
      }
    }

  return;
}


bool DoitgenHost_mapOpt::compareResults(DoitgenData& data){
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


string DoitgenHost_mapOpt::getKeyResults(EventTimer event){

    string result = "";
    result += "--------------- Key execution times ---------------\n"; 
    result += event.getInfoEvents();
    result += "---------------------------------------------------\n";

  return result;
}


string DoitgenHost_mapOpt::printArrays(DoitgenData& data){
    string result="";
    result += "RESULTS ARRAY A\n";
    result += "=====================\n";
    result += data.printData_A() + "\n";

    result += "RESULTS ARRAY C4\n";
    result += "=====================\n";
    result += data.printData_C4() + "\n";

    result += "RESULTS FROM CPU\n";
    result += "=====================\n";
    result += data.printData_resultCPU() + "\n";

    result += "RESULTS FROM DEVICE\n";
    result += "=====================\n";
    result += data.printData_resultDevice() + "\n";

  return result;
}


void DoitgenHost_mapOpt::emsamble_dataToBuffers(DoitgenData& data, typeData *temp_A,  typeData *temp_C4, typeData *temp_resultDevice){
    int pos=0;
    for (int r = 0; r < data.get_SIZE_R(); r++) {
        for (int q = 0; q < data.get_SIZE_Q(); q++) {
            for (int p = 0; p < data.get_SIZE_P(); p++){
                temp_A[pos] = data.get_A()[r][q][p];
                temp_resultDevice[pos]=0.0;
                pos++;
            }
        }
    }

    pos=0;
    for (int p1 = 0; p1 < data.get_SIZE_P(); p1++) {
        for (int p2 = 0; p2 < data.get_SIZE_P(); p2++) {
            temp_C4[pos] = data.get_C4()[p1][p2];
            pos++;
        }
    }

  return;
}


void DoitgenHost_mapOpt::emsamble_buffersToData(DoitgenData& data, typeData *temp_resultDevice){
    int pos=0;
    for (int r = 0; r < data.get_SIZE_R(); r++) {
        for (int q = 0; q < data.get_SIZE_Q(); q++) {
            for (int p = 0; p < data.get_SIZE_P(); p++){
                data.get_resultDevice()[r][q][p] = temp_resultDevice[pos];
                pos++;
            }
        }
    }
    return;
}