#**********************
#* CHECK ENTRY PARAMS *
#**********************
ifeq ($(TARGET),sw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_doitgen_Opt2/sw_emu/
else ifeq ($(TARGET),hw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_doitgen_Opt2/hw_emu/
else ifeq ($(TARGET),hw)
	KERNEL_BUILD_SUBFOLDER := ker_doitgen_Opt2/hw/
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

BUILD_DIR := ../../../../build/kernelBuild
CONNECTIVITY_ROUTE := ../doitgen_connectivityConfig
BOARD_CONFIG := $(CONNECTIVITY_ROUTE)/connectivity_u200.ini
LINK_Opt2S := --config $(CONNECTIVITY_ROUTE)/connectivity_u200.ini -I ./src/kernel
IP_CACHE_DIR ?= ip_cache

VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g --hls.jobs $(JOBS) --vivado.synth.jobs $(JOBS) --vivado.impl.jobs $(JOBS)
VPPLFLAGS := --config $(CONNECTIVITY_ROUTE)/profile.ini --config $(BOARD_CONFIG)
CUSTOMPARAMS_GENERAL :=




#********************************************
#* COMPILATION OF ker_doitgen_Opt2 *
#********************************************
MAKEFILE_NAME := makeKer_doitgen_Opt2.mk

ker_doitgen_Opt2_XOS := ker_doitgen_Opt2_mini.xo
ker_doitgen_Opt2_XOS += ker_doitgen_Opt2_small.xo
ker_doitgen_Opt2_XOS += ker_doitgen_Opt2_medium.xo
ker_doitgen_Opt2_XOS += ker_doitgen_Opt2_large.xo
ker_doitgen_Opt2_XOS += ker_doitgen_Opt2_extralarge.xo

ker_doitgen_Opt2_mini_KERNEL := ker_doitgen_Opt2_mini
ker_doitgen_Opt2_small_KERNEL := ker_doitgen_Opt2_small
ker_doitgen_Opt2_medium_KERNEL := ker_doitgen_Opt2_medium
ker_doitgen_Opt2_large_KERNEL := ker_doitgen_Opt2_large
ker_doitgen_Opt2_extralarge_KERNEL := ker_doitgen_Opt2_extralarge


ker_doitgen_Opt2_mini_CUSTOMPARAMS := -DMINI_DATASET
ker_doitgen_Opt2_small_CUSTOMPARAMS := -DSMALL_DATASET
ker_doitgen_Opt2_medium_CUSTOMPARAMS := -DMEDIUM_DATASET
ker_doitgen_Opt2_large_CUSTOMPARAMS := -DLARGE_DATASET
ker_doitgen_Opt2_extralarge_CUSTOMPARAMS := -DEXTRALARGE_DATASET


ker_doitgen_Opt2_FOLDER := ./
ker_doitgen_Opt2_SRC := ker_doitgen_Opt2.cpp
ker_doitgen_Opt2_SRC_ROUTE := $(ker_doitgen_Opt2_FOLDER)ker_doitgen_Opt2.cpp
ker_doitgen_Opt2_HEADER :=
ker_doitgen_Opt2_HEADER_ROUTE :=


#********************
#* GOALS DEFINITION *
#********************
build: copy_files

copy_files:
	mkdir -p ${BUILD_DIR};
	mkdir -p ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp -r ${CONNECTIVITY_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}/..;
	cp ${ker_doitgen_Opt2_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp ${MAKEFILE_NAME} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
ifeq ($(TARGET),sw_emu)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_doitgen_Opt2_sw_emu.xclbin
else ifeq ($(TARGET),hw_emu)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_doitgen_Opt2_hw_emu.xclbin
else ifeq ($(TARGET),hw)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_doitgen_Opt2_hw.xclbin
endif



kerPack_doitgen_Opt2_sw_emu.xclbin: $(ker_doitgen_Opt2_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_doitgen_Opt2_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_doitgen_Opt2_hw_emu.xclbin: $(ker_doitgen_Opt2_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_doitgen_Opt2_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_doitgen_Opt2_hw.xclbin: $(ker_doitgen_Opt2_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_doitgen_Opt2_XOS) --remote_ip_cache ${IP_CACHE_DIR}



ker_doitgen_Opt2_mini.xo: $(ker_doitgen_Opt2_SRC) $(ker_doitgen_Opt2_HEADER)
	v++ --kernel $(ker_doitgen_Opt2_mini_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_doitgen_Opt2_mini_CUSTOMPARAMS) -c -o $@ $(ker_doitgen_Opt2_SRC) $<

ker_doitgen_Opt2_small.xo: $(ker_doitgen_Opt2_SRC) $(ker_doitgen_Opt2_HEADER)
	v++ --kernel $(ker_doitgen_Opt2_small_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_doitgen_Opt2_small_CUSTOMPARAMS) -c -o $@ $(ker_doitgen_Opt2_SRC) $<

ker_doitgen_Opt2_medium.xo: $(ker_doitgen_Opt2_SRC) $(ker_doitgen_Opt2_HEADER)
	v++ --kernel $(ker_doitgen_Opt2_medium_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_doitgen_Opt2_medium_CUSTOMPARAMS) -c -o $@ $(ker_doitgen_Opt2_SRC) $<

ker_doitgen_Opt2_large.xo: $(ker_doitgen_Opt2_SRC) $(ker_doitgen_Opt2_HEADER)
	v++ --kernel $(ker_doitgen_Opt2_large_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_doitgen_Opt2_large_CUSTOMPARAMS) -c -o $@ $(ker_doitgen_Opt2_SRC) $<

ker_doitgen_Opt2_extralarge.xo: $(ker_doitgen_Opt2_SRC) $(ker_doitgen_Opt2_HEADER)
	v++ --kernel $(ker_doitgen_Opt2_extralarge_KERNEL) $(VPPFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_doitgen_Opt2_extralarge_CUSTOMPARAMS) -c -o $@ $(ker_doitgen_Opt2_SRC) $<