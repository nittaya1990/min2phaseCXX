all: libmin2phase.so example benchmark tests

example: libmin2phase.so
	g++ -I include/ example.cpp -L. -lmin2phase -o example -Wl,-rpath,.

benchmark: libmin2phase.so
	g++ -I include/ benchmark.cpp -L. -lmin2phase -o benchmark -Wl,-rpath,.

tests: libmin2phase.so
	g++ -I include/ tests.cpp -L. -lmin2phase -o tests -Wl,-rpath,.

libmin2phase.so:
	g++ -I include/ -c -fPIC src/info.cpp src/CubieCube.cpp src/coords.cpp src/Search.cpp src/tests.cpp src/tools.cpp src/min2phase.cpp
	g++ -shared -fPIC -o libmin2phase.so info.o CubieCube.o coords.o Search.o tests.o tools.o min2phase.o
	rm -f *.o

clean:
	rm -f *.o *.so example benchmark tests