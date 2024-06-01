#**********************
#* CHECK ENTRY PARAMS *
#**********************
ifeq ($(TARGET),sw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_gemm_per_noOpt/sw_emu/
else ifeq ($(TARGET),hw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_gemm_per_noOpt/hw_emu/
else ifeq ($(TARGET),hw)
	KERNEL_BUILD_SUBFOLDER := ker_gemm_per_noOpt/hw/
else
    @echo "Incorret value of TARGET. Please, try again using: make -f <makeFileName> TARGET=sw_emu|he_emu|hw"
    $(error "Incorret value of TARGET. Please, try again using: make -f <makeFileName> TARGET=sw_emu|he_emu|hw")
	exit -1
endif


#*****************************
#* CONFIG COMPULATION PARAMS *
#*****************************
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms
PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)

BUILD_DIR := ../../../build/kernelBuild
CONNECTIVITY_ROUTE := ../connectivityConfig
BOARD_CONFIG := $(CONNECTIVITY_ROUTE)/connectivity_u200.ini
LINK_OPTS := --config $(CONNECTIVITY_ROUTE)/connectivity_u200.ini -I ./src/kernel
IP_CACHE_DIR ?= ip_cache

VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g --hls.jobs $(JOBS)
VPPLFLAGS := --config $(CONNECTIVITY_ROUTE)/profile.ini --config $(BOARD_CONFIG)
CUSTOMPARAMS_GENERAL :=




#********************************************
#* COMPILATION OF kergemm_per_noOptimization *
#********************************************
MAKEFILE_NAME := makeKer_gemm_per_noOpt.mk

ker_gemm_per_noOpt_XOS := ker_gemm_per_noOpt_mini.xo
ker_gemm_per_noOpt_XOS += ker_gemm_per_noOpt_small.xo
ker_gemm_per_noOpt_XOS += ker_gemm_per_noOpt_medium.xo
ker_gemm_per_noOpt_XOS += ker_gemm_per_noOpt_large.xo
ker_gemm_per_noOpt_XOS += ker_gemm_per_noOpt_extralarge.xo

ker_gemm_per_noOpt_mini_KERNEL := ker_gemm_per_noOpt_mini
ker_gemm_per_noOpt_small_KERNEL := ker_gemm_per_noOpt_small
ker_gemm_per_noOpt_medium_KERNEL := ker_gemm_per_noOpt_medium
ker_gemm_per_noOpt_large_KERNEL := ker_gemm_per_noOpt_large
ker_gemm_per_noOpt_extralarge_KERNEL := ker_gemm_per_noOpt_extralarge


ker_gemm_per_noOpt_mini_CUSTOMPARAMS := -DMINI_DATASET
ker_gemm_per_noOpt_small_CUSTOMPARAMS := -DSMALL_DATASET
ker_gemm_per_noOpt_medium_CUSTOMPARAMS := -DMEDIUM_DATASET
ker_gemm_per_noOpt_large_CUSTOMPARAMS := -DLARGE_DATASET
ker_gemm_per_noOpt_extralarge_CUSTOMPARAMS := -DEXTRALARGE_DATASET


ker_gemm_per_noOpt_FOLDER := ./
ker_gemm_per_noOpt_SRC := ker_gemm_per_noOpt.cpp
ker_gemm_per_noOpt_SRC_ROUTE := $(ker_gemm_per_noOpt_FOLDER)ker_gemm_per_noOpt.cpp
ker_gemm_per_noOpt_HEADER :=
ker_gemm_per_noOpt_HEADER_ROUTE :=


#********************
#* GOALS DEFINITION *
#********************
build: copy_files

copy_files:
	mkdir -p ${BUILD_DIR};
	mkdir -p ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp -r ${CONNECTIVITY_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}/${CONNECTIVITY_ROUTE};
	cp ${ker_gemm_per_noOpt_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp ${MAKEFILE_NAME} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
ifeq ($(TARGET),sw_emu)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_gemm_per_noOpt_sw_emu.xclbin
else ifeq ($(TARGET),hw_emu)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_gemm_per_noOpt_hw_emu.xclbin
else ifeq ($(TARGET),hw)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_gemm_per_noOpt_hw.xclbin
endif



kerPack_gemm_per_noOpt_sw_emu.xclbin: $(ker_gemm_per_noOpt_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemm_per_noOpt_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemm_per_noOpt_hw_emu.xclbin: $(ker_gemm_per_noOpt_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemm_per_noOpt_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemm_per_noOpt_hw.xclbin: $(ker_gemm_per_noOpt_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemm_per_noOpt_XOS) --remote_ip_cache ${IP_CACHE_DIR}



ker_gemm_per_noOpt_mini.xo: $(ker_gemm_per_noOpt_SRC) $(ker_gemm_per_noOpt_HEADER)
	v++ --kernel $(ker_gemm_per_noOpt_mini_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemm_per_noOpt_mini_CUSTOMPARAMS) -c -o $@ $(ker_gemm_per_noOpt_SRC) $<

ker_gemm_per_noOpt_small.xo: $(ker_gemm_per_noOpt_SRC) $(ker_gemm_per_noOpt_HEADER)
	v++ --kernel $(ker_gemm_per_noOpt_small_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemm_per_noOpt_small_CUSTOMPARAMS) -c -o $@ $(ker_gemm_per_noOpt_SRC) $<

ker_gemm_per_noOpt_medium.xo: $(ker_gemm_per_noOpt_SRC) $(ker_gemm_per_noOpt_HEADER)
	v++ --kernel $(ker_gemm_per_noOpt_medium_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemm_per_noOpt_medium_CUSTOMPARAMS) -c -o $@ $(ker_gemm_per_noOpt_SRC) $<

ker_gemm_per_noOpt_large.xo: $(ker_gemm_per_noOpt_SRC) $(ker_gemm_per_noOpt_HEADER)
	v++ --kernel $(ker_gemm_per_noOpt_large_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemm_per_noOpt_large_CUSTOMPARAMS) -c -o $@ $(ker_gemm_per_noOpt_SRC) $<

ker_gemm_per_noOpt_extralarge.xo: $(ker_gemm_per_noOpt_SRC) $(ker_gemm_per_noOpt_HEADER)
	v++ --kernel $(ker_gemm_per_noOpt_extralarge_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemm_per_noOpt_extralarge_CUSTOMPARAMS) -c -o $@ $(ker_gemm_per_noOpt_SRC) $<