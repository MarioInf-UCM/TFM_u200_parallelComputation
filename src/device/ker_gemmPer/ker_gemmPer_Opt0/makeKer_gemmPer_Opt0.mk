#**********************
#* CHECK ENTRY PARAMS *
#**********************
ifeq ($(TARGET),sw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_gemmPer_Opt0/sw_emu/
else ifeq ($(TARGET),hw_emu)
	KERNEL_BUILD_SUBFOLDER := ker_gemmPer_Opt0/hw_emu/
else ifeq ($(TARGET),hw)
	KERNEL_BUILD_SUBFOLDER := ker_gemmPer_Opt0/hw/
else
    @echo "Incorret value of TARGET. Please, try again using: make -f <makeFileName> TARGET=sw_emu|hw_emu|hw"
    $(error "Incorret value of TARGET. Please, try again using: make -f <makeFileName> TARGET=sw_emu|hw_emu|hw")
	exit -1
endif

ifndef JOBS
	NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
	JOBS := $(shell expr $(NCPUS) - 1)
endif

#*****************************
#* CONFIG COMPULATION PARAMS *
#*****************************
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms
PFM := $(PLATFORM_REPO_PATHS)/$(PLATFORM)/$(PLATFORM).xpfm
IP_CACHE_DIR ?= ip_cache


BUILD_DIR := ../../../../build/kernelBuild
CONNECTIVITY_FOLDER := ../gemmPer_connectivityConfig/
CONNECTIVITY_NAME := gemmPer_connec.ini
CONNECTIVITY_URL := $(CONNECTIVITY_FOLDER)$(CONNECTIVITY_NAME)

PROFILE_FOLDER := ../gemmPer_connectivityConfig/
PROFILE_NAME := profile.ini
PROFILE_URL := $(CONNECTIVITY_FOLDER)$(PROFILE_NAME)

VPPFLAGS := --platform $(PFM) -t $(TARGET) -s -g --hls.jobs $(JOBS) --vivado.synth.jobs $(JOBS) --vivado.impl.jobs $(JOBS)
VPPLFLAGS := --config $(CONNECTIVITY_NAME) --config $(PROFILE_NAME) 
CUSTOMPARAMS_GENERAL :=



#********************************************
#* COMPILATION OF kergemmPer_Opt0imization *
#********************************************
MAKEFILE_NAME := makeKer_gemmPer_Opt0.mk

PackKerList := kerPack_gemmPer_Opt0_mini
PackKerList += kerPack_gemmPer_Opt0_small
PackKerList += kerPack_gemmPer_Opt0_medium
PackKerList += kerPack_gemmPer_Opt0_large
PackKerList += kerPack_gemmPer_Opt0_extralarge


ker_gemmPer_Opt0_mini_XOS := ker_gemmPer_Opt0_mini.xo
ker_gemmPer_Opt0_small_XOS := ker_gemmPer_Opt0_small.xo
ker_gemmPer_Opt0_medium_XOS := ker_gemmPer_Opt0_medium.xo
ker_gemmPer_Opt0_large_XOS := ker_gemmPer_Opt0_large.xo
ker_gemmPer_Opt0_extralarge_XOS := ker_gemmPer_Opt0_extralarge.xo

ker_gemmPer_Opt0_mini_KERNEL := ker_gemmPer_Opt0_mini
ker_gemmPer_Opt0_small_KERNEL := ker_gemmPer_Opt0_small
ker_gemmPer_Opt0_medium_KERNEL := ker_gemmPer_Opt0_medium
ker_gemmPer_Opt0_large_KERNEL := ker_gemmPer_Opt0_large
ker_gemmPer_Opt0_extralarge_KERNEL := ker_gemmPer_Opt0_extralarge


ker_gemmPer_Opt0_mini_CUSTOMPARAMS := -DMINI_DATASET
ker_gemmPer_Opt0_small_CUSTOMPARAMS := -DSMALL_DATASET
ker_gemmPer_Opt0_medium_CUSTOMPARAMS := -DMEDIUM_DATASET
ker_gemmPer_Opt0_large_CUSTOMPARAMS := -DLARGE_DATASET
ker_gemmPer_Opt0_extralarge_CUSTOMPARAMS := -DEXTRALARGE_DATASET


ker_gemmPer_Opt0_SRC := ker_gemmPer_Opt0.cpp
ker_gemmPer_Opt0_HEADER :=


#********************
#* GOALS DEFINITION *
#********************
build: copy_files

copy_files:
	mkdir -p ${BUILD_DIR};
	mkdir -p ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp -r ${CONNECTIVITY_URL} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp -r ${PROFILE_URL} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp -r ${ker_gemmPer_Opt0_SRC} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	cp -r ${MAKEFILE_NAME} ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER};
	$(foreach val,$(PackKerList), \
		$(if $(filter $(TARGET),sw_emu), \
			cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
			make -f $(MAKEFILE_NAME) TARGET=$(TARGET) $(val)_sw_emu.xclbin;, \
		$(if $(filter $(TARGET),hw_emu), \
			cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
			make -f $(MAKEFILE_NAME) TARGET=$(TARGET) $(val)_hw_emu.xclbin;, \
		$(if $(filter $(TARGET),hw), \
			cd ${BUILD_DIR}/${KERNEL_BUILD_SUBFOLDER}; \
			make -f $(MAKEFILE_NAME) TARGET=$(TARGET) $(val)_hw.xclbin;, \
		$(error Error: TARGET $(TARGET) is not recognized.) \
	))))






