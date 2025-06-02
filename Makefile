CXX=clang++
CXXFLAGS=-Wall -Wextra -std=c++23 -pedantic -ggdb -Og -I./raylib-cpp/include/
LIBS=-lraylib

DEPS=projectile.hh       \
     player.hh           \
     states.hh           \
     running.hh          \
     util.hh             \
	 game.hh			 \
     pool.hh

OBJ=main.o               \
    projectile.o         \
    player.o             \
    states.o             \
    running.o            \
    util.o 			  	 \
    pool.o

bullethell: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

run: bullethell
	./bullethell

%.o: %.cc Makefile $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o bullethell

.PHONY: clean
