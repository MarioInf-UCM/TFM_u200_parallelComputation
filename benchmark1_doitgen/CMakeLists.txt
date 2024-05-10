cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCL REQUIRED)
find_package(XRT REQUIRED)
find_package(OpenMP)

if(NOT XRT_FOUND)
  MESSAGE(FATAL_ERROR "XRT not found, please source XRT setup script before building")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/kernelBuild/kerDoitgen_noOptimization.xclbin")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/kernelBuild/kerDoitgen_noOptimization.xclbin ${CMAKE_CURRENT_BINARY_DIR}/build/kernelBuild/kerDoitgen_noOptimization.xclbin COPYONLY)
else()
  MESSAGE(FATAL_ERROR "Kernels aren't compiled. Compile the kernels and try again")
endif()

# Library of utility functions common to all applications
add_library(example_utils STATIC
  src/utilities/event_timer/event_timer.cpp
  src/utilities/xilinx_ocl_helper/xilinx_ocl_helper.cpp
)

target_include_directories(example_utils PUBLIC
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
)

# LOADING DOITGEN EXAMPLE
if (DEFINED TYPEDATA)
  message(STATUS "Value for ENABLE_FEATURE: ${ENABLE_FEATURE}")

  if (TYPEDATA STREQUAL "INT")
    add_definitions(-DTYPEDATA_INT)
  elseif (TYPEDATA STREQUAL "FP")
    add_definitions(-DTYPEDATA_FP)
  else()
    message(WARNING "ERROR: Invalid value for ENABLE_FEATURE: ${ENABLE_FEATURE}")
  endif()

else()
  message(STATUS "Default value for ENABLE_FEATURE")
  add_definitions(-DTYPEDATA_INT)
endif()


add_executable(doitgen
  src/main.cpp
  src/doitgenData/doitgenData.cpp
  src/doitgenHost/doitgenHost.cpp
  )

target_include_directories(doitgen PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${XRT_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
  )

target_link_libraries(doitgen PRIVATE
  ${XRT_LIBS}
  ${OpenCL_LIBRARIES}
  pthread
  uuid
  ${CMAKE_DL_LIBS}
  example_utils
  )




