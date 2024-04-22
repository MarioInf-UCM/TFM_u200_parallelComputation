include config.mk

PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)

VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g
VPPLFLAGS := --jobs $(JOBS) --config profile.ini 
BOARD_CONFIG := connectivity_u200.ini
VPPLFLAGS += --config $(BOARD_CONFIG)

XOS = doitgen.xo

IP_CACHE_DIR ?= kernels/ip_cache

.phony: clean traces help

buildKernels: doitgenKernels.xclbin

doitgenKernels.xclbin: $(XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(XOS) --remote_ip_cache ${IP_CACHE_DIR}

doitgen.xo: doitgen.cpp
	v++ --kernel vadd $(VPPFLAGS) -c -o $@ $<

clean:
	$(RM) -r *.xo _x .Xil sd_card *.xclbin *.ltx *.log *.info *compile_summary* vitis_analyzer* *link_summary*
