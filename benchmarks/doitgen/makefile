

.phony: clean traces help


#COMMANDS FOR KERNELS COMPILE
#**************************************
makeKernels_build:
	@cd kernels; \
	make -f makeKernels.mk build

makeKernels_clean:
	@cd kernels; \
	make -f makeKernels.mk clean


#COMMANDS FOR SOFTWARE COMPILE
#**************************************
doitgen:
	gcc -O3 -I utilities -I src/ src/utilities/polybench.cpp src/doitgen.cpp -DPOLYBENCH_TIME -DEXTRALARGE_DATASET -o build/doitgen
	
doitgenRun:
	./build/doitgen

clean:
	rm -rf build/**
