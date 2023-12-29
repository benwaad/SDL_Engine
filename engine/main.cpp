#include <iostream>
#include "defines.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 360;

struct KeyPressSurface
{
  enum Values
  {
    DEFAULT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    TOTAL
  };
};

bool init();
bool loadMedia();
SDL_Surface *loadSurface(std::string filename);
SDL_Texture *loadTexture(std::string filename);
void close();

// Window we render to
SDL_Window *gWindow = nullptr;
// Renderer for displaying textures
SDL_Renderer* gRenderer = nullptr;
// Surfaces depending on key stroke
SDL_Texture *gKeyPressTextures[KeyPressSurface::TOTAL];
// The image we will render on the window
SDL_Texture *gCurrentTexture = nullptr;

int main(int argc, char *argv[])
{

  if (!init())
  {
    return -1;
  }

  // SDL_FillSurfaceRect(gScreenSurface, nullptr, SDL_MapRGBA(gScreenSurface->format, 0, 150, 136, 255/3));
  if (!loadMedia())
  {
    return -2;
  }
  gCurrentTexture = gKeyPressTextures[KeyPressSurface::DEFAULT];
  SDL_FRect legendRect = {0.0, 0.0, SCREEN_WIDTH/5, SCREEN_HEIGHT/5};
  // legendRect.x = 0;
  // legendRect.y = 0;
  // legendRect.w = SCREEN_WIDTH / 10;
  // legendRect.h = SCREEN_HEIGHT / 10;

  bool running = true;
  while (running)
  {
    // SDL_BlitSurfaceScaled(gCurrentTexture, nullptr, gScreenSurface, &screenSurfaceRect, SDL_SCALEMODE_BEST);
    // SDL_UpdateWindowSurface(gWindow);

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        running = false;
      }
      else if (event.type == SDL_EVENT_KEY_DOWN)
      {
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
          gCurrentTexture = gKeyPressTextures[KeyPressSurface::UP];
          break;

        case SDLK_DOWN:
          gCurrentTexture = gKeyPressTextures[KeyPressSurface::DOWN];
          break;

        case SDLK_LEFT:
          gCurrentTexture = gKeyPressTextures[KeyPressSurface::LEFT];
          break;

        case SDLK_RIGHT:
          gCurrentTexture = gKeyPressTextures[KeyPressSurface::RIGHT];
          break;

        default:
          gCurrentTexture = gKeyPressTextures[KeyPressSurface::DEFAULT];
        }
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(gRenderer);
    SDL_RenderTexture(gRenderer, gCurrentTexture, nullptr, nullptr);

    SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(gRenderer, &legendRect);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderRect(gRenderer, &legendRect);

    SDL_RenderPresent(gRenderer);
  }

  int num_drivers = SDL_GetNumRenderDrivers();
  std::cout << "Number of render drivers: " << num_drivers << std::endl;
  std::cout << "Driver names:\n";
  for (int i = 0; i < num_drivers; i++) {
    std::cout << "  > " << SDL_GetRenderDriver(i) << std::endl;
  }

  close();
  return 0;
}

bool init() {
  
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Could not initialize SDL! Error: " << SDL_GetError() << std::endl;
    return false;
  }

  int init_flags = IMG_INIT_JPG;
  if (! (IMG_Init(init_flags) & init_flags)) {
    std::cout << "Could not initialize SDL_image! Error: " << IMG_GetError() << std::endl;
    return false;
  }

  gWindow = SDL_CreateWindow("Hello world image", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!gWindow) {
    std::cout << "Could not create window! Error: " << SDL_GetError() << std::endl;
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, nullptr, SDL_RENDERER_ACCELERATED);
  if (!gRenderer) {
    std::cout << "Could not create renderer! Error: " << SDL_GetError() << std::endl;
    return false;
  }
  SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RendererInfo info;
  SDL_GetRendererInfo(gRenderer, &info);
  std::cout << "Using renderer '" << info.name << "'" << std::endl;

  return true;
}

bool loadMedia() {
  gKeyPressTextures[KeyPressSurface::DEFAULT] = loadTexture("media/press.bmp");
  gKeyPressTextures[KeyPressSurface::UP]      = loadTexture("media/up.bmp");
  gKeyPressTextures[KeyPressSurface::DOWN]    = loadTexture("media/down.bmp");
  gKeyPressTextures[KeyPressSurface::LEFT]    = loadTexture("media/left.bmp");
  gKeyPressTextures[KeyPressSurface::RIGHT]   = loadTexture("media/right.bmp");

  for (int i = 0; i < KeyPressSurface::TOTAL; i++) {
    if (!gKeyPressTextures[i]) {
      std::cout << "Error! One or more surfaces failed to load" << std::endl;
      return false;
    }
  }

  return true;
}

SDL_Surface* loadSurface(std::string filename) {
  // int x,y,n;
  // unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
  // SDL_Surface* surf = SDL_CreateSurfaceFrom((void*)data, x, y, x*4, SDL_PIXELFORMAT_ABGR8888);
  SDL_Surface* surf = IMG_Load(filename.c_str());
  if (!surf) {
    std::cout << "Error! Failed to load surface " << filename << std::endl;
  }
  
  // Convert to screen format
  // SDL_Surface* opt = SDL_ConvertSurface(surf, gScreenSurface->format);
  // SDL_DestroySurface(surf);
  return surf;
}

SDL_Texture* loadTexture(std::string filename) {
  SDL_Surface* surface = loadSurface(filename);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
  SDL_DestroySurface(surface);
  return texture;
}

void close() {
  for (int i = 0; i < KeyPressSurface::TOTAL; i++) {
    SDL_DestroyTexture(gKeyPressTextures[i]);
    gKeyPressTextures[i] = nullptr;
  }

  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gRenderer = nullptr;
  gWindow = nullptr;
  
  IMG_Quit();
  SDL_Quit();
}