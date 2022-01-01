#include <SDL2/SDL.h>
#include <cairo/cairo.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, const char* argv)
{
  SDL_Window* window = NULL;
  SDL_Surface* surf = NULL;
  SDL_Texture* tex = NULL;
  Uint32 rmask, gmask, bmask/*, amask*/;

  SDL_Rect destr;
  destr.x = 0;
  destr.y = 0;
  destr.w = SCREEN_WIDTH;
  destr.h = SCREEN_HEIGHT;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
#endif

  surf = SDL_CreateRGBSurface(
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    32,
    rmask, gmask, bmask, 0
  );

  if (surf == NULL) {
    SDL_Log("SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
    exit(1);
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "could not initialize SDL2: %s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow(
    "Soundmaker",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Renderer* renderer;
  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_LockSurface(surf);

  cairo_surface_t *cairosurf;
  cairo_t* cr;

  cairosurf = cairo_image_surface_create_for_data (
    surf->pixels,
    CAIRO_FORMAT_RGB24,
    surf->w,
    surf->h,
    surf->pitch
  );
  
  cr = cairo_create(cairosurf);

  cairo_pattern_t *pat;

  pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, 256.0);
  cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
  cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
  cairo_rectangle (cr, 0, 0, 256, 256);
  cairo_set_source (cr, pat);
  cairo_fill (cr);
  cairo_pattern_destroy (pat);

  pat = cairo_pattern_create_radial (115.2, 102.4, 25.6,
                                   102.4,  102.4, 128.0);
  cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
  cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
  cairo_set_source (cr, pat);
  cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
  cairo_fill (cr);
  cairo_pattern_destroy (pat);

  SDL_UnlockSurface(surf);
  
  tex = SDL_CreateTextureFromSurface(renderer, surf);

  if (tex == NULL) {
    fprintf(stderr, "CreateTexture failed: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_FreeSurface(surf);

  SDL_RenderClear(renderer);
  if ((SDL_RenderCopy(renderer, tex, NULL, &destr)) < 0) {
    fprintf(stderr, "error: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_RenderPresent(renderer);

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
  }

  cairo_destroy(cr);
  cairo_surface_destroy(cairosurf);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
