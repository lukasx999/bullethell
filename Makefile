CXX=c++
CXXFLAGS=-Wall -Wextra -std=c++23 -pedantic -ggdb -Og
LIBS=-lraylib

DEPS=projectile.hh       \
     player.hh           \
     welcome.hh          \
     paused.hh           \
     over.hh             \
     running.hh          \
     util.hh             \
     game.hh

OBJ=main.o               \
    projectile.o         \
    player.o             \
    welcome.o            \
    paused.o             \
    over.o               \
    running.o            \
    util.o

bullethell: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

run: bullethell
	./bullethell

%.o: %.cc Makefile $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o bullethell

.PHONY: clean
