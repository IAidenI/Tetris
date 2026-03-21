# ================== App ==================
APP = tetris
MODE ?= cli

CC = gcc

SRC_DIR = src
INC_DIR = includes

# ================== Common sources ==================
CORE_SOURCES  := $(shell find $(SRC_DIR)/core -name "*.c")
UTIL_SOURCES  := $(shell find $(SRC_DIR)/utils -name "*.c")

ifeq ($(MODE),cli)
    MAIN_SOURCE = $(SRC_DIR)/mainCLI.c
    UI_SOURCES  := $(shell find $(SRC_DIR)/ui/ncurses -name "*.c")
    LDLIBS      = -lncursesw -lm
else ifeq ($(MODE),gui)
    MAIN_SOURCE = $(SRC_DIR)/mainGUI.c
    UI_SOURCES  := $(shell find $(SRC_DIR)/ui/raylib -name "*.c")
    LDLIBS      = -lraylib -lm
else
    $(error MODE invalide. Utiliser MODE=cli ou MODE=gui)
endif

SOURCES := $(CORE_SOURCES) $(UTIL_SOURCES) $(UI_SOURCES) $(MAIN_SOURCE)
OBJECTS := $(SOURCES:.c=.o)
.INTERMEDIATE: $(OBJECTS)

# ================== Flags ==================
CFLAGS = -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE=199309L -D_XOPEN_SOURCE=700 -I$(INC_DIR)

# ================== Rules ==================
.PHONY: all clean run cli gui

all: $(APP)
	@echo "Binary ready -> $(APP) [$(MODE)]"

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
	./$(APP)

cli:
	$(MAKE) MODE=cli

gui:
	$(MAKE) MODE=gui