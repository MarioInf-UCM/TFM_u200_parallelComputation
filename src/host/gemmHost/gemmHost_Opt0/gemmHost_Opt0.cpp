#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <omp.h>
#include "gemmHost_Opt0.hpp"

using namespace std;
using globalConfiguration_typeData::typeData;

using cl::Event;
using cl::CommandQueue;
using cl::Kernel;
using cl::Buffer;


//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
GemmHost_Opt0::GemmHost_Opt0(){}
GemmHost_Opt0::~GemmHost_Opt0(){}
//*************************************
// MAIN FUNCTION - START
//*************************************
bool GemmHost_Opt0::exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    fileWriter_logFile.writeln("Executing host function \"GemmHost::gemmHost_Opt0_exec\". Execution configuration:\n" + exec.displayInfo("\t"));

    unsigned int SIZE_I=0, SIZE_J=0, SIZE_K=0;    
    bool result = initParameter(exec, SIZE_I, SIZE_J, SIZE_K);
    if(!result){
        cout << "\033[1;31mERROR..:Entry params unexpected. Fanalizating execution.\033[0m\n"  << endl;
        return false;
    }
    GemmKernel data = GemmKernel(GEMM_ALPHA, GEMM_BETA, SIZE_I, SIZE_J, SIZE_K);
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

    if(exec.get_kernel().find("gemmPer") != string::npos){
        fileWriter_logFile.writeln("Execution of kernel_gemm_per_CPU");
        data.kernel_gemm_per_CPU();
    }else{
        fileWriter_logFile.writeln("Execution of kernel_gemm_CPU");
        data.kernel_gemm_CPU();
    }

    event.finish();
    fileWriter_logFile.write("STEP 2 - END: Running kernel in CPU (" + event.getInfoEvents(1));
    //STEP 2 - END: Running kernel in CPU"


    //STEP 3 - START: Running kernel in CPU optimizated"
    fileWriter_logFile.writeln("STEP 3 - START: Running kernel in CPU optimizated");
    event.add("Running kernel in CPU optimizated");

    if(exec.get_kernel().find("gemmPer") != string::npos){
        fileWriter_logFile.writeln("Execution of kernel_gemm_per_CPU_opt");
        data.kernel_gemm_per_CPU_opt();
    }else{
        fileWriter_logFile.writeln("Execution of kernel_gemm_CPU_opt");
        data.kernel_gemm_CPU_opt();
    }

    event.finish();
    fileWriter_logFile.write("STEP 3 - END: Running kernel in CPU optimizated (" + event.getInfoEvents(2));
    //STEP 3 - END: Running kernel in CPU optimizated"
    

    //STEP 4 - START: Creating buffer 
    fileWriter_logFile.writeln("STEP 4 - START: Creating buffer");
    event.add("Creating buffers");

    vector<typeData> temp_A = vector<typeData>();
    vector<typeData> temp_B = vector<typeData>();
    vector<typeData> temp_C = vector<typeData>();
    vector<typeData> temp_resultDevice = vector<typeData>();
    emsamble_dataToBuffers(data, temp_A, temp_B, temp_C, temp_resultDevice);

    Buffer sendBuff_A(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_A.size() * sizeof(typeData),
                        temp_A.data(),
                        NULL);

    Buffer sendBuff_B(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_B.size() * sizeof(typeData),
                        temp_B.data(),
                        NULL);

    Buffer sendBuff_C(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_C.size() * sizeof(typeData),
                        temp_C.data(),
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_resultDevice.size() * sizeof(typeData),
                        temp_resultDevice.data(),
                        NULL);

    ker.setArg(0, data.get_alpha());
    ker.setArg(1, data.get_beta());
    ker.setArg(2, sendBuff_A);
    ker.setArg(3, sendBuff_B);
    ker.setArg(4, sendBuff_C);
    ker.setArg(5, recvBuff_resultDevice);

    event.finish();
    fileWriter_logFile.write("STEP 4 - END: Creating buffer (" + event.getInfoEvents(3)); 
    //STEP 4 - END: Creating buffer 


    //STEP 5 - START: Transmision data to device
    fileWriter_logFile.writeln("STEP 5 - START: Transmision data to device");
    event.add("Transmision data to device");

    q.enqueueMigrateMemObjects({sendBuff_A, sendBuff_B, sendBuff_C}, 0, NULL, &event_sp);
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
    results.push_back(data.get_SIZE_I() * data.get_SIZE_J()); 
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



bool GemmHost_Opt0::initParameter(Execution exec, unsigned int &SIZE_I, unsigned int &SIZE_J, unsigned int &SIZE_K){
    if(exec.get_dataSize() == "mini"){
        SIZE_I=GEMM_I_MINI;
        SIZE_J=GEMM_J_MINI;
        SIZE_K=GEMM_K_MINI;
    }else if(exec.get_dataSize() == "small"){
        SIZE_I=GEMM_I_SMALL;
        SIZE_J=GEMM_J_SMALL;
        SIZE_K=GEMM_K_SMALL;
    }else if(exec.get_dataSize() == "medium"){
        SIZE_I=GEMM_I_MEDIUM;
        SIZE_J=GEMM_J_MEDIUM;
        SIZE_K=GEMM_K_MEDIUM;
    }else if(exec.get_dataSize() == "large"){
        SIZE_I=GEMM_I_LARGE;
        SIZE_J=GEMM_J_LARGE;
        SIZE_K=GEMM_K_LARGE;
    }else if(exec.get_dataSize() == "extralarge"){
        SIZE_I=GEMM_I_EXTRALARGE;
        SIZE_J=GEMM_J_EXTRALARGE;
        SIZE_K=GEMM_K_EXTRALARGE;
    }else{
        return false;
    }  
    return true;
}



bool GemmHost_Opt0::compareResults(GemmKernel& data){
    for (int i = 0; i < data.get_SIZE_I(); i++){
      for (int j = 0; j < data.get_SIZE_J(); j++){
        if(data.get_resultCPU()[i][j] != data.get_resultDevice()[i][j]){
            return false;
        }
      }
    }
    return true;
}



void GemmHost_Opt0::emsamble_dataToBuffers(GemmKernel& data, vector<typeData> &temp_A, vector<typeData>& temp_B, vector<typeData>& temp_C, vector<typeData>& temp_resultDevice){

    temp_A.clear();
    temp_resultDevice.clear();
    for (int i = 0; i < data.get_SIZE_I(); i++) {
        for (int k = 0; k < data.get_SIZE_K(); k++) {
            temp_A.push_back(data.get_A()[i][k]);
            temp_resultDevice.push_back( 0.0f );
        }
    }

    temp_B.clear();
    for (int k = 0; k < data.get_SIZE_K(); k++) {
        for (int j = 0; j < data.get_SIZE_J(); j++) {
            temp_B.push_back( data.get_B()[k][j] );
        }
    }

    temp_C.clear();
    temp_resultDevice.clear();
    for (int i = 0; i < data.get_SIZE_I(); i++) {
        for (int j = 0; j < data.get_SIZE_J(); j++) {
            temp_C.push_back(data.get_C()[i][j]);
            temp_resultDevice.push_back( 0.0f );
        }
    }

  return;
}


void GemmHost_Opt0::emsamble_buffersToData(GemmKernel& data, vector<typeData>& temp_resultDevice){
    
    int pos=0;
    for (int i = 0; i < data.get_SIZE_I(); i++) {
        for (int j = 0; j < data.get_SIZE_J(); j++) {
            data.get_resultDevice()[i][j] = temp_resultDevice[pos];
            pos++;
        }
    }
    return;
}