#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

/* initialize_window: initializes SDL and then creates window with given
params , then creates a renderer.
returns the state of the window -> 0 or 1 */
bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL Init Error: \n");
        return false;
    }
    
    window = SDL_CreateWindow(
        "Test",  // window name
        SDL_WINDOWPOS_CENTERED, // x axis for window to appear 
        SDL_WINDOWPOS_CENTERED,  // y axis to appear
        WINDOW_WIDTH, 
        WINDOW_HEIGHT , 
        SDL_WINDOW_SHOWN  // flags
    );
     
    if(!window) {
        fprintf(stderr, "SDL CreateWindow Error\n");
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window, 
        -1, // chose a driver: -1 is set to default
        SDL_RENDERER_ACCELERATED // flags
        | SDL_RENDERER_PRESENTVSYNC
    );

    // destroy window if error on renderer creation
    if (!renderer) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error\n");
        SDL_Quit();
        return false;
    }

    return true;
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
                break;
            }
    }
}

void update() {
    //TODO:
}

void render() {
    //TODO:
}

/* destroy_window: cleaning up processes after exiting main loop
destroys current renderer ,window and quits SDL */
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    // Set global is_running to current state of the window 
    is_running = initialize_window();
    
    // main game loop outputs current frame
    // cahnges behavior based on inputs processed
    // breaks: is_running -> 1 on window close or ESC input
    while(is_running) {
        process_input();
        update();
        render();
    }
    // Cleanup process on exiting main loop
    destroy_window();
     
    return EXIT_SUCCESS;
}
