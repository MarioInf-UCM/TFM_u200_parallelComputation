cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules")

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)
find_package(OpenMP REQUIRED)
find_package(jsoncpp REQUIRED)

if(NOT XRT_FOUND)
  message(FATAL_ERROR "XRT not found, please source XRT setup script before building")
endif()


# Copying kernel files
#******************************************************************
if (DEFINED TARGET)
  message(STATUS "Value for TARGET: ${TARGET}")
  

  if(${TARGET} STREQUAL "sw_emu")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_noOpt/sw_emu/kerPack_doitgen_noOpt_sw_emu.xclbin")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_noOpt/sw_emu/kerPack_doitgen_noOpt_sw_emu.xclbin ${CMAKE_CURRENT_BINARY_DIR}/kerPack_doitgen_noOpt_sw_emu.xclbin COPYONLY)
    else()
      message(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again (kerPack_doitgen_noOpt_sw_emu.xclbin)")
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_Opt/sw_emu/kerPack_doitgen_Opt_sw_emu.xclbin")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_Opt/sw_emu/kerPack_doitgen_Opt_sw_emu.xclbin ${CMAKE_CURRENT_BINARY_DIR}/kerPack_doitgen_Opt_sw_emu.xclbin COPYONLY)
    else()
      message(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again (kerPack_doitgen_Opt_sw_emu.xclbin)")
    endif()


  elseif (${TARGET} STREQUAL "hw_emu")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_noOpt/hw_emu/kerPack_doitgen_noOpt_hw_emu.xclbin")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_noOpt/hw_emu/kerPack_doitgen_noOpt_hw_emu.xclbin ${CMAKE_CURRENT_BINARY_DIR}/kerPack_doitgen_noOpt_hw_emu.xclbin COPYONLY)
    else()
      message(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again (kerPack_doitgen_noOpt_hw_emu.xclbin)")
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_Opt/hw_emu/kerPack_doitgen_Opt_hw_emu.xclbin")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_Opt/hw_emu/kerPack_doitgen_Opt_hw_emu.xclbin ${CMAKE_CURRENT_BINARY_DIR}/kerPack_doitgen_Opt_hw_emu.xclbin COPYONLY)
    else()
      message(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again (kerPack_doitgen_Opt_hw_emu.xclbin)")
    endif()


  elseif (${TARGET} STREQUAL "hw")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_noOpt/hw/kerPack_doitgen_noOpt_hw.xclbin")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_noOpt/hw/kerPack_doitgen_noOpt_hw.xclbin ${CMAKE_CURRENT_BINARY_DIR}/kerPack_doitgen_noOpt_hw.xclbin COPYONLY)
    else()
      message(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again (kerPack_doitgen_noOpt_hw.xclbin)")
    endif()
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_Opt/hw/kerPack_doitgen_Opt_hw.xclbin")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../build/kernelBuild/ker_doitgen_Opt/hw/kerPack_doitgen_Opt_hw.xclbin ${CMAKE_CURRENT_BINARY_DIR}/kerPack_doitgen_Opt_hw.xclbin COPYONLY)
    else()
      message(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again (kerPack_doitgen_Opt_hw.xclbin)")
    endif()


  else()
    message(FATAL_ERROR "ERROR: Invalid value for ENABLE_FEATURE: ${ENABLE_FEATURE}")
  endif()


else()
  message(FATAL_ERROR "ERROR: TARGET not define.")
endif()


# Copying python files
#******************************************************************
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../python/generatePyctures.py")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../python/generatePyctures.py ${CMAKE_CURRENT_BINARY_DIR}/generatePyctures.py COPYONLY)
else()
  message(FATAL_ERROR "Python files haven't been founded")
endif()



# Library of utility functions common to all applications
#******************************************************************
add_library(example_utils STATIC
  ../src/utilities/dateAndTime/dateAndTime.cpp
  ../src/utilities/event_timer/event_timer.cpp
  ../src/utilities/xilinx_ocl_helper/xilinx_ocl_helper.cpp
)

target_include_directories(example_utils PUBLIC
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
)

if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)

endif()

message(STATUS ${OpenMP_CXX_FLAGS})


#******************************
# COMPILING HOST APPLICATION *
#******************************
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp")

add_executable(benchmarkExec
  ../src/main.cpp
  ../src/doitgenData/doitgenData.cpp
  ../src/doitgenHost/doitgenHost.cpp
  ../src/doitgenHost/doitgenHost_noOpt/doitgenHost_noOpt.cpp
  ../src/doitgenHost/doitgenHost_mapOpt/doitgenHost_mapOpt.cpp
  ../src/service/fileWriter_service/fileWriter_service.cpp
  ../src/service/json_service/json_service.cpp
  ../src/service/json_service/jsonConfiguration/jsonConfiguration.cpp
  ../src/service/json_service/jsonConfiguration/execution/execution.cpp
  ../src/service/json_service/jsonConfiguration/test/test.cpp
  ../src/service/externProgramsConnection_service/externProgramsConnection_service.cpp
  )

target_include_directories(benchmarkExec PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  ${jsoncpp_INCLUDE_DIRS} 
  )

target_link_libraries(benchmarkExec PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  jsoncpp_lib
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  OpenMP::OpenMP_CXX
  )

