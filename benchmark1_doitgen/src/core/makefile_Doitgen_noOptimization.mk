#*****************************
#* CONFIG COMPULATION PARAMS *
#*****************************

# Options for TARGET: sw_emu, hw_emu and hw
TARGET ?= sw_emu
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms



PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)

CONNECTIVITY_ROUTE := connectivityConfig
BOARD_CONFIG := $(CONNECTIVITY_ROUTE)/connectivity_u200.ini
LINK_OPTS := --config $(CONNECTIVITY_ROUTE)/connectivity_u200.ini -I ./src/kernel
IP_CACHE_DIR ?= ip_cache

VPPLFLAGS := --jobs $(JOBS) --config $(CONNECTIVITY_ROUTE)/profile.ini
VPPLFLAGS += --config $(BOARD_CONFIG)
VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g

# Defineng CUSTOMFLAGS
ifeq ($(TYPEDATA),INT)
    TYPEDATA_COMPILER = TYPEDATA_INT
else ifeq ($(TYPEDATA),FP)
    TYPEDATA_COMPILER = TYPEDATA_FP
else ifeq ($(TYPEDATA),)
    TYPEDATA = INT
    TYPEDATA_COMPILER = TYPEDATA_INT
else
    $(error "TYPEDATA no tiene un valor válido")
endif

CUSTOMFLAGS := -D$(TYPEDATA_COMPILER)
BUILD_DIR := ../../build/kernelBuild/
MAKEFILE_NAME := makefile_Doitgen_noOptimization.mk

#********************************************
#* COMPILATION OF kerDoitgen_noOptimization *
#********************************************
kerDoitgen_noOptimization_XOS := kerDoitgen_noOptimization_miniDataset.xo
kerDoitgen_noOptimization_XOS += kerDoitgen_noOptimization_smallDataset.xo
kerDoitgen_noOptimization_XOS += kerDoitgen_noOptimization_mediumDataset.xo
kerDoitgen_noOptimization_XOS += kerDoitgen_noOptimization_largeDataset.xo
kerDoitgen_noOptimization_XOS += kerDoitgen_noOptimization_extralargeDataset.xo


kerDoitgen_noOptimization_FOLDER := kerDoitgen_noOptimization/

kerDoitgen_noOptimization_miniDataset_KERNEL := kerDoitgen_noOptimization_miniDataset
kerDoitgen_noOptimization_miniDataset_SRC := kerDoitgen_noOptimization_miniDataset.cpp
kerDoitgen_noOptimization_miniDataset_SRC_ROUTE := $(kerDoitgen_noOptimization_FOLDER)kerDoitgen_noOptimization_miniDataset.cpp
kerDoitgen_noOptimization_miniDataset_HEADER :=
kerDoitgen_noOptimization_miniDataset_HEADER_ROUTE :=

kerDoitgen_noOptimization_smallDataset_KERNEL := kerDoitgen_noOptimization_smallDataset
kerDoitgen_noOptimization_smallDataset_SRC := kerDoitgen_noOptimization_smallDataset.cpp
kerDoitgen_noOptimization_smallDataset_SRC_ROUTE := $(kerDoitgen_noOptimization_FOLDER)kerDoitgen_noOptimization_smallDataset.cpp
kerDoitgen_noOptimization_smallDataset_HEADER :=
kerDoitgen_noOptimization_smallDataset_HEADER_ROUTE :=

kerDoitgen_noOptimization_mediumDataset_KERNEL := kerDoitgen_noOptimization_mediumDataset
kerDoitgen_noOptimization_mediumDataset_SRC := kerDoitgen_noOptimization_mediumDataset.cpp
kerDoitgen_noOptimization_mediumDataset_SRC_ROUTE := $(kerDoitgen_noOptimization_FOLDER)kerDoitgen_noOptimization_mediumDataset.cpp
kerDoitgen_noOptimization_mediumDataset_HEADER :=
kerDoitgen_noOptimization_mediumDataset_HEADER_ROUTE :=

kerDoitgen_noOptimization_largeDataset_KERNEL := kerDoitgen_noOptimization_largeDataset
kerDoitgen_noOptimization_largeDataset_SRC := kerDoitgen_noOptimization_largeDataset.cpp
kerDoitgen_noOptimization_largeDataset_SRC_ROUTE := $(kerDoitgen_noOptimization_FOLDER)kerDoitgen_noOptimization_largeDataset.cpp
kerDoitgen_noOptimization_largeDataset_HEADER :=
kerDoitgen_noOptimization_largeDataset_HEADER_ROUTE :=

