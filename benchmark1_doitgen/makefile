TYPEDATA=FP	#Posible values: INT, FP
BUILD_DIRECTORY=build/hostBuild
KERNELS_ROUTE=src/core/
KERNEL_DOITGEN_NOOPT_SUBROUTE=ker_doitgen_noOpt
KERNEL_DOITGEN_NOOPT_NAME=makeKer_Doitgen_noOpt.mk

LIBS = -ljsoncpp

.phony: clean build


#**************************************
# COMMANDS FOR KERNELS COMPILE
#**************************************
kernelBuild_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_NOOPT_NAME) TARGET=sw_emu build
	
kernelBuild_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_NOOPT_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_NOOPT_NAME) TARGET=hw_emu build



#**************************************
# COMMANDS FOR SOFTWARE COMPILE
#**************************************
#config_sw_emu:
#	@cd ${BUILD_DIRECTORY}; \
#	export XCL_EMULATION_MODE=sw_emu; \
#	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1
#
#config_hw_emu:
#	@cd ${BUILD_DIRECTORY}; \
#	export XCL_EMULATION_MODE=hw_emu; \
#	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1


hostBuild_sw_emu:
	@cd $(BUILD_DIRECTORY); \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1
	cmake -DTYPEDATA=${TYPEDATA} ../..; \
	make -j

hostBuild_hw_emu:
	@cd $(BUILD_DIRECTORY); \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1
	cmake -DTYPEDATA=${TYPEDATA} ../..; \
	make -j

doitgenRun:
	@cd $(BUILD_DIRECTORY); \
	./doitgen


#**************************************
# OTHER COMMANDS
#**************************************
cleanHost:
	rm -rf build/hostBuild/*
	rm -rf build/hostBuild/.run
	rm -rf build/hostBuild/.Xil

cleanKernels:
	rm -rf build/kernelBuild/*
	rm -rf build/kernelBuild/.run
	rm -rf build/kernelBuild/.Xil
