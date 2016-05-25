#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "spriteManager.h"
#include "vector2f.h"
#include <algorithm>

class ScaledSpriteCompare {
public:
	bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
		return lhs->getScale() < rhs->getScale();
	}
};

SpriteManager::SpriteManager() :
 back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
 front("front", Gamedata::getInstance().getXmlInt("front/factor") ),
 cloud("cloud", Gamedata::getInstance().getXmlInt("cloud/factor") ),
 sprites(),
 currentSprite(0),
 fireSurf(IOManager::getInstance().loadAndSet(Gamedata::getInstance().getXmlStr("fire/file"), Gamedata::getInstance().getXmlBool("fire/transparency")) ),
 fire(),
 playerExploded( false ),
 enemyExploded( false ),
 fireExploded( false ),
 explodePlayerPtr( NULL ),
 explodeEnemyPtr( NULL ),
 explodeFirePtr( NULL ),
 fireCount(Gamedata::getInstance().getXmlInt("fire/count")),
 godMode(false),
 gameOver(false),
 sound(SDLSound::getInstance() )
{  
  sprites.push_back( new Contra("contra") );
  Contra* p = static_cast<Contra* >(sprites[0]);
  sprites.push_back( new Enemy("enemy", p));
  setBlaze();
}

SpriteManager::~SpriteManager() { 
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  sprites.clear();
  SDL_FreeSurface(fireSurf);

  std::list<ScaledSprite *>::iterator itr = fire.begin();
  ScaledSprite* tmp = NULL;
  while (itr != fire.end() ) {
  	tmp = *itr;
  	itr = fire.erase(itr);
  	delete tmp;
  }   
 if (explodePlayerPtr != NULL)  {
  	delete explodePlayerPtr;
  }
  if (explodeEnemyPtr != NULL) {
  	delete explodeEnemyPtr;
  }
}

const Drawable* SpriteManager::getSpriteObject() const {
	return sprites[currentSprite];
}

int SpriteManager::getCurrentSprite() const {
	return currentSprite;
}

void SpriteManager::drawSprites() const {
	std::list<ScaledSprite *>::const_iterator mItr = fire.begin();
	std::list<ScaledSprite *>::const_iterator cItr25 = fire.begin();
	std::list<ScaledSprite *>::const_iterator cItr75 = fire.begin();
	std::list<ScaledSprite *>::const_iterator mEtr = fire.end();
	
	std::advance(cItr25, static_cast<int>(fireCount/4));
	std::advance(cItr75, static_cast<int>(3 *(fireCount/4) ) );
    back.draw();
	
	while( mItr != cItr25 ) {	
		(*mItr)->draw();
		++mItr;
	}
	while( mItr != cItr75) {	
		(*mItr)->draw();
		++mItr;
	}
    front.draw();
	cloud.draw();

	while( mItr != mEtr) {	
		(*mItr)->draw();
		++mItr;
	}	

	if ( !playerExploded ) {
		sprites[0]->draw();
	}	
	
	if ( !enemyExploded ) {
		sprites[1]->draw();
	}	
	
	if ( playerExploded ) {
		explodePlayerPtr->draw();		
	}

	if ( enemyExploded ) {
		explodeEnemyPtr->draw();
	}
	
	if (fireExploded) {
		explodeFirePtr->draw();
	}	
}

void SpriteManager::updateSprites(int ticks) {
	for (unsigned int i = 0; i < sprites.size(); ++i) {
		sprites[i]->update(ticks);
	}
	for (std::list<ScaledSprite *>::iterator mItr = fire.begin(); mItr != fire.end(); ++mItr ) {	
		(*mItr)->update(ticks);
	}	
    back.update();
  	front.update();
  	cloud.update();

  	if ( checkForPlayerCollisions() && (!godMode)) {
		playerHealthDecrement();
		if ( playerHealthIsZero() ) {
			playerExploded = true;
			explodePlayerNow();
		}
	}
	
	if ( playerExploded )	{
		explodePlayerPtr->update(ticks);
	if ( explodePlayerPtr->isChunkEmpty() ) {
			delete explodePlayerPtr;
		    explodePlayerPtr = NULL;			
			playerExploded = false;
		    gameOver = true;
			static_cast<Contra* >(sprites[0])->resetHealth();		
		}	
	}

	if ( enemyExploded )	{
	    explodeEnemyPtr->update(ticks);
		if ( explodeEnemyPtr->isChunkEmpty() ) {
			delete explodeEnemyPtr;
		    explodeEnemyPtr = NULL;
		    enemyExploded = false;
		    gameOver = true;
			static_cast<Enemy* >(sprites[1])->resetHealth();
		}		
	}	
	
	if ( fireExploded ) {
		explodeFirePtr->update(ticks);
	}
	
	if (fireExploded && explodeFirePtr->isChunkEmpty() ) {
		fireExploded = false;
		ExplodingSprite* tmp = explodeFirePtr;
		explodeFirePtr = NULL;
		delete tmp;	
	}
	
	checkForBulletCollisions();
    playersBulletCollisionCheck();
}

