# Compiler
CC = gcc

# Flags
CFLAGS = -I include -Wall
SRC = main.c $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET = game.exe
    LIBS = -lwinmm
    RM = del /Q
    SEP = \\
else
    TARGET = game
    LIBS =
    RM = rm -f
    SEP = /
endif

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compile each .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	$(RM) $(TARGET) $(OBJ)

# Run
run: $(TARGET)
ifeq ($(OS),Windows_NT)
	@$(TARGET)
else
	@./$(TARGET)
endif

.PHONY: all clean run



# compile: make
# clean: make clean
# run: make run

# windows run: mingw32-make run, 
# windows clean: mingw32-make clean