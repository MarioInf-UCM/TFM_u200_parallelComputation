TYPEDATA=INT	#Posible values: INT, FP

.phony: clean build


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
	@cd build; \
	cmake -DTYPEDATA=${TYPEDATA} ..; \
	make -j

doitgenRun:
	@cd build; \
	./doitgen

clean:
	rm -rf build/*
