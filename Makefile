# ================== App ==================
APP = tetris

CC = gcc

SRC_DIR = src
INC_DIR = includes

# ================== Sources ==================
SOURCES := $(shell find $(SRC_DIR) -name "*.c")
OBJECTS := $(SOURCES:.c=.o)
.INTERMEDIATE: $(OBJECTS)

# ================== Flags ==================
CFLAGS = -Wall -Wextra -g -std=c11 -I$(INC_DIR)

# ================== Rules ==================
.PHONY: all clean run

all: $(APP)
	@echo "Binary ready -> $(APP)"

$(APP): $(OBJECTS)
	@echo "Linking $(APP)..."
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS) $(APP)

run: all
	./$(APP)
