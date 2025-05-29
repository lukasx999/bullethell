CXX=c++
CXXFLAGS=-Wall -Wextra -std=c++23 -pedantic -ggdb -Og
LIBS=-lraylib
DEPS=projectile.hh player.hh util.hh

bullethell: main.o projectile.o player.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

run: bullethell
	./bullethell

%.o: %.cc Makefile $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o bullethell

.PHONY: clean
