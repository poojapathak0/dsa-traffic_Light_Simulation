CC=g++
CFLAGS=-Iinclude -Llib -lmingw32 -lSDL2main -lSDL2
SRCDIR=src
BINDIR=bin

all: $(BINDIR)/simulator

$(BINDIR)/simulator: $(SRCDIR)/vehicle_generator.c $(SRCDIR)/traffic_light_simulator.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BINDIR)/*