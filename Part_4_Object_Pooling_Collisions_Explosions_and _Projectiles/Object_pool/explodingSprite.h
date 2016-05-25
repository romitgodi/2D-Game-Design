#include <vector>
#include <list>
#include "chunk.h"

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite& s);
  ExplodingSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel);
  ExplodingSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel, const Frame* frm);
  virtual ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunkListPtr->size(); }
  unsigned int freeCount()  const { return freeListPtr->size(); }
  bool isChunkEmpty() const {
  	return ( chunkListPtr->size() == 0 );
  }
  
  void resetChunks(const Drawable* d);
  void resetChunks();
  
  ExplodingSprite(const ExplodingSprite&);
 

  
private:
  std::list<Chunk> chunks;
  std::list<Chunk> freeList;
  std::vector<Frame*> frames;
  
  std::list<Chunk> *chunkListPtr;		// Points to the chunk List
  std::list<Chunk> *freeListPtr;		// Points to the free List
 
  ExplodingSprite& operator=(const ExplodingSprite&);
};
