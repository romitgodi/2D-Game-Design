#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

Player::Player(const std::string& name ) :
	Drawable(name, Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
		 Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), Gamedata::getInstance().getXmlInt(name+"/speedY") ) ),
	frameSTAND( FrameFactory::getInstance().getFrames("cstand") ),
	frameX( FrameFactory::getInstance().getFrames("crun") ),
	frameUP( FrameFactory::getInstance().getFrames("cup") ),
	frameDOWN( FrameFactory::getInstance().getFrames("cdown") ),
	frameBLAST( FrameFactory::getInstance().getFrames("cstand" ) ),	
	curMovement(frameSTAND),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	currentFrame(0),
	numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
	timeSinceLastFrame( 0 ),
	frameWidth( frameSTAND[0]->getWidth() ),
	frameHeight(frameSTAND[0]->getHeight() ),
	Xdirection(true),
	Ydirection(true),
	keyPressedX(false),
	keyPressedY(false),
	blastKey(false),
	bulletName( Gamedata::getInstance().getXmlStr(name+"/Bullet") ),
	bullets( bulletName ),
	minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
	healthBar( Gamedata::getInstance().getXmlStr(name+"/healthhud")),
	strategy(new PerPixelCollisionStrategy)
 {}
 
Player::Player(const Player& s) :
	Drawable(s),
	frameSTAND( s.frameSTAND ),
	frameX( s.frameX ),
	frameUP( s.frameUP ),
	frameDOWN( s.frameDOWN ),
	frameBLAST( s.frameBLAST ),	
	curMovement( s.curMovement ),
	worldWidth( s.worldWidth ),
	worldHeight( s.worldHeight ),
	currentFrame( s.currentFrame ),
	numberOfFrames( s.numberOfFrames ),
	frameInterval( s.frameInterval ),
	timeSinceLastFrame( s.timeSinceLastFrame ),
	frameWidth( s.frameWidth),
	frameHeight( s.frameHeight ),
	Xdirection( s.Xdirection ),
	Ydirection( s.Ydirection ),
	keyPressedX( s.keyPressedX ),
	keyPressedY( s.keyPressedY),
	blastKey(s.blastKey),
	bulletName(s.bulletName),
	bullets(s.bullets),
	minSpeed(s.minSpeed),
	healthBar(s.healthBar),
	strategy(new PerPixelCollisionStrategy)
 { }
 
void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		currentFrame = (currentFrame+1) % (numberOfFrames );
		timeSinceLastFrame = 0;
	}
}

void Player::blast() {

	stop();

	blastKey = true;
	curMovement = frameBLAST;

	float y = Y();
	if ( Xdirection ) {
		float x = X() + getFrame()->getWidth();
 		bullets.shoot( Vector2f(x,y), Vector2f(abs(minSpeed+velocityX()), velocityY()) );
 	}
 	else {
		float x = X() - ( getFrame()->getWidth() ) / 2;
 		bullets.shoot( Vector2f(x,y), Vector2f( ( (-1.5 ) * ( abs( -minSpeed - velocityX() ) ) ), velocityY()) );
 	} 	
 	
}	


 
void Player::stop() {
	if (!keyPressedX) velocityX(0);
	if (!keyPressedY) velocityY(0);	
    curMovement = frameSTAND;
}

void Player::moveUp() {
	keyPressedY = true;	
	curMovement = frameUP;
}

void Player::moveDown() {
	keyPressedY = true;
	curMovement = frameDOWN;
}

void Player::moveLeft() {
	keyPressedX = true;
	Xdirection = false;
	if ( X() > 0 ) {
		velocityX(-250);
	}
	curMovement = frameX;
}

void Player::moveRight() {
	keyPressedX = true;
	Xdirection = true;	
	currentFrame = 1;			
	if ( X() > 0 ) {
		velocityX( 250);
	}
	curMovement = frameX;
}
 
void Player::draw() const {
Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if (Xdirection)
    curMovement[currentFrame]->draw(x, y);
  else 
    curMovement[currentFrame]->drawleft(x, y);
  bullets.draw();
  healthBar.draw();
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
		blastKey = false;
		bullets.update(ticks);
		healthBar.update(ticks);
}   	
  	
