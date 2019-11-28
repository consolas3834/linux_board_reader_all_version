SOURCE=$(wildcard *.cpp)
TARGETS=$(patsubst %.cpp, %, $(SOURCE))

#CC=g++
#CC=arm-fsl-linux-gnueabi-g++
CC=/opt/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc

CFLAGS=-L./ -lYBTRFAPI

all:$(TARGETS)
$(TARGETS):%:%.cpp
	$(CC) $< $(CFLAGS) -o $@
.PHONY:clean all 
clean: 
	-rm -rf $(TARGETS)
