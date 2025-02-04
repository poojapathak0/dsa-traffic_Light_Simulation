CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lSDL2

all: simulator traffic_generator

simulator: simulator.c
    $(CC) $(CFLAGS) -o simulator simulator.c $(LIBS)

traffic_generator: traffic_generator.c
    $(CC) $(CFLAGS) -o traffic_generator traffic_generator.c

clean:
    rm -f simulator traffic_generator