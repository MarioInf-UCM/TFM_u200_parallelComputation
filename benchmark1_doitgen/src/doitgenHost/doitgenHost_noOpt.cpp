#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include "doitgenHost.hpp"

using namespace std;
using namespace cl;

//********************************
//* CONSTRUCTORS AND DESTRUCTORS *
//********************************
DoitgenHost::DoitgenHost(){}
DoitgenHost::~DoitgenHost(){}


//*************************************
// MAIN FUNCTION - START
//*************************************
int DoitgenHost::doitgenHost_noOpt_exec(Execution exec, FileWriter_service fileWriter_logFile){
    fileWriter_logFile.writeln("Executing host function \"DoitgenHost::doitgenHost_noOpt_exec\". Execution configuration:\n" + exec.displayInfo("\t"));

    DoitgenData data = DoitgenData();
    ostringstream stringToPrint;
    EventTimer event;
    Event event_sp;
    bool result;

    //STEP 1 - START: Initializaton OpenCL and load kernels"
    fileWriter_logFile.writeln("STEP 1 - START: Initializaton OpenCL and load kernels");
    event.add("Initializaton OpenCL and load kernels");

    xilinx::example_utils::XilinxOclHelper xocl;
    xocl.initialize(exec.get_kernelPackage());
    CommandQueue q = xocl.get_command_queue();
    Kernel ker = xocl.get_kernel(exec.get_kernel());

    event.finish();
    fileWriter_logFile.writeln("STEP 1 - END: Initializaton OpenCL and load kernels (" + event.getInfoEvents(0) + "ms)");
    //STEP 1 - END: Initializaton OpenCL and load kernels"


    //STEP 2 - START: Running kernel in CPU"
    fileWriter_logFile.writeln("STEP 2 - START: Running kernel in CPU");
    event.add("Running kernel in CPU");

    kernel_doitgen_CPU(&data);

    event.finish();
    fileWriter_logFile.writeln("STEP 2 - END: Running kernel in CPU (" + event.getInfoEvents(1) + "ms)");
    //STEP 2 - END: Running kernel in CPU"


    //STEP 3 - START: Creating and mapping buffer 
    fileWriter_logFile.writeln("STEP 3 - START: Creating and mapping buffer");
    event.add("Creating and mapping buffer");

    Buffer sendBuff_A(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        SIZE_R * SIZE_Q * SIZE_P * sizeof(typeData),
                        data.getA(),
                        NULL);

    Buffer sendBuff_C4(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR),
                        SIZE_P * SIZE_P * sizeof(typeData),
                        data.getC4(),
                        NULL);

    Buffer recvBuff_resultDevice(xocl.get_context(),
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR),
                        SIZE_R * SIZE_Q * SIZE_P * sizeof(typeData),
                        data.getResultDevice(),
                        NULL);

    ker.setArg(0, sendBuff_A);
    ker.setArg(1, sendBuff_C4);
    ker.setArg(2, recvBuff_resultDevice);

    event.finish();
    fileWriter_logFile.writeln("STEP 3 - END: Creating and mapping buffer (" + event.getInfoEvents(2) + "ms)"); 
    //STEP 3 - END: Creating and mapping buffer 


    //STEP 4 - START: Transmision data to device
    fileWriter_logFile.writeln("STEP 4 - START: Transmision data to device");
    event.add("Transmision data to device");

    q.enqueueMigrateMemObjects({sendBuff_A, sendBuff_C4}, 0, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    event.finish();
    fileWriter_logFile.writeln("4 - END: Transmision data to device (" + event.getInfoEvents(2) + "ms)");
    //STEP 4 - END: Transmision data to device 


    //STEP 5 - START: Device execution
    fileWriter_logFile.writeln("STEP 5 - START: Device execution");
    event.add("Device execution");

    q.enqueueTask(ker, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);

    event.finish();
    fileWriter_logFile.writeln("STEP 5 - END: Device execution (" + event.getInfoEvents(2) + "ms)");
    //STEP 5 - END: Device execution


    //STEP 6 - START: Transmision data from device
    fileWriter_logFile.writeln("STEP 6 - START: Transmision data from device");
    event.add("Transmision data from device ");

    q.enqueueMigrateMemObjects({recvBuff_resultDevice}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &event_sp);
    clWaitForEvents(1, (const cl_event *)&event_sp);
    q.finish();

    event.finish();
    fileWriter_logFile.writeln("STEP 6 - END: Transmision data from device (" + event.getInfoEvents(2) + "ms)");
    //STEP 6 - END: Transmision data from device 

    if(PRINTRESULT){
        printArrays(data);
    }

    if(compareResults(data)){
        cout << endl;
        cout << endl;
        cout << "\033[1;32m***WELL, The results match***\033[0m\n"  << endl;
        printKeyResults(event);
        //stringToPrint = event.getStream();
        //fileWriter.write(stringToPrint, VERBOSE);
    }else{
        cout << endl;
        cout << endl;
        cout << "\033[1;31m***BAD, The results don't match***\033[0m\n"  << endl;
        return 0;
    }



  return 0;
}
//*************************************
// MAIN FUNCTION - END
//*************************************



void DoitgenHost::kernel_doitgen_CPU(DoitgenData *data){

    double sum[SIZE_P];

    for (int r = 0; r < SIZE_R; r++){
      for (int q = 0; q < SIZE_Q; q++){
        for (int p = 0; p < SIZE_P; p++){
          
          for (int s = 0; s < SIZE_P; s++){
            sum[p]=0.0;
            sum[p] += data->getA()[r][q][s] * data->getC4()[s][p];
          }
        }

        for (int p = 0; p < SIZE_P; p++){
            data->getResultCPU()[r][q][p] = sum[p];
        }
      }
    }

  return;
}


bool DoitgenHost::compareResults(DoitgenData data){
    for (int r = 0; r < SIZE_R; r++) {
      for (int q = 0; q < SIZE_Q; q++) {
        for (int p = 0; p < SIZE_P; p++){
          if(data.getResultCPU()[r][q][p] != data.getResultDevice()[r][q][p]){
              return false;
          }
        }
      }
    }
    return true;
}


void DoitgenHost::printKeyResults(EventTimer event){

    cout << "--------------- Key execution times ---------------" << endl;
    cout << "-- (SIZE_R = " << SIZE_R << "  SIZE_Q = " << SIZE_Q << "  SIZE_P = " << SIZE_P << "  ) --" << endl;
    cout << endl;
    event.print();
    cout << endl;
    cout << "---------------------------------------------------" << endl;

  return;
}


void DoitgenHost::printArrays(DoitgenData data){

    cout << "*******************" << endl;
    cout << "* RESULTS ARRAY A *" << endl;
    cout << "*******************" << endl;
    data.printData_A();

    cout << endl;
    cout << "********************" << endl;
    cout << "* RESULTS ARRAY C4 *" << endl;
    cout << "********************" << endl;
    data.printData_C4();

    cout << endl;
    cout << "********************" << endl;
    cout << "* RESULTS FROM CPU *" << endl;
    cout << "********************" << endl;
    data.printData_resultCPU();

    cout << endl;
    cout << "***********************" << endl;
    cout << "* RESULTS FROM DEVICE *" << endl;
    cout << "***********************" << endl;
    data.printData_resultDevice();

  return;
}