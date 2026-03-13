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
CFLAGS = -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE=199309L -I$(INC_DIR)
LDLIBS = -lncurses -lm

# ================== Rules ==================
.PHONY: all clean run

all: $(APP)
	@echo "Binary ready -> $(APP)"

$(APP): $(OBJECTS)
	@echo "Linking $(APP)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS) $(APP)

run: all
	./$(APP) -d snapshot
