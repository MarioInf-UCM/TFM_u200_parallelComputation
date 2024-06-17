#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <omp.h>
#include "jacobi2dHost_Opt1.hpp"

using namespace std;
using globalConfiguration_typeData::typeData;

using cl::Event;
using cl::CommandQueue;
using cl::Kernel;
using cl::Buffer;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
Jacobi2dHost_Opt1::Jacobi2dHost_Opt1(){}
Jacobi2dHost_Opt1::~Jacobi2dHost_Opt1(){}

//*************************************
// MAIN FUNCTION - START
//*************************************
bool Jacobi2dHost_Opt1::exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    fileWriter_logFile.writeln("Executing host function \"Jacobi2dHost::Jacobi2dHost_Opt1_exec\". Execution configuration:\n" + exec.displayInfo("\t"));

    unsigned int STEPS=0, SIZE_N=0;    
    bool result = initParameter(exec, STEPS, SIZE_N);
    if(!result){
        cout << "\033[1;31mERROR..:Entry params unexpected. Fanalizating execution.\033[0m\n"  << endl;
        return false;
    }
    Jacobi_2dKernel data = Jacobi_2dKernel(STEPS, SIZE_N);
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

    data.kernel_jacobi_2d_CPU();

    event.finish();
    fileWriter_logFile.write("STEP 2 - END: Running kernel in CPU (" + event.getInfoEvents(1));
    //STEP 2 - END: Running kernel in CPU"


    //STEP 3 - START: Running kernel in CPU optimizated"
    fileWriter_logFile.writeln("STEP 3 - START: Running kernel in CPU optimizated");
    data.initData_A(data.get_SIZE_N());
    data.initData_B(data.get_SIZE_N());
    event.add("Running kernel in CPU optimizated");

    data.kernel_jacobi_2d_CPU_opt();

    event.finish();
    fileWriter_logFile.write("STEP 3 - END: Running kernel in CPU optimizated (" + event.getInfoEvents(2));
    //STEP 3 - END: Running kernel in CPU optimizated"


    //STEP 4 - START: Creating buffer
    fileWriter_logFile.writeln("STEP 4 - START: Creating buffer");
    data.initData_A(data.get_SIZE_N());
    data.initData_B(data.get_SIZE_N());
    event.add("Creating buffers");

    cl_mem_ext_ptr_t bank_ext0;
    bank_ext0.flags = 0 | XCL_MEM_TOPOLOGY;
    bank_ext0.obj   = NULL;
    bank_ext0.param = 0;

    cl_mem_ext_ptr_t bank_ext1;
    bank_ext1.flags = 1 | XCL_MEM_TOPOLOGY;
    bank_ext1.obj   = NULL;
    bank_ext1.param = 0;

    cl_mem_ext_ptr_t bank_ext2;
    bank_ext2.flags = 2 | XCL_MEM_TOPOLOGY;
    bank_ext2.obj   = NULL;
    bank_ext2.param = 0;

    Buffer sendBuff_A(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX),
                        data.get_SIZE_N() * data.get_SIZE_N() * sizeof(typeData),
                        &bank_ext0,
                        NULL);

    Buffer sendBuff_B(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX),
                        data.get_SIZE_N() * data.get_SIZE_N() * sizeof(typeData),
                        &bank_ext1,
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX),
                        data.get_SIZE_N() * data.get_SIZE_N() * 2 * sizeof(typeData),
                        &bank_ext2,
                        NULL);

    ker.setArg(0, sendBuff_A);
    ker.setArg(1, sendBuff_B);
    ker.setArg(2, recvBuff_resultDevice);

    typeData *temp_A = (typeData *)q.enqueueMapBuffer(sendBuff_A,
                                                        CL_TRUE,
                                                        CL_MAP_WRITE,
                                                        0,
                                                        data.get_SIZE_N() * data.get_SIZE_N() * sizeof(typeData));

    typeData *temp_B = (typeData *)q.enqueueMapBuffer(sendBuff_B,
                                                        CL_TRUE,
                                                        CL_MAP_WRITE,
                                                        0,
                                                        data.get_SIZE_N() * data.get_SIZE_N() * sizeof(typeData));

    typeData *temp_resultDevice = (typeData *)q.enqueueMapBuffer(recvBuff_resultDevice,
                                                        CL_TRUE,
                                                        CL_MAP_WRITE | CL_MAP_READ,
                                                        0,
                                                        data.get_SIZE_N() * data.get_SIZE_N() * 2 * sizeof(typeData));

    ensamble_dataToBuffers(data, temp_A, temp_B, temp_resultDevice);

    event.finish();
    fileWriter_logFile.write("STEP 4 - END: Creating buffer (" + event.getInfoEvents(3)); 
    //STEP 4 - END: Creating buffer 



    //STEP 5 - START: Transmision data to device
    fileWriter_logFile.writeln("STEP 5 - START: Transmision data to device");
    event.add("Transmision data to device");

    q.enqueueMigrateMemObjects({sendBuff_A}, 0, NULL, &event_sp);
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

    ensamble_buffersToData(data, temp_resultDevice);
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
    results.push_back(data.get_SIZE_N()); 
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



