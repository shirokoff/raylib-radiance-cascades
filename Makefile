# Makefile for raylib-shadows (macOS / Linux)
CC := clang
UNAME_S := $(shell uname -s)
CFLAGS := -Wall -Wextra -O2 -g -I./includes

SRC := main.c
OBJ := $(SRC:.c=.o)
TARGET := raylib-shadows

.PHONY: all clean run

ifeq ($(UNAME_S),Darwin)
    # Use local static library (raylib.a) and macOS frameworks
    LIBRAYLIB := ./includes/libraylib.a
    FRAMEWORKS := -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -framework OpenGL
    LIBS := $(LIBRAYLIB) $(FRAMEWORKS) -lm
else
    # Linux link flags (unchanged)
    LIBS := -lraylib -lGL -lm -pthread -ldl -lrt -lX11
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)