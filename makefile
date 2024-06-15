CURRENT_DIR := $(shell pwd)
BUILD_DIRECTORY=build/hostBuild
CMAKE_DIRECTORY=../../cmake
SCRIPTS_ROUTE=cmake/scripts/
DEPENDENCIES_SCRIPTS_ROUTE=cmake/scripts/installDependencies.sh

JOBS = 6
REPLACE_FILES = true
CONFIGFILE_ROUTE=../../config/configTest_jacobi2d_sw_emu.json


#Kernel url specification - START
#***********************************
KERNELS_ROUTE=src/device/

KERNEL_CHOLESKY_OPT0_SUBROUTE=ker_cholesky/ker_cholesky_Opt0
KERNEL_CHOLESKY_OPT0_NAME=makeKer_cholesky_Opt0.mk
KERNEL_CHOLESKY_OPT1_SUBROUTE=ker_cholesky/ker_cholesky_Opt1
KERNEL_CHOLESKY_OPT1_NAME=makeKer_cholesky_Opt1.mk
KERNEL_CHOLESKY_OPT2_SUBROUTE=ker_cholesky/ker_cholesky_Opt2
KERNEL_CHOLESKY_OPT2_NAME=makeKer_cholesky_Opt2.mk

KERNEL_DOITGEN_OPT0_SUBROUTE=ker_doitgen/ker_doitgen_Opt0
KERNEL_DOITGEN_OPT0_NAME=makeKer_doitgen_Opt0.mk
KERNEL_DOITGEN_OPT1_SUBROUTE=ker_doitgen/ker_doitgen_Opt1
KERNEL_DOITGEN_OPT1_NAME=makeKer_doitgen_Opt1.mk
KERNEL_DOITGEN_OPT2_SUBROUTE=ker_doitgen/ker_doitgen_Opt2
KERNEL_DOITGEN_OPT2_NAME=makeKer_doitgen_Opt2.mk

KERNEL_GEMM_OPT0_SUBROUTE=ker_gemm/ker_gemm_Opt0
KERNEL_GEMM_OPT0_NAME=makeKer_gemm_Opt0.mk
KERNEL_GEMM_OPT1_SUBROUTE=ker_gemm/ker_gemm_Opt1
KERNEL_GEMM_OPT1_NAME=makeKer_gemm_Opt1.mk
KERNEL_GEMM_OPT2_SUBROUTE=ker_gemm/ker_gemm_Opt2
KERNEL_GEMM_OPT2_NAME=makeKer_gemm_Opt2.mk

KERNEL_GEMMPER_OPT0_SUBROUTE=ker_gemmPer/ker_gemmPer_Opt0
KERNEL_GEMMPER_OPT0_NAME=makeKer_gemmPer_Opt0.mk
KERNEL_GEMMPER_OPT1_SUBROUTE=ker_gemmPer/ker_gemmPer_Opt1
KERNEL_GEMMPER_OPT1_NAME=makeKer_gemmPer_Opt1.mk
KERNEL_GEMMPER_OPT2_SUBROUTE=ker_gemmPer/ker_gemmPer_Opt2
KERNEL_GEMMPER_OPT2_NAME=makeKer_gemmPer_Opt2.mk

KERNEL_JACOBI2D_OPT0_SUBROUTE=ker_jacobi2d/ker_jacobi2d_Opt0
KERNEL_JACOBI2D_OPT0_NAME=makeKer_jacobi2d_Opt0.mk
KERNEL_JACOBI2D_OPT1_SUBROUTE=ker_jacobi2d/ker_jacobi2d_Opt1
KERNEL_JACOBI2D_OPT1_NAME=makeKer_jacobi2d_Opt1.mk
KERNEL_JACOBI2D_OPT2_SUBROUTE=ker_jacobi2d/ker_jacobi2d_Opt2
KERNEL_JACOBI2D_OPT2_NAME=makeKer_jacobi2d_Opt.mk
#***********************************
#Kernel url specification - END




.phony: clean build

