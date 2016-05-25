#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "viewport.h"
#include "multisprite.h"
#include "sprite.h"
#include "world.h"
#include "player.h"
#include "scaledSprite.h"

class SpriteManager {
public:
  static SpriteManager& getInstance();
  const Drawable* getSpriteObject() const;
  int getCurrentSprite() const;
  void drawSprites() const;
  void updateSprites(int ticks);
  void trackSprite();
  ~SpriteManager (); 

friend class Manager;

private:
  std::vector<Drawable *> sprites;
  int currentSprite;
  
  SDL_Surface* fireSurf;
  std::vector<ScaledSprite * > fire;
  int fireCount;
  void setBlaze();
		
  SpriteManager ();
  SpriteManager (const SpriteManager&); 
  const SpriteManager& operator=(const SpriteManager&);
  
  World back;
  World front;
  World cloud;
};
