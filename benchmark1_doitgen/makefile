TYPEDATA=FP	#Posible values: INT, FP
BUILD_DIRECTORY=build
KERNELS_DIRECTORY=src/core

.phony: clean build


#COMMANDS FOR KERNELS COMPILE
#**************************************
makeKernels_build:
	@cd ${KERNELS_DIRECTORY}; \
	make -f makeKernels.mk TYPEDATA=${TYPEDATA} build

makeKernels_clean:
	@cd ${KERNELS_DIRECTORY};  \
	make -f makeKernels.mk clean


#COMMANDS FOR SOFTWARE COMPILE
#**************************************
config_sw_emu:
	@cd ${BUILD_DIRECTORY}; \
	export XCL_EMULATION_MODE=sw_emu; \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1

config_hw_emu:
	@cd ${BUILD_DIRECTORY}; \
	export XCL_EMULATION_MODE=hw_emu; \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1



doitgen:
	@cd build; \
	cmake -DTYPEDATA=${TYPEDATA} ..; \
	make -j

doitgenRun:
	@cd build; \
	./doitgen

clean:
	rm -rf build/*
	rm -rf build/.run
	rm -rf build/.Xil
