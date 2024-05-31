CURRENT_DIR := $(shell pwd)
BUILD_DIRECTORY=build/hostBuild
CMAKE_DIRECTORY=../../cmake

KERNELS_ROUTE=src/device/
KERNEL_DOITGEN_NOOPT_SUBROUTE=ker_doitgen_noOpt
KERNEL_DOITGEN_NOOPT_NAME=makeKer_doitgen_noOpt.mk
KERNEL_DOITGEN_OPT_SUBROUTE=ker_doitgen_Opt
KERNEL_DOITGEN_OPT_NAME=makeKer_doitgen_Opt.mk

KERNEL_CHOLESKY_NOOPT_SUBROUTE=ker_cholesky_noOpt
KERNEL_CHOLESKY_NOOPT_NAME=makeKer_cholesky_noOpt.mk


CONFIGFILE_ROUTE=../../config/configBenchmark_exec1.json
SCRIPTS_ROUTE=cmake/scripts/

DEPENDENCIES_SCRIPTS_ROUTE=cmake/scripts/installDependencies.sh


.phony: clean build


#**************************************
# COMMANDS FOR DOITGEN KERNELS COMPILE
#**************************************
kerDoitgen_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_NOOPT_NAME) TARGET=sw_emu build;
	@cd $(CURRENT_DIR);
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT_NAME) TARGET=sw_emu build; \
	
kerDoitgen_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_NOOPT_NAME) TARGET=hw_emu build;
	@cd $(CURRENT_DIR);
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT_NAME) TARGET=hw_emu build; \
	
kerDoitgen_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_NOOPT_NAME) TARGET=hw build;
	@cd $(CURRENT_DIR);
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT_NAME) TARGET=hw build; \



#**************************************
# COMMANDS FOR CHOLESKY KERNELS COMPILE
#**************************************
kerCholesky_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_NOOPT_NAME) TARGET=sw_emu build;
	
kerCholesky_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_NOOPT_NAME) TARGET=hw_emu build;
	
kerCholesky_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_NOOPT_NAME) TARGET=hw build;



#**************************************
# COMMANDS FOR SOFTWARE COMPILE
#**************************************

hostBuild_sw_emu:
	cd $(BUILD_DIRECTORY); \
	echo '#!/bin/sh' > export_env.sh; \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1; \
	cmake -DTARGET=sw_emu $(CMAKE_DIRECTORY); \
	make -j

hostBuild_hw_emu:
	@cd $(BUILD_DIRECTORY); \
	echo '#!/bin/sh' > export_env.sh; \
	echo 'export XCL_EMULATION_MODE="hw_emu"' >> export_env.sh; \
	chmod +x export_env.sh; \
	source ./export_env.sh; \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1; \
	make -j

hostBuild_hw:
	cd $(BUILD_DIRECTORY); \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1; \
	cmake -DTARGET=hw $(CMAKE_DIRECTORY); \
	make -j

run:
	@cd $(BUILD_DIRECTORY); \
	./benchmarkExec $(CONFIGFILE_ROUTE); \


#**************************************
# OTHER COMMANDS
#**************************************
clean_host:
	rm -rf build/hostBuild/*
	rm -rf build/hostBuild/.run
	rm -rf build/hostBuild/.Xil

clean_kernels:
	rm -rf build/kernelBuild/*
	rm -rf build/kernelBuild/.run
	rm -rf build/kernelBuild/.Xil

clean_out:
	rm -rf out/*

install_dependencies:
	./$(DEPENDENCIES_SCRIPTS_ROUTE)