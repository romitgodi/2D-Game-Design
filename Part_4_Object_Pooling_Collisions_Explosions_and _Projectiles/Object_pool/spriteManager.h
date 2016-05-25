#include <vector>
#include <list>
#include <SDL.h>
#include "ioManager.h"
#include "viewport.h"
#include "multisprite.h"
#include "sprite.h"
#include "world.h"
#include "player.h"
#include "scaledSprite.h"
#include "explodingSprite.h"
#include "vector2f.h"
#include "ioManager.h"

class SpriteManager {
public:
  static SpriteManager& getInstance();
  const Drawable* getSpriteObject() const;
  int getCurrentSprite() const;
  void drawSprites() const;
  void updateSprites(int ticks);
  void trackSprite();
  ~SpriteManager (); 

void playerStop();
void playerLeft();
void playerRight();
void playerUp();
void playerDown();
void playerBlast();
void playerResetPosition();
void playerGodMode() {
	godMode = !godMode;
}
bool isGodModeOn() const {
	return godMode;
}

bool checkForPlayerCollisions() const;
void checkForBulletCollisions();
void playerHealthDecrement() {
         static_cast<Player* >(sprites[0])->decrementHealthBar();
}
bool playerHealthIsZero() {
	return	static_cast<Player* >(sprites[0])->isHealthEmpty();
}

void drawBulletHUD() {
	static_cast<Player* >(sprites[0])->drawHUD();	
}


private:
  World back;
  World front;
  World cloud;
  
  std::vector<Drawable *> sprites;
  int currentSprite;
  
  SDL_Surface* fireSurf;
  std::list<ScaledSprite * > fire;
  
  bool playerExploded;
  bool fireExploded;
		
  ExplodingSprite* explodePlayerPtr;
  ExplodingSprite* explodeFirePtr;
    
  int fireCount;
  bool godMode;
		
  SpriteManager ();
  SpriteManager (const SpriteManager&); 
  const SpriteManager& operator=(const SpriteManager&);
  
  void setBlaze();
  void explodePlayerNow();
  void explodeFireNow() {
	checkForBulletCollisions();
}
};
