DEBUG=y
STDFLAG=--std=c++11
CUDAPATH=/usr/local/cuda
CUDALIB=-L$(CUDAPATH)/lib64/ -lcuda -lcudart
NVCC=$(CUDAPATH)/bin/nvcc
CUDACC=sm_61

ifeq ($(DEBUG),n)
	OPTFLAG= -O3
else
	OPTFLAG= -O0 -g -DDEBUG
	CUDADEBUGFLAG = -G
endif

nimwi: nimwi.cpp
	g++ -fopenmp $(STDFLAG) $(OPTFLAG) -Wall nimwi.cpp $(CUDALIB) -o nimwi

clean:
	rm -f nimwi *.o
