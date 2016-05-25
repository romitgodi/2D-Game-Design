#include <iostream>
#include <string>
#include <iomanip>
#include "vector2f.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"

Manager::~Manager() { }

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  HUD_HEIGHT(300),
  HUD_WIDTH(300),
  screen( io.getScreen() ),
  viewport( Viewport::getInstance() ),
  spriteHandler(SpriteManager::getInstance()),
  
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  HUDkey(true),
  timer(0)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
 
  viewport.setObjectToTrack(spriteHandler.getSpriteObject());
}

void Manager::drawHUD(SDL_Surface* screen, int x, int y) const {
  const Uint32 BLUE = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, x+HUD_WIDTH,y+HUD_HEIGHT/2, HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, x,y, x+HUD_WIDTH,y, BLUE);
  Draw_AALine(screen, x,y, x,y+HUD_HEIGHT, BLUE);
    
  io.printMessageAt("Contra Game", 75, 35);  
  io.printMessageValueAt("Seconds: ", clock.getSeconds(), 15, 50);
  io.printMessageValueAt("fps: ", clock.getAvgFps(), 15, 75);
  
  io.printMessageAt("UP arrow to move up", 15, 125);  
  io.printMessageAt("DOWN arrow to move down", 15, 145);  
  io.printMessageAt("RIGHT arrow to move right", 15, 165);  
  io.printMessageAt("LEFT arrow to move left", 15, 185);
  io.printMessageAt("SPACE button to fire", 15, 205);
  io.printMessageAt("G for God Mode", 15, 235);
  io.printMessageAt("R for Reset", 15, 255);
  
  spriteHandler.drawBulletHUD();
}


void Manager::draw() const {
  spriteHandler.drawSprites();
  viewport.draw();

  io.printMessageAt("Press F1 to toggle HUD", 45, 10);

  if (HUDkey) drawHUD(screen, 5, 30);
  io.printMessageAt(title, 30, 450);

   if (spriteHandler.isGodModeOn() ) {
  	io.printMessageAt("GOD Mode", 380, 50);  
  }

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  spriteHandler.updateSprites(ticks);
 
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  viewport.update(); // always update viewport last
   if ( (timer = clock.getSeconds() ) == 3 ) {
  	HUDkey = false;
  }
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool flag = false;

  while ( not done ) {
	  SDL_PollEvent(&event);
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
	  if(event.type == SDL_KEYUP) { 
		  flag = false;
    }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }

     if ( keystate[SDLK_r] ) {
      	    spriteHandler.playerResetPosition();
      }

      if (keystate[SDLK_RIGHT] && keystate[SDLK_LEFT]) {
      	    spriteHandler.playerStop();      
      }
      
      else if (keystate[SDLK_RIGHT] && keystate[SDLK_SPACE] ) {
            spriteHandler.playerBlast();
      }
      else if (keystate[SDLK_LEFT] && keystate[SDLK_SPACE] ) {
            spriteHandler.playerBlast();     
      }     
      
      else if(keystate[SDLK_RIGHT]) {   		
		    spriteHandler.playerRight();
      }
      
      else if(keystate[SDLK_LEFT]) {	
		    spriteHandler.playerLeft();
      }
      
      if (keystate[SDLK_UP] && keystate[SDLK_DOWN]) {
      	    spriteHandler.playerStop();      
      }
      
      else if (keystate[SDLK_UP] && keystate[SDLK_SPACE] ) {
            spriteHandler.playerBlast();      
      }
      else if (keystate[SDLK_DOWN] && keystate[SDLK_SPACE] ) {
            spriteHandler.playerBlast();     
      }
              
      else if(keystate[SDLK_DOWN]) {
       	       spriteHandler.playerDown();
      }
      
      else if(keystate[SDLK_UP]) {
       	       spriteHandler.playerUp();
      }
      
      if (  keystate[SDLK_SPACE] && keystate[SDLK_RIGHT] && keystate[SDLK_LEFT] ) {
        spriteHandler.playerStop();
      }
      
      else if (  keystate[SDLK_SPACE]  && keystate[SDLK_LEFT] ) {
        spriteHandler.playerStop();
      }
      
      else if (  keystate[SDLK_SPACE] && keystate[SDLK_RIGHT] ) {
	      spriteHandler.playerStop();
      }
      
      else if (  keystate[SDLK_SPACE]  && keystate[SDLK_DOWN]) {
            spriteHandler.playerStop();
      }
      
      else if (  keystate[SDLK_SPACE]  && keystate[SDLK_UP]) {
            spriteHandler.playerStop();
      }

      else if (keystate[SDLK_SPACE]){
       	    spriteHandler.playerBlast();       	 
      }  
      
      if ( keystate[SDLK_g] && !flag) {
		  flag = true; 
      		spriteHandler.playerGodMode();	
      }
        
      if ( keystate[SDLK_F1] && !flag) {
		flag = true;
       	if (HUDkey) {
       		HUDkey = false;
       	}
       	else {
       		HUDkey = true;
       	}       
      }
     		
        if ( keystate[SDLK_t] && !flag) {
			flag = true;
			spriteHandler.trackSprite();
		    viewport.setObjectToTrack(spriteHandler.getSpriteObject());
        }
        if ( keystate[SDLK_p] && !flag) {
			flag = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
        }
        if (keystate[SDLK_s] && !flag) {
			flag = true;
            clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    draw();
    update();
  }
}
