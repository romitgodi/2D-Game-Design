#include <string>
#include <vector>

#include "drawable.h"
#include "frame.h"
#include "bulletPool.h"
#include "health.h"
#include "collisionStrategy.h"

class Player : public Drawable {
public:
	Player(const std::string&);
	Player(const Player&);
  	virtual ~Player() { 
  		delete strategy;
  	}
	
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	
	void stop();
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void blast();

virtual const Frame* getFrame() const { 
		return curMovement[currentFrame]; 
	}

	bool collidedWith(const Drawable* d) const {
		return strategy->execute(*this, *d);
	}
	

	void decrementHealthBar() {
		healthBar.decrement();
	}
	
	bool isHealthEmpty() {
		return healthBar.isEmpty();
	}
	
	void resetHealth() {
		healthBar.reset();
	}
	

	bool checkBulletCollisions(const Drawable* d) {
		return	bullets.hasBulletCollided(d);
	}	

	void drawHUD() {
		bullets.drawHUD();
	}
		
private:	
	const std::vector<Frame *> frameSTAND;
	const std::vector<Frame *> frameX;
	const std::vector<Frame *> frameUP;
	const std::vector<Frame *> frameDOWN;
	const std::vector<Frame *> frameBLAST;
	std::vector<Frame *> curMovement;
	
	int worldWidth;
	int worldHeight;

	unsigned currentFrame;
	unsigned numberOfFrames;
	unsigned frameInterval;
	float timeSinceLastFrame;

	int frameWidth;
	int frameHeight;

	bool Xdirection;
	bool Ydirection;	
	
	bool keyPressedX;
	bool keyPressedY;		
	bool blastKey;
	

	std::string bulletName;
	BulletPool bullets;
	float minSpeed;
	

	Health healthBar;
	

	CollisionStrategy* strategy;
	

	void advanceFrame(Uint32 ticks);
	const Player& operator=(const Player&);	
};