#**********************************************
# COMMANDS FOR CHOLESKY KERNELS COMPILE - START
#**********************************************
# KERNEL TARGET sw_emu
#*********************************
kerCholesky_Opt0_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT0_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_OPT0_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerCholesky_Opt1_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT1_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_OPT1_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerCholesky_Opt2_build_sw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT2_SUBROUTE}; \
#make -f $(KERNEL_CHOLESKY_OPT2_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerCholesky_All_build_sw_emu: kerCholesky_Opt0_build_sw_emu kerCholesky_Opt1_build_sw_emu kerCholesky_Opt2_build_sw_emu


# KERNEL TARGET hw_emu
#*********************************
kerCholesky_Opt0_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT0_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_OPT0_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerCholesky_Opt1_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT1_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_OPT1_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerCholesky_Opt2_build_hw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT2_SUBROUTE}; \
#make -f $(KERNEL_CHOLESKY_OPT2_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerCholesky_All_build_hw_emu: kerCholesky_Opt0_build_hw_emu kerCholesky_Opt1_build_hw_emu kerCholesky_Opt2_build_hw_emu


# KERNEL TARGET hw
#*********************************
kerCholesky_Opt0_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT0_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_OPT0_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerCholesky_Opt1_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT1_SUBROUTE}; \
	make -f $(KERNEL_CHOLESKY_OPT1_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerCholesky_Opt2_build_hw:
#@cd ${KERNELS_ROUTE}${KERNEL_CHOLESKY_OPT2_SUBROUTE}; \
#make -f $(KERNEL_CHOLESKY_OPT2_NAME) TARGET=hw JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerCholesky_All_build_hw: kerCholesky_Opt0_build_hw kerCholesky_Opt1_build_hw kerCholesky_Opt2_build_hw
#**********************************************
# COMMANDS FOR CHOLESKY KERNELS COMPILE - END
#**********************************************





#*********************************************
# COMMANDS FOR DOITGEN KERNELS COMPILE - START
#*********************************************
# KERNEL TARGET sw_emu
#*********************************
kerDoitgen_Opt0_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT0_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT0_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerDoitgen_Opt1_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT1_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT1_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerDoitgen_Opt2_build_sw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT2_SUBROUTE}; \
#make -f $(KERNEL_DOITGEN_OPT2_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerDoitgen_All_build_sw_emu: kerDoitgen_Opt0_build_sw_emu kerDoitgen_Opt1_build_sw_emu kerDoitgen_Opt2_build_sw_emu


# KERNEL TARGET hw_emu
#*********************************
kerDoitgen_Opt0_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT0_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT0_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerDoitgen_Opt1_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT1_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT1_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerDoitgen_Opt2_build_hw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT2_SUBROUTE}; \
#make -f $(KERNEL_DOITGEN_OPT2_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerDoitgen_All_build_hw_emu: kerDoitgen_Opt0_build_hw_emu kerDoitgen_Opt1_build_hw_emu kerDoitgen_Opt2_build_hw_emu


# KERNEL TARGET hw
#*********************************
kerDoitgen_Opt0_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT0_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT0_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerDoitgen_Opt1_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT1_SUBROUTE}; \
	make -f $(KERNEL_DOITGEN_OPT1_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerDoitgen_Opt2_build_hw:
#@cd ${KERNELS_ROUTE}${KERNEL_DOITGEN_OPT2_SUBROUTE}; \
#make -f $(KERNEL_DOITGEN_OPT2_NAME) TARGET=hw JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerDoitgen_All_build_hw: kerDoitgen_Opt0_build_hw kerDoitgen_Opt1_build_hw kerDoitgen_Opt2_build_hw
#*********************************************
# COMMANDS FOR DOITGEN KERNELS COMPILE - END
#*********************************************





#******************************************
# COMMANDS FOR GEMM KERNELS COMPILE - START
#******************************************
# KERNEL TARGET sw_emu
#*********************************
kerGemm_Opt0_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT0_SUBROUTE}; \
	make -f $(KERNEL_GEMM_OPT0_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerGemm_Opt1_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT1_SUBROUTE}; \
	make -f $(KERNEL_GEMM_OPT1_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerGemm_Opt2_build_sw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT2_SUBROUTE}; \
#make -f $(KERNEL_GEMM_OPT2_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerGemm_All_build_sw_emu: kerGemm_Opt0_build_sw_emu kerGemm_Opt1_build_sw_emu kerGemm_Opt2_build_sw_emu


