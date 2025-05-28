#!/bin/sh
set -euxo pipefail

c++ main.cc -Wall -Wextra -std=c++23 -pedantic -ggdb -Og -lraylib -o bullethell
./bullethell
