#ifndef _ENEMY_H
#define _ENEMY_H


#include "player.h"
#include "contra.h"
#include "sound.h"

class Enemy : public Player {
public:
	Enemy(const std::string&,Contra* p);
	Enemy(const Enemy&);					

	virtual void draw() const;
	virtual void update(Uint32 ticks);	

	virtual void moveRight();
	virtual void moveLeft();
	virtual void moveUp();
	virtual void moveDown();
	virtual void blast();
	

        virtual	void decrementHealthBar() {
        	healthBar.decrementEnemy();
        }
        virtual bool isHealthEmpty() {
        	return healthBar.isFull();
        }
      	virtual void resetHealth() {
		healthBar.resetReverse();
	}		

	void UpdatePos(const Vector2f& pos) {
		contraPos = pos;
		angle = getDirection( pos, this->getPosition() );

		if ( ( distanceToContra < firingDistance) && (!enemyFire) ) {
			enemyFire = true;
		}
		else if ( distanceToContra > firingDistance){
			enemyFire = false;
		}		
	}
	void setMODE(int d) {
		MODE = d;
	}
private:
	enum {NORMAL, EVADE};
	Vector2f contraPos;
	float firingDistance;
	bool enemyFire;
	float distanceToContra;
	float angle;
	float blastInterval;
	float timeSinceLastBlast;
	SDLSound& sound;	
	int MODE;
	
	const Enemy& operator=(const Enemy&);			
	
	float getDirection(const Vector2f&, const Vector2f& );
	void rePosition();
};
#endif
