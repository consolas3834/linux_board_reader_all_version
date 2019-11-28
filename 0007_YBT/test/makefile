SOURCE=$(wildcard *.cpp)
TARGETS=$(patsubst %.cpp, %, $(SOURCE))

#CC=g++
CC=/opt/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc

#CFLAGS=-lYBTRFAPI64 -m64

all:$(TARGETS)
$(TARGETS):%:%.cpp
	$(CC) $< -o -LlibYBTRFAPI64.so  $@
.PHONY:clean all 
clean: 
	-rm -rf $(TARGETS)

#CC=/opt/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc
#CC=gcc

#reader:reader.o 
#	$(CC) -o reader reader.o  -lpthread  -I/opt/include -L/opt/lib/   -L./lib/libdhcpcd -lrt ./lib/libMercuryAPI.a

#reader.o:reader.c 
#	$(CC) -c  reader.c

#clean:
#	rm reader  *.o

