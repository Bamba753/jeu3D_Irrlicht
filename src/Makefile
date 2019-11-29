CPPFLAGS = -I/usr/include/irrlicht -I"../include" -L"/usr/lib" ../bin/linux-gcc-64/libIrrKlang.so -pthread
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused
OPT= -I"../include" -L"/usr/lib" ../bin/linux-gcc-64/libIrrKlang.so -pthread 

irrlicht: main.o GameEvent.o Camera.o Player.o Game.o Enemy.o
	g++ $^ -o $@ -lIrrlicht $(OPT)

clean:
	rm -f *.o
mrproper: clean
	rm -f irrlicht
