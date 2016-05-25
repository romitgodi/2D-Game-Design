#include "multisprite.h"
#include "string"

class Player : public MultiSprite {
public:
	Player(const std::string&);
	Player(const Player&);
  	virtual ~Player() { }
	
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	
	void stop();
	void turnRight();
	void turnLeft();
	void goUp();
	void goDown();

private:
	bool keyPressedX;
	bool keyPressedY;
	Vector2f initialV;
	virtual void advanceFrame(Uint32 ticks);
};
