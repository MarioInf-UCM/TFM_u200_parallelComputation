#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <mutex>
#include <omp.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <xrt.h>
#include <papi.h>
#include <xrt/xrt_device.h>
#include "vectorAddHost_Opt4.hpp"

using namespace std;
using globalConfiguration_typeData::typeData;
using globalConfiguration_typeData::typeData_fixed;

using cl::Event;
using cl::CommandQueue;
using cl::Kernel;
using cl::Buffer;


atomic<bool> stop_thread(false);
mutex mtx;
float sharedVariable = 0.0f;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
VectorAddHost_Opt4::VectorAddHost_Opt4(){}
VectorAddHost_Opt4::~VectorAddHost_Opt4(){}

//*************************************
// MAIN FUNCTION - START
//*************************************
bool VectorAddHost_Opt4::exec(Execution exec, vector<double>& results, FileWriter_service fileWriter_logFile, FileWriter_service fileWriter_statsFile){
    fileWriter_logFile.writeln("Executing host function \"VectorAddHost::VectorAddHost_Opt4_exec\". Execution configuration:\n" + exec.displayInfo("\t"));


    //STEP 0 - START: Initializating parameters"
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
    float resultMeasure_Device=0.0f, resultMeasure_CPU=0.0f, resultMeasure_CPUopt=0.0f;

    bool PAPImeasureFlag=true;
    int PAPIevent_PowerMeasure = PAPI_NULL;
    long long CPU_powerMeasure;
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        PAPImeasureFlag=false;
        cerr << "ERROR..: We couldn't initialize PAPI library." << endl;
    }
    if (PAPI_create_eventset(&PAPIevent_PowerMeasure) != PAPI_OK) {
        PAPImeasureFlag=false;
        cerr << "ERROR..: We couldn't create the serie of events." << endl;
    }

    if (PAPI_add_event(PAPIevent_PowerMeasure, PAPI_FP_OPS) != PAPI_OK) {
        PAPImeasureFlag=false;
        std::cerr << "ERROR..: We couldn't add energy measurent event to serie of events" << endl;
    }


    fileWriter_logFile.writeln("STEP 0 - END: Initializating parameters");
    //STEP 0 - END: Initializating parameters"


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

    if(PAPImeasureFlag){
        stop_thread.store(false);
        //thread thread_deviceMeasure(threadFunction_CPUPowerMeasure, resultMeasure_CPU);
        data.kernel_vectorAdd_CPU();
        stop_thread.store(true);

    }else{
        data.kernel_vectorAdd_CPU();
    }

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

    Buffer sendBuff_vA(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX),
                        data.get_SIZE() * sizeof(typeData_fixed),
                        &bank_ext0,
                        NULL);

    Buffer sendBuff_vB(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX),
                        data.get_SIZE() * sizeof(typeData_fixed),
                        &bank_ext1,
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_EXT_PTR_XILINX),
                        data.get_SIZE() * sizeof(typeData_fixed),
                        &bank_ext2,
                        NULL);

    ker.setArg(0, sendBuff_vA);
    ker.setArg(1, sendBuff_vB);
    ker.setArg(2, recvBuff_resultDevice);

    typeData_fixed *temp_vA = (typeData_fixed *)q.enqueueMapBuffer(sendBuff_vA,
                                                        CL_TRUE,
                                                        CL_MAP_WRITE,
                                                        0,
                                                        data.get_SIZE() * sizeof(typeData_fixed));

    typeData_fixed *temp_vB = (typeData_fixed *)q.enqueueMapBuffer(sendBuff_vB,
                                                        CL_TRUE,
                                                        CL_MAP_WRITE,
                                                        0,
                                                        data.get_SIZE() * sizeof(typeData_fixed));

    typeData_fixed *temp_resultDevice = (typeData_fixed *)q.enqueueMapBuffer(recvBuff_resultDevice,
                                                        CL_TRUE,
                                                        CL_MAP_WRITE | CL_MAP_READ,
                                                        0,
                                                        data.get_SIZE() * sizeof(typeData_fixed));

    ensamble_dataToBuffers(data, temp_vA, temp_vB, temp_resultDevice);

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
    
    if(exec.get_kernelPackage().find("hw.xclbin") != string::npos){
        
        stop_thread.store(false);
        q.enqueueTask(ker, NULL, &event_sp);
        event.add("Device execution");

        thread thread_deviceMeasure(threadFunction_DevicePowerMeasure);
        
        clWaitForEvents(1, (const cl_event *)&event_sp);
        event.finish();

        stop_thread.store(true);
        if (thread_deviceMeasure.joinable()) {
            thread_deviceMeasure.join();
        }
        
        resultMeasure_Device = sharedVariable;

    }else{
        q.enqueueTask(ker, NULL, &event_sp);
        event.add("Device execution");
        clWaitForEvents(1, (const cl_event *)&event_sp);  
        event.finish();      
    }

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
bool VectorAddHost_Opt4::initParameter(Execution exec, unsigned int &SIZE){
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



bool VectorAddHost_Opt4::compareResults(VectorAddKernel& data){

    typeData_fixed tolerance = 0.01;
    typeData_fixed tempDataCPU, tempDataDevice;

    for (int i = 0; i < data.get_resultDevice().size() ; i++) {
        tempDataCPU = data.get_resultCPU()[i];
        tempDataDevice = data.get_resultDevice()[i];
        if( !((tempDataCPU > tempDataDevice ? tempDataCPU - tempDataDevice : tempDataDevice - tempDataCPU) < tolerance) ){
            //cout << i << "  " << data.get_resultCPU()[i] << "  " << data.get_resultDevice()[i] << endl;
            return false;
        }
    }
    return true;
}



void VectorAddHost_Opt4::ensamble_dataToBuffers(VectorAddKernel& data, typeData_fixed *temp_A, typeData_fixed *temp_B, typeData_fixed *temp_resultDevice){

    for (int i = 0; i < data.get_SIZE(); i++) {
        temp_A[i] = data.get_vA()[i];
        temp_B[i] = data.get_vB()[i];
        temp_resultDevice[i]=0.0;
    }

  return;
}



void VectorAddHost_Opt4::ensamble_buffersToData(VectorAddKernel& data, typeData_fixed *temp_resultDevice){
    
    for (int i = 0; i < data.get_SIZE() ; i++) {
        data.get_resultDevice()[i] = temp_resultDevice[i];
    }
    return;
}




float VectorAddHost_Opt4::searchPropertyValue(const string& texto, const string& subcadena) {
    istringstream stream(texto);
    string linea;
    string resultado;

    while (getline(stream, linea)) {
        if (linea.find(subcadena) != string::npos) {
            size_t start = linea.find(":") + 1;
            size_t end = linea.find(",", start);
            if (end == string::npos) {
                end = linea.size();
            }

            resultado = linea.substr(start, end - start);
            resultado.erase(remove(resultado.begin(), resultado.end(), '\"'), resultado.end());
            resultado.erase(remove(resultado.begin(), resultado.end(), ' '), resultado.end());
            return stof(resultado);
        }
    }
    return 0.0f;
}



void VectorAddHost_Opt4::threadFunction_CPUPowerMeasure(int *PAPIevent_PowerMeasure, atomic<double> &CPU_powerMeasure) {

    cout << "Measuring CPU Power...";
    unsigned int numIter=0;
    long long measure_picoJulios=0, measureTemp_picoJulios=0;
    double measure_Julios=0;
    CPU_powerMeasure=0;

    if (PAPI_start(*PAPIevent_PowerMeasure) != PAPI_OK) {
        cerr << "ERROR..: We couldn't init the serie of events" << endl;
    }
    auto start = chrono::high_resolution_clock::now();
    
    do{

        if (PAPI_read(*PAPIevent_PowerMeasure, &measureTemp_picoJulios) != PAPI_OK) {
            cerr << "ERROR..: We couldn't read the power measure event"<< endl;
        }


        measure_picoJulios += measure_picoJulios + measureTemp_picoJulios;
        this_thread::sleep_for(chrono::milliseconds(1));
    }while (!stop_thread.load());
   
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    if (PAPI_stop(*PAPIevent_PowerMeasure, NULL) != PAPI_OK) {
        cerr << "ERROR..: We couldn't stop the serie of events" << endl;
    }

    measure_picoJulios = measure_picoJulios/numIter;
    measure_Julios = measure_picoJulios * 1e-12;
    CPU_powerMeasure = measure_Julios / elapsed.count();

    cout << "...Finalizing Measure CPU power ("<< CPU_powerMeasure <<" Watts  |  " << numIter << " lectures )." << endl;
    return;
}



void VectorAddHost_Opt4::threadFunction_DevicePowerMeasure() {

    cout << "Measuring device Power...";
    unsigned int numIter=0; 

    sharedVariable=0;
    do{
        //lock_guard<mutex> lock(mtx);
        sharedVariable += searchPropertyValue( xrt::device(0).get_info<xrt::info::device::electrical>(), "power_consumption_watts");
        numIter++;
        this_thread::sleep_for(chrono::milliseconds(1));
    }while (!stop_thread.load());

    sharedVariable = sharedVariable/numIter;
    cout << "...Finalizing Measure device power ("<< sharedVariable <<" Watts  |  " << numIter << " lectures )." << endl;
    return;
}

