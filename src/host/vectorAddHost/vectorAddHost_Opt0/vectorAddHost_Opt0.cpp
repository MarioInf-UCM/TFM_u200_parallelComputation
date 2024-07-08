#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <omp.h>
#include "vectorAddHost_Opt0.hpp"

using namespace std;
using globalConfiguration_typeData::typeData;

using cl::Event;
using cl::CommandQueue;
using cl::Kernel;
using cl::Buffer;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
VectorAddHost_Opt0::VectorAddHost_Opt0(){}
VectorAddHost_Opt0::~VectorAddHost_Opt0(){}

//*************************************
// MAIN FUNCTION - START
//*************************************
bool VectorAddHost_Opt0::exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    fileWriter_logFile.writeln("Executing host function \"VectorAddHost::VectorAddHost_Opt0_exec\". Execution configuration:\n" + exec.displayInfo("\t"));

    //STEP 1 - START: Initializating parameters"
    fileWriter_logFile.writeln("STEP 0 - START: Initializating parameters");
    
    unsigned int SIZE=0;    
    bool result = initParameter(exec, SIZE);
    if(!result){
        cout << "\033[1;31mERROR..:Entry params unexpected. Fanalizating execution.\033[0m\n"  << endl;
        return false;
    }
    VectorAddKernel data = VectorAddKernel(SIZE);
    EventTimer event;
    Event event_sp;

    fileWriter_logFile.writeln("STEP 0 - END: Initializating parameters");
    //STEP 1 - END: Initializating parameters"


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

    data.kernel_vectorAdd_CPU();

    event.finish();
    fileWriter_logFile.write("STEP 2 - END: Running kernel in CPU (" + event.getInfoEvents(1));
    //STEP 2 - END: Running kernel in CPU"


    //STEP 3 - START: Running kernel in CPU optimizated"
    fileWriter_logFile.writeln("STEP 3 - START: Running kernel in CPU optimizated");
    event.add("Running kernel in CPU optimizated");

    data.kernel_vectorAdd_CPU_opt();

    event.finish();
    fileWriter_logFile.write("STEP 3 - END: Running kernel in CPU optimizated (" + event.getInfoEvents(2));
    //STEP 3 - END: Running kernel in CPU optimizated"


    //STEP 4 - START: Creating buffer 
    fileWriter_logFile.writeln("STEP 4 - START: Creating buffer");
    event.add("Creating buffers");

    vector<typeData> temp_vA = vector<typeData>();
    vector<typeData> temp_vB = vector<typeData>();
    vector<typeData> temp_resultDevice = vector<typeData>();
    ensamble_dataToBuffers(data, temp_vA, temp_vB, temp_resultDevice);

    Buffer sendBuff_vA(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_vA.size() * sizeof(typeData),
                        temp_vA.data(),
                        NULL);

    Buffer sendBuff_vB(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_vB.size() * sizeof(typeData),
                        temp_vB.data(),
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR),
                        temp_resultDevice.size() * sizeof(typeData),
                        temp_resultDevice.data(),
                        NULL);

    ker.setArg(0, sendBuff_vA);
    ker.setArg(1, sendBuff_vB);
    ker.setArg(2, recvBuff_resultDevice);

    event.finish();
    fileWriter_logFile.write("STEP 4 - END: Creating buffer (" + event.getInfoEvents(3)); 
    //STEP 4 - END: Creating buffer 


    //STEP 5 - START: Transmision data to device
    fileWriter_logFile.writeln("STEP 5 - START: Transmision data to device");
    event.add("Transmision data to device");

    q.enqueueMigrateMemObjects({sendBuff_vA, sendBuff_vB}, 0, NULL, &event_sp);
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

    event.finish();
    fileWriter_logFile.write("STEP 7 - END: Transmision data from device (" + event.getInfoEvents(6));
    //STEP 7 - END: Transmision data from device 
    
    
    ensamble_buffersToData(data, temp_resultDevice);
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
    results.push_back(stod(event.getTimeEvents(1)));                                  //CPU execution time
    results.push_back(stod(event.getTimeEvents(2)));                                  //CPU execution time optimizated
    results.push_back(stod(event.getTimeEvents(5)));                                  //Device execution time 
    results.push_back(stod(event.getTimeEvents(4)) + stod(event.getTimeEvents(6)));   //Transmision (Send+Recv) time
    results.push_back(stod(event.getTimeEvents(4)));                                  //Send to device time
    results.push_back(stod(event.getTimeEvents(6)));                                  //Recieve from device time

  return result;
}
//*************************************
// MAIN FUNCTION - END
//*************************************



bool VectorAddHost_Opt0::initParameter(Execution exec, unsigned int &SIZE){
    if(exec.get_dataSize() == "mini"){
        SIZE=VECTORADD_SIZE_MINI;
    }else if(exec.get_dataSize() == "small"){
        SIZE=VECTORADD_SIZE_SMALL;
    }else if(exec.get_dataSize() == "medium"){
        SIZE=VECTORADD_SIZE_MEDIUM;
    }else if(exec.get_dataSize() == "large"){
        SIZE=VECTORADD_SIZE_LARGE;
    }else if(exec.get_dataSize() == "extralarge"){
        SIZE=VECTORADD_SIZE_EXTRALARGE;
    }else{
        return false;
    }  
    return true;
}



bool VectorAddHost_Opt0::compareResults(VectorAddKernel& data){
    for (int i = 0; i < data.get_resultDevice().size() ; i++) {
        if(data.get_resultCPU()[i] != data.get_resultDevice()[i]){
            //cout << n1 << "  " << n2 << "  " << data.get_resultCPU()[n1][n2] << "  " << data.get_resultDevice()[n1][n2] << endl;
            return false;
        }
    }
    return true;
}



void VectorAddHost_Opt0::ensamble_dataToBuffers(VectorAddKernel& data, vector<typeData> &temp_A, vector<typeData> &temp_B, vector<typeData>& temp_resultDevice){

    temp_A.clear();
    temp_B.clear();
    temp_resultDevice.clear();
    for (int i = 0; i < data.get_SIZE(); i++) {
        temp_A.push_back( data.get_vA()[i] );
        temp_B.push_back( data.get_vB()[i] );
        temp_resultDevice.push_back( 0.0 );
    }

  return;
}


void VectorAddHost_Opt0::ensamble_buffersToData(VectorAddKernel& data, vector<typeData>& temp_resultDevice){
    
    for (int i = 0; i < data.get_resultDevice().size() ; i++) {
        data.get_resultDevice()[i] = temp_resultDevice[i];
    }
    return;
}