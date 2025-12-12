#include <stdio.h>
#include <SDL2/SDL.h>

int main()
{

  FILE *in = stdin;
  char *pthroway = calloc(1000, sizeof(char));

  fgets(pthroway, 1000, in);
  // fgets(pthroway, 1000, in);  // only if the ppm file contains an comment on the second line

  char *pdimemsions = calloc(1000, sizeof(char));
  fgets(pdimemsions, 1000, in);
  fgets(pthroway, 1000, in);
  free(pthroway);

  int width = -1;
  int height = -1;
  sscanf(pdimemsions, "%d %d\n", &width, &height);
  free(pdimemsions);
  printf("width = %d, height = %d", width, height);

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *pwindow = SDL_CreateWindow(
      "Image Viewer",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width, height,
      0);

  if (!pwindow)
  {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  SDL_Rect pixel = (SDL_Rect){0, 0, 1, 1};
  for (int x = 0; x < height; x++)
  {
    for (int y = 0; y < width; y++)
    {
      Uint8 r, g, b;
      r = (char) getchar();
      g = (char) getchar();
      b = (char) getchar();
      Uint32 color = SDL_MapRGB(psurface->format, r, g, b);
      pixel.x = y;
      pixel.y = x;
      SDL_FillRect(psurface, &pixel, color);
    }
  }

  SDL_UpdateWindowSurface(pwindow);

  SDL_Event event;
  int running = 1;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = 0;
      }
    }
    SDL_Delay(16); // ~60 FPS loop
  }

  SDL_DestroyWindow(pwindow);
  SDL_Quit();
  return 0;
}
