all:
	g++ src/*.cpp main.cpp -c
	g++ *.o -o BasicCompiler
	del *.o