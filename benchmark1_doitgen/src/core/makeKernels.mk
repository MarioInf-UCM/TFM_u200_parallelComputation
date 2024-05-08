#*****************************
#* CONFIG COMPULATION PARAMS *
#*****************************

# Options for TARGET: sw_emu, hw_emu and hw
TARGET ?= sw_emu
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms

# Definir TYPEDATA_INT o TYPEDATA_FP basado en el valor de TYPEDATA
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

PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)
BOARD_CONFIG := cmake/connectivity_u200.ini
LINK_OPTS := --config cmake/connectivity_u200.ini -I ./src/kernel

VPPLFLAGS := --jobs $(JOBS) --config cmake/profile.ini
VPPLFLAGS += --config $(BOARD_CONFIG)
VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g

# Define CUSTOMFLAGS con TYPEDATA_COMPILER
CUSTOMFLAGS := -D$(TYPEDATA_COMPILER)

XOS = doitgenKernel.xo
IP_CACHE_DIR ?= ip_cache


#***********************
#* KERNELS SOURCE LIST *
#***********************
DOITGEN_KERNEL_SRC := doitgenKernel.cpp
DOITGEN_KERNEL_HEADER := ../config/config.hpp

.phony: clean traces help

build: kernels.xclbin

kernels.xclbin: $(XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMFLAGS) -o $@ $(XOS) --remote_ip_cache ${IP_CACHE_DIR}

doitgenKernel.xo: $(DOITGEN_KERNEL_SRC) $(DOITGEN_KERNEL_HEADER)
	v++ --kernel doitgenKernel $(VPPFLAGS) $(CUSTOMFLAGS) -c -o $@ $(KERNEL_SRC) $<

clean:
	@find . \( -type d \( -name '_x' -o -name '.Xil' \) -o -type f \( -name '*.xo' -o -name '*.json' -o -name 'sd_card' -o -name '*.xclbin' -o -name '*.ltx' -o -name '*.log' -o -name '*.info' -o -name '*compile_summary*' -o -name 'vitis_analyzer*' -o -name '*link_summary*' \) \) -exec rm -rf {} +
