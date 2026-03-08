# ================== Choix de l'app ==================
# Usage:
#   make                 -> build tetris
#   make APP= -> build 
APP ?= tetris

CC         = gcc
SOURCEDIR  = sources
HEADERSDIR = headers

# ================== Sources communes ==================
COMMON_SOURCES := $(wildcard $(SOURCEDIR)/**/*.c) $(wildcard $(SOURCEDIR)/*.c)

# ================== Sources par app ==================
APP_SOURCE := $(APP).c
SOURCES    := $(APP_SOURCE) $(COMMON_SOURCES)

OBJECTS := $(SOURCES:.c=.o)
.INTERMEDIATE: $(OBJECTS)

CFLAGS  = -Wall -g -std=c11 -I$(HEADERSDIR)

# Maybe uncomment if there is an raylib implementation
# ================== Raylib uniquement pour l'UI ==================
#RAYLIB_CFLAGS :=
#RAYLIB_LIBS   :=

#ifeq ($(APP),blackjackUI)
#  RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
#  RAYLIB_LIBS   := $(shell pkg-config --libs raylib 2>/dev/null)
  # Fallback si pkg-config n'est pas dispo/renvoie vide
#  ifeq ($(strip $(RAYLIB_LIBS)),)
#    RAYLIB_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#  endif
#endif

#CFLAGS += $(RAYLIB_CFLAGS)
#LDLIBS  = $(RAYLIB_LIBS)

#RM := rm -f

# ================== Règles ==================
.PHONY: all clean run

all: $(APP)
	@echo "Binary ready ! -> $(APP)"

$(APP): $(OBJECTS)
	@echo "Compiling and linking $(APP)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c $(HEADERSDIR)/*.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	-$(RM) $(OBJECTS) blackjackCLI blackjackUI

run: all
	./$(APP)
