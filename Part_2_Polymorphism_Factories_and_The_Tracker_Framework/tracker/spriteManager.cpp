#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "spriteManager.h"


SpriteManager& SpriteManager::getInstance() {
	static SpriteManager instance;
	return instance;
}

SpriteManager::SpriteManager() : sprites(), currentSprite(0) {  
  sprites.push_back( new MultiSprite("hulk") );
  sprites.push_back( new MultiSprite("ironman"));
}

const Drawable* SpriteManager::getSpriteObject() const {
	return sprites[currentSprite];
}

int SpriteManager::getCurrentSprite() const {
	return currentSprite;
}

void SpriteManager::drawSprites() const {
	for (unsigned i = 0; i < sprites.size(); ++i) {
		sprites[i]->draw();
	}
}

void SpriteManager::updateSprites(int ticks) {
	for (unsigned int i = 0; i < sprites.size(); ++i) {
		sprites[i]->update(ticks);
	}
}

void SpriteManager::trackSprite() {
	currentSprite = (currentSprite+1) % sprites.size();
}

SpriteManager::~SpriteManager() { 
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }	
}