# KERNEL TARGET hw_emu
#*********************************
kerGemm_Opt0_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT0_SUBROUTE}; \
	make -f $(KERNEL_GEMM_OPT0_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerGemm_Opt1_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT1_SUBROUTE}; \
	make -f $(KERNEL_GEMM_OPT1_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerGemm_Opt2_build_hw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT2_SUBROUTE}; \
#make -f $(KERNEL_GEMM_OPT2_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerGemm_All_build_hw_emu: kerGemm_Opt0_build_hw_emu kerGemm_Opt1_build_hw_emu kerGemm_Opt2_build_hw_emu


# KERNEL TARGET hw
#*********************************
kerGemm_Opt0_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT0_SUBROUTE}; \
	make -f $(KERNEL_GEMM_OPT0_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerGemm_Opt1_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT1_SUBROUTE}; \
	make -f $(KERNEL_GEMM_OPT1_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerGemm_Opt2_build_hw:
#@cd ${KERNELS_ROUTE}${KERNEL_GEMM_OPT2_SUBROUTE}; \
#make -f $(KERNEL_GEMM_OPT2_NAME) TARGET=hw JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerGemm_All_build_hw: kerGemm_Opt0_build_hw kerGemm_Opt1_build_hw kerGemm_Opt2_build_hw
#******************************************
# COMMANDS FOR GEMM KERNELS COMPILE - END
#******************************************





#**********************************************
# COMMANDS FOR GEMM_PER KERNELS COMPILE - START
#**********************************************
# KERNEL TARGET sw_emu
#*********************************
kerGemmPer_Opt0_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT0_SUBROUTE}; \
	make -f $(KERNEL_GEMMPER_OPT0_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerGemmPer_Opt1_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT1_SUBROUTE}; \
	make -f $(KERNEL_GEMMPER_OPT1_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerGemmPer_Opt2_build_sw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT2_SUBROUTE}; \
#make -f $(KERNEL_GEMMPER_OPT2_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerGemmPer_All_build_sw_emu: kerGemmPer_Opt0_build_sw_emu kerGemmPer_Opt1_build_sw_emu kerGemmPer_Opt2_build_sw_emu


# KERNEL TARGET hw_emu
#*********************************
kerGemmPer_Opt0_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT0_SUBROUTE}; \
	make -f $(KERNEL_GEMMPER_OPT0_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerGemmPer_Opt1_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT1_SUBROUTE}; \
	make -f $(KERNEL_GEMMPER_OPT1_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerGemmPer_Opt2_build_hw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT2_SUBROUTE}; \
#make -f $(KERNEL_GEMMPER_OPT2_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerGemmPer_All_build_hw_emu: kerGemmPer_Opt0_build_hw_emu kerGemmPer_Opt1_build_hw_emu kerGemmPer_Opt2_build_hw_emu


# KERNEL TARGET hw
#*********************************
kerGemmPer_Opt0_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT0_SUBROUTE}; \
	make -f $(KERNEL_GEMMPER_OPT0_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerGemmPer_Opt1_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT1_SUBROUTE}; \
	make -f $(KERNEL_GEMMPER_OPT1_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerGemmPer_Opt2_build_hw:
#@cd ${KERNELS_ROUTE}${KERNEL_GEMMPER_OPT2_SUBROUTE}; \
#make -f $(KERNEL_GEMMPER_OPT2_NAME) TARGET=hw JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerGemmPer_All_build_hw: kerGemmPer_Opt0_build_hw kerGemmPer_Opt1_build_hw kerGemmPer_Opt2_build_hw
#**********************************************
# COMMANDS FOR GEMM_PER KERNELS COMPILE - END
#**********************************************





#***********************************************
# COMMANDS FOR JACOBI_2D KERNELS COMPILE - START
#***********************************************
# KERNEL TARGET sw_emu
#*********************************
kerJacobi2d_Opt0_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT0_SUBROUTE}; \
	make -f $(KERNEL_JACOBI2D_OPT0_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerJacobi2d_Opt1_build_sw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT1_SUBROUTE}; \
	make -f $(KERNEL_JACOBI2D_OPT1_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerJacobi2d_Opt2_build_sw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT2_SUBROUTE}; \
