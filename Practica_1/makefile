.SUFFIXES:
.PHONY: pc
.PRECIOUS: %_exe

compilador:=g++
all: pcfifo1 pcfifo2 fu

pcfifo1: prodconsFIFO1_exe
	

pcfifo2: prodconFIFO2_exe
	

fu: fumadores_exe
	

%_exe: %.cpp
		$(compilador) -std=c++11 -I. -o $@ $< Semaphore.cpp -pthread

clean:
	rm -f *_exe
