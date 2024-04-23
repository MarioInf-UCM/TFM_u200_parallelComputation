include ../config/config.mk

PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)
BOARD_CONFIG := ../config/connectivity_u200.ini

VPPLFLAGS := --jobs $(JOBS) --config ../config/profile.ini
VPPLFLAGS += --config $(BOARD_CONFIG)
VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g 

XOS = doitgenKernel.xo
IP_CACHE_DIR ?= ip_cache


.phony: clean traces help



build: kernels.xclbin

kernels.xclbin: $(XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(XOS) --remote_ip_cache ${IP_CACHE_DIR}

doitgenKernel.xo: doitgenKernel.cpp
	v++ --kernel doitgenKernel $(VPPFLAGS) -c -o $@ $<



clean:
	@find . \( -type d \( -name '_x' -o -name '.Xil' \) -o -type f \( -name '*.xo' -o -name 'sd_card' -o -name '*.xclbin' -o -name '*.ltx' -o -name '*.log' -o -name '*.info' -o -name '*compile_summary*' -o -name 'vitis_analyzer*' -o -name '*link_summary*' \) \) -exec rm -rf {} +