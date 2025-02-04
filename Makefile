CC=gcc
CFLAGS=-I. -lSDL2 -lm
DEPS=traffic_light_simulator.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

traffic_light_simulator: traffic_light_simulator.o
	$(CC) -o $@ $^ $(CFLAGS)

vehicle_generator: vehicle_generator.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o traffic_light_simulator vehicle_generator