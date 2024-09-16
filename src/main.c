#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include <math.h>

int is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int last_frame_time = 0;
float t = 0.0f;
float step = 0.005f;

struct Point {
    float x;
    float y;
};

struct Segment[] {
    struct Point p1;
    struct Point p2;
};

struct Triangle {
    struct Point p1;
    struct Point p2;
    struct Point p3;
};

struct Point lerp(struct Point start, struct Point end, float t) {
    struct Point result;
    result.x = start.x + t * (end.x - start.x);
    result.y = start.y + t * (end.y - start.y);
    return result;
}

// Function to draw a line from start to end incrementally
void drawTriangle(SDL_Renderer* renderer, struct Triangle triangle, float t) {
    // Interpolate points for each side of the triangle
    struct Point AB = lerp(triangle.p1, triangle.p2, t); // From A to B
    struct Point BC = lerp(triangle.p2, triangle.p3, t); // From B to C
    struct Point CA = lerp(triangle.p3, triangle.p1, t); // From C to A

    // Draw each side incrementally
    SDL_RenderDrawLine(renderer, triangle.p1.x, triangle.p1.y, AB.x, AB.y); // AB side
    SDL_RenderDrawLine(renderer, triangle.p2.x, triangle.p2.y, BC.x, BC.y); // BC side
    SDL_RenderDrawLine(renderer, triangle.p3.x, triangle.p3.y, CA.x, CA.y); // CA side
}

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

void setup() {

}

/* process_input: introduce events , and poll for any event possible
then act depending on the given event. Treat keyboard input.
Pressing close button -> initiates quit
*/
void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            // Pressing ESC closes window
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
                break;
            }
    }
}

void update() {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();    

    t += step;
    if (t > 1.0f) {
        t = 1.0f;  // Once fully drawn, stop increasing 't'
    }
    SDL_Delay(10);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderClear(renderer);

    struct Triangle triangle = {
        {X, Y - (2.0f / 3.0f) * TRI_HEIGHT},  // Top point (p1)
        {X - SIDE_LEN / 2.0f, Y + (1.0f / 3.0f) * TRI_HEIGHT},  // Bottom-left point (p2)
        {X + SIDE_LEN / 2.0f, Y + (1.0f / 3.0f) * TRI_HEIGHT}   // Bottom-right point (p3)
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    drawTriangle(renderer, triangle, t);

    SDL_RenderPresent(renderer); 
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
    

    setup();
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
