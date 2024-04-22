include config/config.mk

PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)

VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g
VPPLFLAGS := --jobs $(JOBS) --config profile.ini

BOARD_CONFIG := config/connectivity_u200.ini
VPPLFLAGS += --config $(BOARD_CONFIG)

TEMP_DIR := kernels
VPPLFLAGS += --temp_dir $(TEMP_DIR)

LOG_DIR := kernels
VPPLFLAGS += --log_dir $(LOG_DIR)

REPORT_DIR := kernels
VPPLFLAGS += --report_dir $(REPORT_DIR)


XOS = doitgenKernel.xo
IP_CACHE_DIR ?= kernels/ip_cache

.phony: clean traces help

buildKernels: doitgenKernels.xclbin

doitgenKernels.xclbin: $(XOS) $(BOARD_CONFIG)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o kernels/$@ $(XOS) --remote_ip_cache ${IP_CACHE_DIR}

doitgenKernel.xo: kernels/doitgenKernel.cpp
	v++ --kernel kernels/doitgenKernel.cpp $(VPPFLAGS) -c -o kernels/$@ $<




doitgen:
	gcc -O3 -I utilities -I src/ src/utilities/polybench.cpp src/doitgen.cpp -DPOLYBENCH_TIME -DEXTRALARGE_DATASET -o build/doitgen
	
doitgenRun:
	./build/doitgen

clean:
	rm -rf build/**

cleanKernels:
	find . \( -type d \( -name '_x' -o -name '.Xil' \) -o -type f \( -name '*.xo' -o -name 'sd_card' -o -name '*.xclbin' -o -name '*.ltx' -o -name '*.log' -o -name '*.info' -o -name '*compile_summary*' -o -name 'vitis_analyzer*' -o -name '*link_summary*' \) \) -exec rm -rf {} +