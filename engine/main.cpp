#include <cstdio>
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

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
  void render(int x, int y);

  int getWidth() {return _width;}
  int getHeight() {return _height;}

private:
  SDL_Texture* _texture;
  int width;
  int height;
};

int main() {

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

Texture::Texture() : _texture(nullptr), _width(0), _height(0) {}
Texture::~Texture() { free(); }
bool Texture::loadFromFile(std::string path) {
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (!loaded_surface) {
    printf("Failed to load image %s! Error: %s", path, SDL_GetError());
    return false;
  }
}