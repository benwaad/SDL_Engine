#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL_blendmode.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_oldnames.h"
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include <cstdio>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

bool init();
void close();

class Texture {
public:
  Texture();
  ~Texture();

  bool loadFromFile(std::string path);
  void free();
  void render(float x, float y, SDL_FRect* clip=nullptr);

  void setBlendMode(SDL_BlendMode blend_mode);
  void setAlpha(Uint8 alpha);
  void setColor(Uint8 red, Uint8 green, Uint8 blue);

  float getWidth() {return _width;}
  float getHeight() {return _height;}

private:
  SDL_Texture* _texture;
  float _width;
  float _height;
};

int main() {
  init();

  Texture foo;
  foo.loadFromFile("media/foo.png");
  foo.setBlendMode(SDL_BLENDMODE_BLEND);
  // SDL_FRect foo_body_rect = {0, foo.getHeight()/2, foo.getWidth(), foo.getHeight()/2};

  Texture background;
  background.loadFromFile("media/background.png");

  // Modulation colors
  Uint8 r = 255;
  Uint8 g = 255;
  Uint8 b = 255;
  Uint8 a = 255;

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    // Handle input
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
      // Modify modulation
      if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_u:
            r -= 32;
            break;
          case SDLK_i:
            r += 32;
            break;
          case SDLK_j:
            g -= 32;
            break;
          case SDLK_k:
            g += 32;
            break;
          case SDLK_n:
            b -= 32;
            break;
          case SDLK_m:
            b += 32;
            break;
          case SDLK_DOWN:
            a -= 32;
            break;
          case SDLK_UP:
            a += 32;
            break;
        }
      }
    }

    // Render to screen 
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    background.setColor(r, g, b);
    background.render(0, 0);

    foo.setAlpha(a);
    foo.render(240, 190);
    SDL_RenderPresent(gRenderer);

  }


  close();
}

bool init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to initialize SDL! Error: %s", SDL_GetError());
    return false;
  }

  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
    printf("Failed to intialize SDL_image! Error: %s", SDL_GetError());
    return false;
  }
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("Failed to intialize SDL_image! Error: %s", SDL_GetError());
    return false;
  }

  gWindow = SDL_CreateWindow("Rendering", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!gWindow) {
    printf("Failed to create window! Error: %s", SDL_GetError());
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, nullptr, SDL_RENDERER_ACCELERATED);
  if (!gRenderer) {
    printf("Failed to create renderer! Error: %s", SDL_GetError());
    return false;
  }

  return true;
}

void close() {
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gRenderer = nullptr;
  gWindow = nullptr;

  IMG_Quit();
  SDL_Quit();
}

Texture::Texture() : _texture(nullptr), _width(0), _height(0) {}
Texture::~Texture() { free(); }
bool Texture::loadFromFile(std::string path) {
  free();

  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (!loaded_surface) {
    printf("Failed to load image %s! Error: %s", path.c_str(), SDL_GetError());
    return false;
  }

  SDL_SetSurfaceColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
  _texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
  if (!_texture) {
    printf("Unable to create texture from loaded image %s. SDL Error: %s", path.c_str(), SDL_GetError());
    SDL_DestroySurface(loaded_surface);
    return false;
  }

  _height = loaded_surface->h;
  _width  = loaded_surface->w;
  SDL_DestroySurface(loaded_surface);

  return true;
}
void Texture::free() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
    _height = 0;
    _width = 0;
  }
}
void Texture::render(float x, float y, SDL_FRect* clip) {
  SDL_FRect render_quad = {x, y, _width, _height};
  if (clip) {
    render_quad.w = clip->w;
    render_quad.h = clip->h;
  }
  SDL_RenderTexture(gRenderer, _texture, clip, &render_quad);
}
void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(_texture, red, green, blue);
}
void Texture::setBlendMode(SDL_BlendMode blend_mode) {
  SDL_SetTextureBlendMode(_texture, blend_mode);
}
void Texture::setAlpha(Uint8 alpha) {
  SDL_SetTextureAlphaMod(_texture, alpha);
}





