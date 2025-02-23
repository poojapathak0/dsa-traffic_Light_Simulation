CC=gcc
# Update this path to the include directory
CFLAGS=-Iinclude -IC:/Users/ACER/Downloads/SDL2-devel-2.32.0-mingw/SDL2-2.32.0/x86_64-w64-mingw32/include/SDL2
# Update this path to the lib directory
LDFLAGS=-LC:/Users/ACER/Downloads/SDL2-devel-2.32.0-mingw/SDL2-2.32.0/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lpthread

SRCS=src/traffic_light_simulator.c src/queue.c src/vehicle.c src/traffic_light.c src/priority_queue.c
GENERATOR_SRC=src/vehicle_generator.c

BIN_DIR=bin

all: $(BIN_DIR)/simulator.exe $(BIN_DIR)/generator.exe

$(BIN_DIR)/simulator.exe: $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/simulator.exe $(SRCS) $(CFLAGS) $(LDFLAGS)

ifneq ("$(wildcard $(GENERATOR_SRC))","")
$(BIN_DIR)/generator.exe: $(GENERATOR_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/generator.exe $(GENERATOR_SRC) $(CFLAGS) $(LDFLAGS)
else
$(BIN_DIR)/generator.exe:
	@echo "Warning: vehicle_generator.c not found, skipping generator build."
endif

clean:
	rm -f $(BIN_DIR)/*.exe $(BIN_DIR)/*.o

.PHONY: all clean