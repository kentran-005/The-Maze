CC = gcc
CFLAGS = -I include -Wall
SRC = main.c src/*.c

ifeq ($(OS),Windows_NT)
    TARGET = game.exe
    LIBS = -lwinmm
    RM = del /Q
else
    TARGET = game
    LIBS = 
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LIBS)

clean:
	$(RM) $(TARGET)

run: $(TARGET)
ifeq ($(OS),Windows_NT)
	$(TARGET)
else
	./$(TARGET)
endif


# compile: make
# clean: make clean
# run: make run

# windows run: mingw32-make run
# windows clean: mingw32-make clean