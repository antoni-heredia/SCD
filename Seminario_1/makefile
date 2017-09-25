# Fichero: makefile
HOME = .


BIN=$(HOME)/bin
OBJ=$(HOME)/obj
INCLUDE=$(HOME)/include
LIB=$(HOME)/lib
SRC=$(HOME)/src


all : $(BIN)/ejemplo09

#ejecutables
$(BIN)/ejemplo09 : $(SRC)/ejemplo09-solucion.cpp

	g++ -g -o $(BIN)/ejemplo09 $(SRC)/ejemplo09-solucion.cpp -pthread -std=c++11

#opciones de limpieza
cleanlib :
	@echo Borrando librerias
	@-rm $(LIB)/*.a

clean :
	@echo Borrando ficheros objetos
	@-rm $(OBJ)/*.o
mr-proper :

	@echo Borrando ejecutables
	@-rm $(BIN)/*