void SpriteManager::trackSprite() {
	currentSprite = (currentSprite+1) % sprites.size();
}

void SpriteManager::setBlaze() {
for (int i = 0; i < fireCount; ++i ) {	
	    fire.push_back( new ScaledSprite("fire", fireSurf, i) );
	}
    fire.sort(ScaledSpriteCompare() ); 	 
}


void SpriteManager::playerStop() {
	static_cast<Contra* >(sprites[0])->stop();
}

void SpriteManager::playerLeft() {
	static_cast<Contra* >(sprites[0])->moveLeft();
}

void SpriteManager::playerRight() {
	static_cast<Contra* >(sprites[0])->moveRight();
}

void SpriteManager::playerUp() {
	static_cast<Contra* >(sprites[0])->moveUp();
}

void SpriteManager::playerDown() {	
	static_cast<Contra* >(sprites[0])->moveDown();
}

void SpriteManager::playerBlast() {
	static_cast<Contra* >(sprites[0])->blast();
}


bool SpriteManager::checkForPlayerCollisions() const {
	std::list<ScaledSprite *>::const_iterator sItr = fire.begin();
	std::list<ScaledSprite *>::const_iterator eItr = fire.end();
	std::advance(sItr, static_cast<int>(3 *(fireCount/4) ) );	
	while ( sItr != eItr ) {
		if ( static_cast<Contra* >(sprites[0])->collidedWith((*sItr) ) ) return true;		
		++sItr;
	}
	return false;
}

void SpriteManager::checkForBulletCollisions() {
	std::list<ScaledSprite *>::iterator sItr = fire.begin();
	std::list<ScaledSprite *>::iterator eItr = fire.end();
	std::advance(sItr, static_cast<int>(3 *(fireCount/4) ) );	
	while ( sItr != eItr ) {
		if ( static_cast<Contra* >(sprites[0])->checkBulletCollisions((*sItr) ) ) {
			explodeFirePtr = new ExplodingSprite("fire", (*sItr)->getPosition(), Vector2f(150, 150));
			sound[2];
			fireExploded = true;
			ScaledSprite* tmp = *sItr;
			sItr = fire.erase(sItr);
			delete tmp; 
			}
		else {
			++sItr;
		} 
	}	
}

void SpriteManager::explodePlayerNow() {
	if ( explodePlayerPtr != NULL ) {
		ExplodingSprite* tmp = explodePlayerPtr;
		explodePlayerPtr = NULL;
		delete tmp;	
	}
    Contra* p = static_cast<Contra *>(sprites[0]);
	explodePlayerPtr = new ExplodingSprite("contra", p->getPosition(), Vector2f(100, 100), p->getFrame() );		
}

void SpriteManager::explodeEnemyNow() {
	if ( explodeEnemyPtr != NULL ) {
		ExplodingSprite* tmp = explodeEnemyPtr;
	    explodeEnemyPtr = NULL;
		delete tmp;	
	}
    Enemy* p = static_cast<Enemy *>(sprites[1]);
    explodeEnemyPtr = new ExplodingSprite("enemy", p->getPosition(), Vector2f(100, 100), p->getFrame() );		
}

void SpriteManager::playerResetPosition() {
	static_cast<Contra* >(sprites[0])->setPosition( Vector2f(120, 402));
	static_cast<Contra* >(sprites[0])->resetHealth();
	static_cast<Contra* >(sprites[0])->clearBulletList();
	static_cast<Enemy* >(sprites[1])->resetHealth();
	godMode = false;
}

void SpriteManager::playersBulletCollisionCheck() {
	Player* contra = static_cast<Contra* >(sprites[0]);
	Player* enemy = static_cast<Enemy* >(sprites[1]);

	if ( contra->checkBulletCollisions(enemy) ) {
	    enemy->decrementHealthBar();
		static_cast<Enemy* >(enemy)->setMODE(1);
		if ( enemy->isHealthEmpty() ) {
		    enemyExploded = true;						
		    explodeEnemyNow();
		}
	}

	if ( ( enemy->checkBulletCollisions(contra)) && (!godMode) )  {
		 static_cast<Contra* >(contra)->decrementHealthBar();		
		if ( contra->isHealthEmpty() ) {
		    playerExploded = true;
		    explodePlayerNow();
		}
	}
}

void SpriteManager::deleteBlaze() {
  std::list<ScaledSprite *>::iterator itr = fire.begin();
  ScaledSprite* tmp = NULL;
  while (itr != fire.end() ) {
  	tmp = *itr;
  	itr = fire.erase(itr);
  	delete tmp;
  }
}
