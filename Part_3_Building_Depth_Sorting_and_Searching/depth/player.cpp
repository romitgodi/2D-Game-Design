#include "player.h"
#include "gamedata.h"

Player::Player(const std::string& name ) :
 MultiSprite(name),
 keyPressedX(false),
 keyPressedY(false),
 initialV(250, 250)
 { }
 
Player::Player(const Player& s) :
 MultiSprite(s),
 keyPressedX( s.keyPressedX ),
 keyPressedY( s.keyPressedY ),
 initialV( s.initialV[0], s.initialV[1] ) 
 { }
 
void Player::stop() {
	if (!keyPressedX) velocityX(0);
	if (!keyPressedY) velocityY(0);	
	currentFrame = 6;
}

void Player::goUp() {
	keyPressedY = true;
	Ydirection = false;
	currentFrame = 8;
}

void Player::goDown() {
	keyPressedY = true;
	Ydirection = true;			        
	currentFrame = 7;
}

void Player::turnLeft() {
	keyPressedX = true;
	Xdirection = false;
	if ( X() > 0 ) {
		velocityX( -initialV[0] );
		currentFrame = 1;			
	}
}

void Player::turnRight() {
	keyPressedX = true;
	Xdirection = true;	
	currentFrame = 1;			
	if ( X() > 0 ) {
		velocityX( initialV[0] );
		currentFrame = 1;			
	}
}

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		if ( Xdirection ) {
			currentFrame = ( (currentFrame) % 4) + 1;
		}	
		else if (keyPressedY && (!Ydirection) ) {
			currentFrame = ( (currentFrame+1) % 1 ) + 8;
		}
		else if ( keyPressedY && Ydirection ) {
			currentFrame = ( (currentFrame+1) % 1 ) + 7;		
		}
		else {
			currentFrame = (( currentFrame + 1 ) % 2) + 5;
		}
		timeSinceLastFrame = 0;
	}	
}
 
void Player::draw() const {
Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if (Xdirection)
  	frames[currentFrame]->draw(x, y);
  else 
  	frames[currentFrame]->drawleft(x, y);
}

void Player::update(Uint32 ticks) {
	advanceFrame(ticks);
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);
	
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
}   	
  	