bool Jacobi2dHost_Opt1::initParameter(Execution exec, unsigned int &STEPS, unsigned int &SIZE_N){
    if(exec.get_dataSize() == "mini"){
        STEPS=JACOBI2D_TSTEPS_MINI;
        SIZE_N=JACOBI2D_N_MINI;
    }else if(exec.get_dataSize() == "small"){
        STEPS=JACOBI2D_TSTEPS_SMALL;
        SIZE_N=JACOBI2D_N_SMALL;
    }else if(exec.get_dataSize() == "medium"){
        STEPS=JACOBI2D_TSTEPS_MEDIUM;
        SIZE_N=JACOBI2D_N_MEDIUM;
    }else if(exec.get_dataSize() == "large"){
        STEPS=JACOBI2D_TSTEPS_LARGE;
        SIZE_N=JACOBI2D_N_LARGE;
    }else if(exec.get_dataSize() == "extralarge"){
        STEPS=JACOBI2D_TSTEPS_EXTRALARGE;
        SIZE_N=JACOBI2D_N_EXTRALARGE;
    }else{
        return false;
    }  
    return true;
}



bool Jacobi2dHost_Opt1::compareResults(Jacobi_2dKernel& data){
    for (int n1 = 0; n1 < data.get_SIZE_N()*2; n1++) {
        for (int n2 = 0; n2 < data.get_SIZE_N(); n2++) {
            if(data.get_resultCPU()[n1][n2] != data.get_resultDevice()[n1][n2]){
                //cout << n1 << "  " << n2 << "  " << data.get_resultCPU()[n1][n2] << "  " << data.get_resultDevice()[n1][n2] << endl;
                return false;
            }
        }
    }
    return true;
}



void Jacobi2dHost_Opt1::ensamble_dataToBuffers(Jacobi_2dKernel& data, typeData *temp_A, typeData *temp_B, typeData *temp_resultDevice){

    int pos=0;
    for (int n1 = 0; n1 < data.get_SIZE_N(); n1++) {
        for (int n2 = 0; n2 < data.get_SIZE_N(); n2++) {
            temp_A[pos] = data.get_A()[n1][n2];
            temp_B[pos] = data.get_B()[n1][n2];
            temp_resultDevice[pos] = 0.0;
            pos++;
        }
    }

    pos=0;
    for (int n1 = 0; n1 < data.get_SIZE_N(); n1++) {
        for (int n2 = 0; n2 < data.get_SIZE_N(); n2++) {
            temp_resultDevice[(data.get_SIZE_N() * data.get_SIZE_N()) +pos] = 0.0;
            pos++;
        }
    }

  return;
}


void Jacobi2dHost_Opt1::ensamble_buffersToData(Jacobi_2dKernel& data, typeData *temp_resultDevice){
    
    int i=0;
    for (int n1 = 0; n1 < data.get_resultDevice().size() ; n1++) {
        for (int n2 = 0; n2 < data.get_resultDevice()[n1].size() ; n2++) {
            data.get_resultDevice()[n1][n2] = temp_resultDevice[i];
            i++;
        }
    }
    return;
}