# Options for TARGET: sw_emu, hw_emu and hw
TARGET ?= sw_emu
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms


PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)
BOARD_CONFIG := config/connectivity_u200.ini

VPPLFLAGS := --jobs $(JOBS) --config config/profile.ini
VPPLFLAGS += --config $(BOARD_CONFIG)
VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g 

XOS = doitgenKernel.xo
IP_CACHE_DIR ?= ip_cache

.phony: clean traces help



build: kernels.xclbin
ifeq ($(TARGET),sw_emu)
	@echo "Compiling kernels for Software Emulation (XCL_EMULATION_MODE=sw_emu)"
	export XCL_EMULATION_MODE=sw_emu

else ifeq ($(TARGET),hw_emu)
	@echo "Compiling kernels for Hardware Emulation (XCL_EMULATION_MODE=sw_emu)"
	export XCL_EMULATION_MODE=hw_emu

else ifeq ($(TARGET),hw)
	@echo "Compiling kernels for Hardware Deployment"

else
	@echo "TARGET variable not recognized. Please set TARGET to 'sw_emu', 'hw_emu', or 'hw'."
endif




kernels.xclbin: $(XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(XOS) --remote_ip_cache ${IP_CACHE_DIR}

doitgenKernel.xo: doitgenKernel.cpp
	v++ --kernel doitgenKernel $(VPPFLAGS) -c -o $@ $<



clean:
	@find . \( -type d \( -name '_x' -o -name '.Xil' \) -o -type f \( -name '*.xo' -o -name 'sd_card' -o -name '*.xclbin' -o -name '*.ltx' -o -name '*.log' -o -name '*.info' -o -name '*compile_summary*' -o -name 'vitis_analyzer*' -o -name '*link_summary*' \) \) -exec rm -rf {} +