#make -f $(KERNEL_JACOBI2D_OPT2_NAME) TARGET=sw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerJacobi2d_All_build_sw_emu: kerJacobi2d_Opt0_build_sw_emu kerJacobi2d_Opt1_build_sw_emu kerJacobi2d_Opt2_build_sw_emu


# KERNEL TARGET hw_emu
#*********************************
kerJacobi2d_Opt0_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT0_SUBROUTE}; \
	make -f $(KERNEL_JACOBI2D_OPT0_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerJacobi2d_Opt1_build_hw_emu:
	@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT1_SUBROUTE}; \
	make -f $(KERNEL_JACOBI2D_OPT1_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerJacobi2d_Opt2_build_hw_emu:
#@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT2_SUBROUTE}; \
#make -f $(KERNEL_JACOBI2D_OPT2_NAME) TARGET=hw_emu JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerJacobi2d_All_build_hw_emu: kerJacobi2d_Opt0_build_hw_emu kerJacobi2d_Opt1_build_hw_emu kerJacobi2d_Opt2_build_hw_emu


# KERNEL TARGET hw
#*********************************
kerJacobi2d_Opt0_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT0_SUBROUTE}; \
	make -f $(KERNEL_JACOBI2D_OPT0_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);
	
kerJacobi2d_Opt1_build_hw:
	@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT1_SUBROUTE}; \
	make -f $(KERNEL_JACOBI2D_OPT1_NAME) TARGET=hw JOBS=$(JOBS) build;
	@cd $(CURRENT_DIR);

kerJacobi2d_Opt2_build_hw:
#@cd ${KERNELS_ROUTE}${KERNEL_JACOBI2D_OPT2_SUBROUTE}; \
#make -f $(KERNEL_JACOBI2D_OPT2_NAME) TARGET=hw JOBS=$(JOBS) build;
#@cd $(CURRENT_DIR);

kerJacobi2d_All_build_hw: kerJacobi2d_Opt0_build_hw kerJacobi2d_Opt1_build_hw kerJacobi2d_Opt2_build_hw
#***********************************************
# COMMANDS FOR JACOBI_2D KERNELS COMPILE - START
#***********************************************



#**************************************
# COMMANDS FOR ALL KERNELS COMPILE
#**************************************
kerAll_build_sw_emu: kerCholesky_Opt0_build_sw_emu kerDoitgen_Opt0_build_sw_emu kerGemm_Opt0_build_sw_emu kerGemmPer_Opt0_build_sw_emu kerJacobi2d_Opt0_build_sw_emu
	
kerAll_build_hw_emu: kerCholesky_Opt0_build_hw_emu kerDoitgen_Opt0_build_hw_emu kerGemm_Opt0_build_hw_emu kerGemmPer_Opt0_build_hw_emu kerJacobi2d_Opt0_build_hw_emu
	
kerAll_build_hw: kerCholesky_Opt0_build_hw kerDoitgen_Opt0_build_hw kerGemm_Opt0_build_hw kerGemmPer_Opt0_build_hw kerJacobi2d_Opt0_build_hw


#kerAll_build_hw_custom: kerJacobi2d_build_hw kerGemmPer_build_hw 



#**************************************
# COMMANDS FOR SOFTWARE COMPILE
#**************************************
hostBuild_sw_emu:
	cd $(BUILD_DIRECTORY); \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1; \
	cmake -DTARGET=sw_emu -DREPLACE_FILES=${REPLACE_FILES} $(CMAKE_DIRECTORY); \
	make -j

hostBuild_hw_emu:
	@cd $(BUILD_DIRECTORY); \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1; \
	cmake -DTARGET=hw_emu -DREPLACE_FILES=${REPLACE_FILES} $(CMAKE_DIRECTORY); \
	make -j

hostBuild_hw:
	cd $(BUILD_DIRECTORY); \
	emconfigutil --platform xilinx_u200_gen3x16_xdma_2_202110_1; \
	cmake -DTARGET=hw -DREPLACE_FILES=${REPLACE_FILES} $(CMAKE_DIRECTORY); \
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