kerDoitgen_noOptimization_extralargeDataset_KERNEL := kerDoitgen_noOptimization_extralargeDataset
kerDoitgen_noOptimization_extralargeDataset_SRC := kerDoitgen_noOptimization_extralargeDataset.cpp
kerDoitgen_noOptimization_extralargeDataset_SRC_ROUTE := $(kerDoitgen_noOptimization_FOLDER)kerDoitgen_noOptimization_extralargeDataset.cpp
kerDoitgen_noOptimization_extralargeDataset_HEADER :=
kerDoitgen_noOptimization_extralargeDataset_HEADER_ROUTE :=


.phony: clean traces help

build: copy_files

copy_files:
	mkdir ${BUILD_DIR}; \
	cp -r ${CONNECTIVITY_ROUTE} ${BUILD_DIR}; \
	cp ${kerDoitgen_noOptimization_miniDataset_SRC_ROUTE} ${BUILD_DIR}; \
	cp ${kerDoitgen_noOptimization_smallDataset_SRC_ROUTE} ${BUILD_DIR}; \
	cp ${kerDoitgen_noOptimization_mediumDataset_SRC_ROUTE} ${BUILD_DIR}; \
	cp ${kerDoitgen_noOptimization_largeDataset_SRC_ROUTE} ${BUILD_DIR}; \
	cp ${kerDoitgen_noOptimization_extralargeDataset_SRC_ROUTE} ${BUILD_DIR}; \
	cp ${MAKEFILE_NAME} ${BUILD_DIR}; \
	cd ${BUILD_DIR}; \
	make -f $(MAKEFILE_NAME) kerDoitgen_noOptimization.xclbin
	

kerDoitgen_noOptimization.xclbin: $(kerDoitgen_noOptimization_XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMFLAGS) -o $@ $(XOS) --remote_ip_cache ${IP_CACHE_DIR}



kerDoitgen_noOptimization_miniDataset.xo: $(kerDoitgen_noOptimization_miniDataset_SRC) $(kerDoitgen_noOptimization_miniDataset_HEADER)
	v++ --kernel $(kerDoitgen_noOptimization_miniDataset_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(kerDoitgen_noOptimization_miniDataset_SRC) $<

kerDoitgen_noOptimization_smallDataset.xo: $(kerDoitgen_noOptimization_smallDataset_SRC) $(kerDoitgen_noOptimization_smallDataset_HEADER)
	v++ --kernel $(kerDoitgen_noOptimization_smallDataset_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(kerDoitgen_noOptimization_smallDataset_SRC) $<

kerDoitgen_noOptimization_mediumDataset.xo: $(kerDoitgen_noOptimization_mediumDataset_SRC) $(kerDoitgen_noOptimization_mediumDataset_HEADER)
	v++ --kernel $(kerDoitgen_noOptimization_mediumDataset_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(kerDoitgen_noOptimization_mediumDataset_SRC) $<

kerDoitgen_noOptimization_largeDataset.xo: $(kerDoitgen_noOptimization_largeDataset_SRC) $(kerDoitgen_noOptimization_largeDataset_HEADER)
	v++ --kernel $(kerDoitgen_noOptimization_largeDataset_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(kerDoitgen_noOptimization_largeDataset_SRC) $<

kerDoitgen_noOptimization_extralargeDataset.xo: $(kerDoitgen_noOptimization_extralargeDataset_SRC) $(kerDoitgen_noOptimization_extralargeDataset_HEADER)
	v++ --kernel $(kerDoitgen_noOptimization_extralargeDataset_KERNEL) $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(kerDoitgen_noOptimization_extralargeDataset_SRC) $<



clean:
	@find . \( -type d \( -name '_x' -o -name '.Xil' \) -o -type f \( -name '*.xo' -o -name '*.json' -o -name 'sd_card' -o -name '*.xclbin' -o -name '*.ltx' -o -name '*.log' -o -name '*.info' -o -name '*compile_summary*' -o -name 'vitis_analyzer*' -o -name '*link_summary*' \) \) -exec rm -rf {} +
