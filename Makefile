CC = clang
CFLAGS = -Iinclude -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2
LDFLAGS = -Llib -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib -lSDL2 -lSDL2_image
SRC = src/*.c
OBJ = $(SRC:.c=.o)
TARGET = build/debug/play

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

run:
	./build/debug/play

clean:
	rm -f $(OBJ) $(TARGET)
