#ifndef _CONTRA_H
#define _CONTRA_H

#include "player.h"

class Contra : public Player {
public:
	Contra(const std::string&);
	Contra(const Contra&);					

	virtual void draw() const;
	virtual void update(Uint32 ticks);	

	virtual void moveRight();
	virtual void moveLeft();
	virtual void moveUp();
	virtual void moveDown();
	virtual void blast();
	
        virtual	void decrementHealthBar() {
        	healthBar.decrement();
        }    
              
        
        virtual bool isHealthEmpty() {
        	return healthBar.isEmpty();
        }
        
       	virtual void resetHealth() {
		healthBar.reset();
	}

	void attach(Player* p) { 
		playerList.push_back(p);
	}
	void detach(Player* p) {
		std::list<Player *>::iterator pItr = playerList.begin();
		while ( pItr != playerList.end() ) {
			if ( p == (*pItr) ) {
				pItr = playerList.erase(pItr);
			}
			else {
				++pItr;
			}
		}	
	}
	void notify() const;
	
private:
	std::vector<Frame *> frameX;
	std::vector<Frame *> frameUP;
	std::vector<Frame *> frameDOWN;
	
	std::list<Player* > playerList;	
	
	const Contra& operator=(const Contra&);
};
#endif
