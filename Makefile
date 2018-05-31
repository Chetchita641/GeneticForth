all: genetic.exe

genetic.exe: genetic.cpp Makefile
	g++ -o genetic.exe genetic.cpp
