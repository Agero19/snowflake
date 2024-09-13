CC = clang
CFLAGS = -Iinclude -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2
LDFLAGS = -Llib -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib -lSDL2 -lSDL2_image

# Use wildcard to get all .c files from src/
SRC = $(wildcard src/*.c)

# Convert .c files in SRC to .o files in OBJ
OBJ = $(SRC:src/%.c=build/debug/%.o)

TARGET = build/debug/play

# Create target directory if it doesn't exist
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

# Rule for compiling .c to .o in build/debug/
build/debug/%.o: src/%.c
	@mkdir -p build/debug
	$(CC) -c $< -o $@ $(CFLAGS)

run:
	./build/debug/play

clean:
	rm -f build/debug/*.o $(TARGET)
