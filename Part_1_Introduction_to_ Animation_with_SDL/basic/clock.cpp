#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"
#include "vector2f.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  started(false), 
  paused(false), 
  sloMo(false), 
  sumOfTicks(0),
  totalTicks(0)
  {
	  start();
}

Clock::Clock(const Clock& c) :
  ticks(c.ticks), 
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  sumOfTicks(c.sumOfTicks),
  totalTicks(c.totalTicks)
  {
  start();
}

void Clock::draw() const {
	IOManager::getInstance().printMessageValueAt("Seconds: ",getTicksSinceLastFrame()/1000,10,20);
	IOManager::getInstance().printMessageValueAt("Fps: ",getFps(),10,50);
}

void Clock::update() { 
	totalTicks = SDL_GetTicks();
	ticks = totalTicks - sumOfTicks;
	sumOfTicks += ticks;
}

unsigned int Clock::getTicksSinceLastFrame() const {
  return ticks;
}

void Clock::toggleSloMo() {
}

int Clock::getFps() const { 
	return 100;
}

void Clock::start() { 
  started = true; 
  paused = false; 
}

void Clock::pause() {
	paused = true;
}

void Clock::unpause() {
	paused = false;
}

