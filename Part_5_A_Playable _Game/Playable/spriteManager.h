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
#include "contra.h"
#include "enemy.h"
#include "sound.h"

class SpriteManager {
public:
  SpriteManager ();
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
void playersBulletCollisionCheck();
void playerGodMode() {
	godMode = !godMode;
}
bool isGodModeOn() const {
	return godMode;
}

bool checkForPlayerCollisions() const;
void checkForBulletCollisions();
void playerHealthDecrement() {
         static_cast<Contra* >(sprites[0])->decrementHealthBar();
}
bool playerHealthIsZero() {
	return	static_cast<Contra* >(sprites[0])->isHealthEmpty();
}

void drawBulletHUD() {
	static_cast<Contra* >(sprites[0])->drawHUD();	
}

bool checkPlayer() const {
	return gameOver;
}

void resetPlayer() {
	gameOver = false;
}

void setBlaze();
void deleteBlaze();

private:
  World back;
  World front;
  World cloud;
  
  std::vector<Drawable *> sprites;
  int currentSprite;
  
  SDL_Surface* fireSurf;
  std::list<ScaledSprite * > fire;
  
  bool playerExploded;
  bool enemyExploded;
  bool fireExploded;
		
  ExplodingSprite* explodePlayerPtr;
  ExplodingSprite* explodeEnemyPtr;
  ExplodingSprite* explodeFirePtr;
    
  int fireCount;
  bool godMode;
  bool gameOver;
  SDLSound& sound;
  SpriteManager (const SpriteManager&); 
  const SpriteManager& operator=(const SpriteManager&);
  
  void explodePlayerNow();
  void explodeEnemyNow();  
};
