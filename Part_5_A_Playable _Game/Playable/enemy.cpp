#include "enemy.h"
#include "frameFactory.h"
#include "cmath"
#include "gamedata.h"

Enemy::Enemy(const std::string& n, Contra* p) :
	Player(n),
    contraPos(),
	firingDistance( Gamedata::getInstance().getXmlInt( ( Gamedata::getInstance().getXmlStr(n+"/Bullet"))+"/distance")  ),
    enemyFire ( false ),
	distanceToContra(1000),
	angle(0),
	blastInterval(Gamedata::getInstance().getXmlInt( ( Gamedata::getInstance().getXmlStr(n+"/Bullet"))+"/interval")),
	timeSinceLastBlast(0),
	sound(SDLSound::getInstance() ),
	MODE(NORMAL)
	{ 
	 	Xdirection = false;
	 	p->attach(this);
	 }
 
Enemy::Enemy(const Enemy& f) :
	Player(f),
	contraPos(f.contraPos),
	firingDistance(f.firingDistance),
    enemyFire(f.enemyFire),
	distanceToContra(f.distanceToContra),
	angle(f.angle),
	blastInterval(f.blastInterval),
	timeSinceLastBlast(f.timeSinceLastBlast),
	sound(f.sound),
	MODE(f.MODE)
	{ }

void Enemy::draw() const {
Uint32 x = static_cast<Uint32>(X());
Uint32 y = static_cast<Uint32>(Y());
  if (Xdirection)
  	curMovement[currentFrame]->draw(x, y);
  else 
  	curMovement[currentFrame]->drawleft(x, y);
  bullets.draw();
  healthBar.drawReverse();
}
void Enemy::update(Uint32 ticks) {
	advanceFrame(ticks);
	timeSinceLastBlast += ticks;
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

	if ( X() < 4500) {
		X(4500);
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
    	rePosition();
    	
    	if ( enemyFire ) {
     		if ( timeSinceLastBlast > blastInterval ) {
    			blast();
    			if (timeSinceLastBlast > 1000) sound[1];
    		}    		
	}
	if ( timeSinceLastBlast > 1100 ) {
		timeSinceLastBlast = 0;
	}
	if ( MODE ) {
		Vector2f newPos( ((rand() % 400) + 4500) , 402);
		setPosition(newPos);
		MODE = NORMAL;
	}	
}	

void Enemy::moveRight() { 
	keyPressedX = true;
	Xdirection = true;
	if ( X() > 0 ) {
		velocityX( 250 );
	}
}
void Enemy::moveLeft() { 
	keyPressedX = true;
	Xdirection = false;
	if ( X() > 0 ) {
		velocityX( -250 );
	}
}
void Enemy::moveUp() { 
	keyPressedY = true;
}
void Enemy::moveDown() { 
	keyPressedY = true;
}	

void Enemy::blast() {

	blastKey = true;
	curMovement = frameBLAST;

	float y = Y();	
	
	float Xcomp = abs ( minSpeed * cos(angle * M_PI / 180) );
	float Ycomp = abs ( minSpeed * sin(angle * M_PI / 180) );

	float cX = contraPos[0];
	float cY = contraPos[1];	
	
	Xcomp = ( cX < X() )? (-1 * abs(Xcomp) ) : abs(Xcomp);
	Ycomp = ( cY < Y() )? (-1 * abs(Ycomp) ) : abs(Ycomp);
	
	if ( Xdirection ) {
		float x = X() + getFrame()->getWidth();
		bullets.shoot( Vector2f(x,y), Vector2f(abs( Xcomp + velocityX() ), (velocityY() + Ycomp) ) );				
	}
	else {
		float x = X() - ( getFrame()->getWidth() ) / 2;
		bullets.shoot( Vector2f(x,y), Vector2f( ( -(abs( Xcomp - velocityX() ) ) ), (velocityY() + Ycomp) ) );
	}	
}

float Enemy::getDirection(const Vector2f& A, const Vector2f& B) {
    distanceToContra = B[0] - A[0];
	float angle = 0;
	if (distanceToContra != 0 ) {
	    angle =  180;
	}
	return angle;
}

void Enemy::rePosition() {
	float cX = contraPos[0];
	float fX = this->X();
	if ( (fX < cX) && (!Xdirection) ) {
		Xdirection = true;		
	}
	else if ( (fX > cX) && (Xdirection)) {
		Xdirection = false;
	}
}
