#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *pwindow = SDL_CreateWindow(
        "Image Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        0
    );

    if (!pwindow) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

    Uint8 r,g,b;
    r = 0xFF;
    g = b = 0;
    Uint32 color = SDL_MapRGB(psurface->format, r, g, b);

    SDL_Rect pixel = (SDL_Rect){0,0,1,1};
    for(int x=0; x<WIDTH; x++){
      for(int y=0; y<HEIGHT; y++){
        pixel.x = x;
        pixel.y = y;
        SDL_FillRect(psurface, &pixel, color);
      }
    }

    SDL_UpdateWindowSurface(pwindow);

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(16); // ~60 FPS loop
    }

    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    return 0;
}
