#**********************
#* CHECK ENTRY PARAMS *
#**********************
ifeq ($(TARGET),sw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_doitgen_noOpt/sw_emu/
else ifeq ($(TARGET),hw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_doitgen_noOpt/hw_emu/
else ifeq ($(TARGET),hw)
	KERNEL_BUILD_SUBFOLDER := ker_doitgen_noOpt/hw/
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

VPPLFLAGS := --jobs $(JOBS) --config $(CONNECTIVITY_ROUTE)/profile.ini
VPPLFLAGS += --config $(BOARD_CONFIG)
VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g
CUSTOMFLAGS :=




#********************************************
#* COMPILATION OF kerDoitgen_noOptimization *
#********************************************
MAKEFILE_NAME := makeKer_Doitgen_noOpt.mk


ker_doitgen_noOpt_XOS := ker_doitgen_noOpt_mini.xo
ker_doitgen_noOpt_XOS += ker_doitgen_noOpt_small.xo
ker_doitgen_noOpt_XOS += ker_doitgen_noOpt_medium.xo
ker_doitgen_noOpt_XOS += ker_doitgen_noOpt_large.xo
ker_doitgen_noOpt_XOS += ker_doitgen_noOpt_extralarge.xo


ker_doitgen_noOptimization_FOLDER := ./

ker_doitgen_noOpt_mini_KERNEL := ker_doitgen_noOpt_mini
ker_doitgen_noOpt_mini_SRC := ker_doitgen_noOpt_mini.cpp
ker_doitgen_noOpt_mini_SRC_ROUTE := $(ker_doitgen_noOptimization_FOLDER)ker_doitgen_noOpt_mini.cpp
ker_doitgen_noOpt_mini_HEADER :=
ker_doitgen_noOpt_mini_HEADER_ROUTE :=

ker_doitgen_noOpt_small_KERNEL := ker_doitgen_noOpt_small
ker_doitgen_noOpt_small_SRC := ker_doitgen_noOpt_small.cpp
ker_doitgen_noOpt_small_SRC_ROUTE := $(ker_doitgen_noOptimization_FOLDER)ker_doitgen_noOpt_small.cpp
ker_doitgen_noOpt_small_HEADER :=
ker_doitgen_noOpt_small_HEADER_ROUTE :=

ker_doitgen_noOpt_medium_KERNEL := ker_doitgen_noOpt_medium
ker_doitgen_noOpt_medium_SRC := ker_doitgen_noOpt_medium.cpp
ker_doitgen_noOpt_medium_SRC_ROUTE := $(ker_doitgen_noOptimization_FOLDER)ker_doitgen_noOpt_medium.cpp
ker_doitgen_noOpt_medium_HEADER :=
ker_doitgen_noOpt_medium_HEADER_ROUTE :=

ker_doitgen_noOpt_large_KERNEL := ker_doitgen_noOpt_large
ker_doitgen_noOpt_large_SRC := ker_doitgen_noOpt_large.cpp
ker_doitgen_noOpt_large_SRC_ROUTE := $(ker_doitgen_noOptimization_FOLDER)ker_doitgen_noOpt_large.cpp
ker_doitgen_noOpt_large_HEADER :=
ker_doitgen_noOpt_large_HEADER_ROUTE :=

ker_doitgen_noOpt_extralarge_KERNEL := ker_doitgen_noOpt_extralarge
ker_doitgen_noOpt_extralarge_SRC := ker_doitgen_noOpt_extralarge.cpp
ker_doitgen_noOpt_extralarge_SRC_ROUTE := $(ker_doitgen_noOptimization_FOLDER)ker_doitgen_noOpt_extralarge.cpp
ker_doitgen_noOpt_extralarge_HEADER :=
ker_doitgen_noOpt_extralarge_HEADER_ROUTE :=



#********************
#* GOALS DEFINITION *
#********************
build: copy_files

copy_files:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
	cp -r ${CONNECTIVITY_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}/${CONNECTIVITY_ROUTE}
	cp ${ker_doitgen_noOpt_mini_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
	cp ${ker_doitgen_noOpt_small_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
	cp ${ker_doitgen_noOpt_medium_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
	cp ${ker_doitgen_noOpt_large_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
	cp ${ker_doitgen_noOpt_extralarge_SRC_ROUTE} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
	cp ${MAKEFILE_NAME} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}
ifeq ($(TARGET),sw_emu)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_doitgen_noOpt_sw_emu.xclbin
else ifeq ($(TARGET),hw_emu)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_doitgen_noOpt_hw_emu.xclbin
else ifeq ($(TARGET),hw)
	cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
	make -f $(MAKEFILE_NAME) TARGET=$(TARGET) kerPack_doitgen_noOpt_hw.xclbin
endif



kerPack_doitgen_noOpt_sw_emu.xclbin: $(ker_doitgen_noOpt_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMFLAGS) -o $@ $(ker_doitgen_noOpt_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_doitgen_noOpt_hw_emu.xclbin: $(ker_doitgen_noOpt_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMFLAGS) -o $@ $(ker_doitgen_noOpt_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_doitgen_noOpt_hw.xclbin: $(ker_doitgen_noOpt_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMFLAGS) -o $@ $(ker_doitgen_noOpt_XOS) --remote_ip_cache ${IP_CACHE_DIR}



ker_doitgen_noOpt_mini.xo: $(ker_doitgen_noOpt_mini_SRC) $(ker_doitgen_noOpt_mini_HEADER)
	v++ --kernel $(ker_doitgen_noOpt_mini_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(ker_doitgen_noOpt_mini_SRC) $<

ker_doitgen_noOpt_small.xo: $(ker_doitgen_noOpt_small_SRC) $(ker_doitgen_noOpt_small_HEADER)
	v++ --kernel $(ker_doitgen_noOpt_small_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(ker_doitgen_noOpt_small_SRC) $<

ker_doitgen_noOpt_medium.xo: $(ker_doitgen_noOpt_medium_SRC) $(ker_doitgen_noOpt_medium_HEADER)
	v++ --kernel $(ker_doitgen_noOpt_medium_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(ker_doitgen_noOpt_medium_SRC) $<

ker_doitgen_noOpt_large.xo: $(ker_doitgen_noOpt_large_SRC) $(ker_doitgen_noOpt_large_HEADER)
	v++ --kernel $(ker_doitgen_noOpt_large_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(ker_doitgen_noOpt_large_SRC) $<

ker_doitgen_noOpt_extralarge.xo: $(ker_doitgen_noOpt_extralarge_SRC) $(ker_doitgen_noOpt_extralarge_HEADER)
	v++ --kernel $(ker_doitgen_noOpt_extralarge_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(ker_doitgen_noOpt_extralarge_SRC) $<