kerPack_gemmPer_Opt0_mini_sw_emu.xclbin: $(ker_gemmPer_Opt0_mini_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_mini_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_small_sw_emu.xclbin: $(ker_gemmPer_Opt0_small_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_small_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_medium_sw_emu.xclbin: $(ker_gemmPer_Opt0_medium_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_medium_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_large_sw_emu.xclbin: $(ker_gemmPer_Opt0_large_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_large_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_extralarge_sw_emu.xclbin: $(ker_gemmPer_Opt0_extralarge_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_extralarge_XOS) --remote_ip_cache ${IP_CACHE_DIR}



kerPack_gemmPer_Opt0_mini_hw_emu.xclbin: $(ker_gemmPer_Opt0_mini_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_mini_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_small_hw_emu.xclbin: $(ker_gemmPer_Opt0_small_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_small_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_medium_hw_emu.xclbin: $(ker_gemmPer_Opt0_medium_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_medium_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_large_hw_emu.xclbin: $(ker_gemmPer_Opt0_large_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_large_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_extralarge_hw_emu.xclbin: $(ker_gemmPer_Opt0_extralarge_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_extralarge_XOS) --remote_ip_cache ${IP_CACHE_DIR}



kerPack_gemmPer_Opt0_mini_hw.xclbin: $(ker_gemmPer_Opt0_mini_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_mini_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_small_hw.xclbin: $(ker_gemmPer_Opt0_small_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_small_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_medium_hw.xclbin: $(ker_gemmPer_Opt0_medium_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_medium_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_large_hw.xclbin: $(ker_gemmPer_Opt0_large_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_large_XOS) --remote_ip_cache ${IP_CACHE_DIR}

kerPack_gemmPer_Opt0_extralarge_hw.xclbin: $(ker_gemmPer_Opt0_extralarge_XOS) $(CONNECTIVITY_NAME)
	v++ -l $(VPPFLAGS) $(VPPLFLAGS) -o $@ $(ker_gemmPer_Opt0_extralarge_XOS) --remote_ip_cache ${IP_CACHE_DIR}




ker_gemmPer_Opt0_mini.xo: $(ker_gemmPer_Opt0_SRC) $(ker_gemmPer_Opt0_HEADER)
	v++ --kernel $(ker_gemmPer_Opt0_mini_KERNEL) $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemmPer_Opt0_mini_CUSTOMPARAMS) -c -o $@ $(ker_gemmPer_Opt0_SRC) $<

ker_gemmPer_Opt0_small.xo: $(ker_gemmPer_Opt0_SRC) $(ker_gemmPer_Opt0_HEADER)
	v++ --kernel $(ker_gemmPer_Opt0_small_KERNEL) $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemmPer_Opt0_small_CUSTOMPARAMS) -c -o $@ $(ker_gemmPer_Opt0_SRC) $<

ker_gemmPer_Opt0_medium.xo: $(ker_gemmPer_Opt0_SRC) $(ker_gemmPer_Opt0_HEADER)
	v++ --kernel $(ker_gemmPer_Opt0_medium_KERNEL) $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemmPer_Opt0_medium_CUSTOMPARAMS) -c -o $@ $(ker_gemmPer_Opt0_SRC) $<

ker_gemmPer_Opt0_large.xo: $(ker_gemmPer_Opt0_SRC) $(ker_gemmPer_Opt0_HEADER)
	v++ --kernel $(ker_gemmPer_Opt0_large_KERNEL) $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemmPer_Opt0_large_CUSTOMPARAMS) -c -o $@ $(ker_gemmPer_Opt0_SRC) $<

ker_gemmPer_Opt0_extralarge.xo: $(ker_gemmPer_Opt0_SRC) $(ker_gemmPer_Opt0_HEADER)
	v++ --kernel $(ker_gemmPer_Opt0_extralarge_KERNEL) $(VPPFLAGS) $(VPPLFLAGS) $(CUSTOMPARAMS_GENERAL) $(ker_gemmPer_Opt0_extralarge_CUSTOMPARAMS) -c -o $@ $(ker_gemmPer_Opt0_SRC) $<