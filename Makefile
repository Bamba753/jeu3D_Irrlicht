CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused

irrlicht: main.o GameEvent.o Camera.o Player.o Game.o Enemy.o
	g++ $^ -o $@ -lIrrlicht

clean:
	rm -f *.o
mrproper: clean
	rm -f irrlicht
