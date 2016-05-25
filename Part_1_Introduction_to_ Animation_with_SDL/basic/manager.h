#include <SDL.h>
#include <vector>
#include "ioManager.h"
#include "clock.h"
#include "sprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  
  int backRed;
  int backGreen;
  int backBlue;

  SDL_Surface * spiralSurface;
  const Frame * const spiralFrame;
  std::vector<Sprite> spiral;

  SDL_Surface * const backgSurf;
  const Frame * const backgFrame;
  Sprite backg;
  
  bool makeVideo;
  int frameCount;
  std::string username;
  int frameMax;
  const std::string TITLE;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void drawBackground() const;
};
