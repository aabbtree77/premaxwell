#GCC_HOME=${HOME}/gcc-4.6.0
#PATH := ${GCC_HOME}/bin:${PATH}
#LD_LIBRARY_PATH := /usr/lib
#LD_RUN_PATH := ${LD_LIBRARY_PATH}
CC=g++
CFLAGS=-Wall -ansi -pedantic -std=c++0x -O3
#CFLAGS=-Wall -ansi -pedantic -std=c++11 -O3 -Wl,-rpath -Wl,${LD_LIBRARY_PATH}
#CFLAGS=-Wall -ansi -pedantic -std=c++0x -O3 -Wl,-rpath

OBJS = main.o halfspace.o sphere.o baseclass.o light.o
all: premaxwell

premaxwell: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o premaxwell

main.o : main.cpp halfspace.hpp sphere.hpp baseclass.hpp
	$(CC) $(CFLAGS) -c main.cpp

halfspace.o : halfspace.cpp halfspace.hpp baseclass.hpp
	$(CC) $(CFLAGS) -c halfspace.cpp

sphere.o : sphere.cpp sphere.hpp baseclass.hpp
	$(CC) $(CFLAGS) -c sphere.cpp

light.o : light.cpp light.hpp baseclass.hpp
	$(CC) $(CFLAGS) -c light.cpp

baseclass.o : baseclass.cpp baseclass.hpp
	$(CC) $(CFLAGS) -c baseclass.cpp

clean: 
	\rm *.o premaxwell

tar:
	tar cfv premaxwell.tar Makefile main.cpp \
	halfspace.hpp halfspace.cpp sphere.cpp sphere.hpp \
	baseclass.hpp baseclass.cpp  light.hpp light.cpp

