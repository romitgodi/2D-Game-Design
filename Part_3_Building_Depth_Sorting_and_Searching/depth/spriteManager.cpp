#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "spriteManager.h"
#include <algorithm>

class ScaledSpriteCompare {
public:
	bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
		return lhs->getScale() < rhs->getScale();
	}
};

SpriteManager& SpriteManager::getInstance() {
	static SpriteManager instance;
	return instance;
}

SpriteManager::SpriteManager() :
 sprites(),
 currentSprite(0),
 fireSurf(IOManager::getInstance().loadAndSet(Gamedata::getInstance().getXmlStr("fire/file"), Gamedata::getInstance().getXmlBool("fire/transparency")) ),
 fire(),
 fireCount(Gamedata::getInstance().getXmlInt("fire/count")),
 back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
 front("front", Gamedata::getInstance().getXmlInt("front/factor") ),
 cloud("cloud", Gamedata::getInstance().getXmlInt("cloud/factor") )
{  
  sprites.push_back( new Player("contra") );
  setBlaze();
}

const Drawable* SpriteManager::getSpriteObject() const {
	return sprites[currentSprite];
}

int SpriteManager::getCurrentSprite() const {
	return currentSprite;
}

void SpriteManager::drawSprites() const {
    back.draw();
	for (int i = 0; i < (3 *(fireCount/4)); ++i ) {	
	    fire[i]->draw();
	}
    front.draw();
	for (int i = fireCount/2; i < (3 *(fireCount/4)); ++i ) {	
	    fire[i]->draw();
	}

	for (unsigned i = 0; i < sprites.size(); ++i) {
		sprites[i]->draw();
	}
	for (int i = (3 *(fireCount/4)); i < fireCount; ++i ) {	
	    fire[i]->draw();
	}
    cloud.draw();
}

void SpriteManager::updateSprites(int ticks) {
	for (unsigned int i = 0; i < sprites.size(); ++i) {
		sprites[i]->update(ticks);
	}
	for (int i = 0; i < fireCount; ++i ) {	
	    fire[i]->update(ticks);
	}	
    back.update();
  	front.update();
  	cloud.update();
}

void SpriteManager::trackSprite() {
	currentSprite = (currentSprite+1) % sprites.size();
}

SpriteManager::~SpriteManager() { 
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
   SDL_FreeSurface(fireSurf);
  for (int i = 0; i < fireCount; ++i) {
  	delete fire[i];
  }
  fire.clear();
}

void SpriteManager::setBlaze() {
    fire.reserve(fireCount);
	for (int i = 0; i < fireCount; ++i ) {	
	    fire.push_back( new ScaledSprite("fire", fireSurf, i) );
	}
	sort(fire.begin(), fire.end(), ScaledSpriteCompare() );	 
}
