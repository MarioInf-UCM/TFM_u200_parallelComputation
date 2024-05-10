TYPEDATA=FP	#Posible values: INT, FP
BUILD_DIRECTORY=build/hostBuild
KERNELS_DIRECTORY=src/core

.phony: clean build


#COMMANDS FOR KERNELS COMPILE
#**************************************
makeKernels_noOptimization_build:
	@cd ${KERNELS_DIRECTORY}; \
	make -f makefile_Doitgen_noOptimization.mk TYPEDATA=${TYPEDATA} build

makeKernels_clean:
	@cd ${KERNELS_DIRECTORY};  \
	make -f makeKernels.mk clean


#COMMANDS FOR SOFTWARE COMPILE
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



cleanHost:
	rm -rf build/hostBuild/*
	rm -rf build/hostBuild/.run
	rm -rf build/hostBuild/.Xil

cleanKernels:
	rm -rf build/kernelBuild/*
	rm -rf build/kernelBuild/.run
	rm -rf build/kernelBuild/.Xil
