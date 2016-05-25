#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "viewport.h"
#include "multisprite.h"
#include "sprite.h"


class SpriteManager {
public:
  static SpriteManager& getInstance();
  const Drawable* getSpriteObject() const;
  int getCurrentSprite() const;
  void drawSprites() const;
  void updateSprites(int ticks);
  void trackSprite();
  ~SpriteManager (); 

private:
  std::vector<Drawable *> sprites;
  int currentSprite;
  SpriteManager ();
  SpriteManager (const SpriteManager&); 
  const SpriteManager& operator=(const SpriteManager&);
};
