#include "contra.h"
#include "frameFactory.h"
#include "enemy.h"

Contra::Contra(const std::string& n) :
	Player(n),
	frameX( FrameFactory::getInstance().getFrames( Gamedata::getInstance().getXmlStr(n+"/run")) ),
	frameUP( FrameFactory::getInstance().getFrames( Gamedata::getInstance().getXmlStr(n+"/up") ) ),
	frameDOWN( FrameFactory::getInstance().getFrames(Gamedata::getInstance().getXmlStr(n+"/down") ) ),	
	playerList()	
{ }

Contra::Contra(const Contra& v) :
	Player(v),
	frameX( v.frameX ),
	frameUP( v.frameUP ),
	frameDOWN(v.frameDOWN),	
	playerList(v.playerList)
	{ }


void Contra::moveRight() {
	keyPressedX = true;
	Xdirection = true;
   	Ydirection = false;

	if ( X() > 0 ) {
		velocityX( 250 );
	}
	curMovement = frameX;
}

void Contra::moveLeft() {
	keyPressedX = true;
	Xdirection = false;
	Ydirection = false;

	if ( X() > 0 ) {
		velocityX( -250 );
	}
	curMovement = frameX;
}

void Contra::moveUp() {
	keyPressedY = true;
	Ydirection = true;

	curMovement = frameUP;
}

void Contra::moveDown() {
	keyPressedY = true;
	Ydirection = false;

	curMovement = frameDOWN;
}

void Contra::draw() const {
Uint32 x = static_cast<Uint32>(X());
Uint32 y = static_cast<Uint32>(Y());
  if (Xdirection)
  	curMovement[currentFrame]->draw(x, y);
  else 
  	curMovement[currentFrame]->drawleft(x, y);
  bullets.draw();
  healthBar.draw();  	
}

void Contra::update(Uint32 ticks) {
	advanceFrame(ticks);
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	Vector2f prevPos = getPosition();
	Vector2f curPos = getPosition() + incr;
	setPosition(getPosition() + incr);
	if ( prevPos != curPos ) {
		notify();
	}
	
	Vector2f ref;
		
	if ( Y() < 0) {
		velocityY( abs( velocityY() ) );
  	}
  	if ( Y() > worldHeight-frameHeight) {
  		Y(worldHeight-frameHeight);
		velocityY( -abs( velocityY() ) );
  	}

	if ( X() < 0) {
		X(0);
		velocityX( abs( velocityX() ) );
  	}
  
  	if ( X() > worldWidth-frameWidth) {
  		X(worldWidth-frameWidth);
    		velocityX( -abs( velocityX() ) );
    	}
    	if ( !keyPressedX ) stop();
  	if ( !keyPressedY ) stop();
    	keyPressedX = false;
    	keyPressedY = false;  	  
    	blastKey = false;
    	bullets.update(ticks);
    	healthBar.update(ticks);
}  	  	

void Contra::blast() {

	stop();

	blastKey = true;
	curMovement = (Ydirection)? frameUP:frameBLAST;
	
	if ( Xdirection ) {
		 if ( Ydirection ) {
		float y = Y();
		float x = X() + 10;
		bullets.shoot( Vector2f(x,y), Vector2f(velocityX(), (-1)*abs(minSpeed+velocityY())) );
	}
		float y = Y() + 7;
		float x = X() + getFrame()->getWidth();
 		bullets.shoot( Vector2f(x,y), Vector2f(abs(minSpeed+velocityX()), velocityY()) );
 	}
 	else {
		if ( Ydirection ) {
		float y = Y();
		float x = X() + 5;
		bullets.shoot( Vector2f(x,y), Vector2f(velocityX(), (-1)*abs(minSpeed+velocityY())) );
	}
		float y = Y() + 7;
		float x = X() - ( getFrame()->getWidth() ) / 2;
 		bullets.shoot( Vector2f(x,y), Vector2f( ( (-1.5 ) * ( abs( -minSpeed - velocityX() ) ) ), velocityY()) );
 	}
 	
}	

void Contra::notify() const {
		std::list<Player *>::const_iterator pItr = playerList.begin();
		while ( pItr != playerList.end() ) {
			if ( dynamic_cast<Enemy*>(*pItr) ) {
				static_cast<Enemy*>(*pItr++)->UpdatePos( getPosition() );
			}
		}
}
