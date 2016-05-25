#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		if(flag) {
			if(velocityX() > 0) {
				currentFrame = (currentFrame+1) % (numberOfFrames / 2);
			}
			else {
				currentFrame = ((currentFrame+1) % (numberOfFrames / 2)) + (numberOfFrames / 2);
			}
		}
		else {
			currentFrame = (currentFrame+1) % (numberOfFrames / 2);
		}

		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames(FrameFactory::getInstance().getFrames(name)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  flag(Gamedata::getInstance().getXmlInt(name+"/flag")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  Vector2f pos = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + pos);
  
   if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
    currentFrame = 0;
  }
  
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    currentFrame = ((currentFrame+1) % (numberOfFrames / 2)) + (numberOfFrames / 2);
  }  
}
