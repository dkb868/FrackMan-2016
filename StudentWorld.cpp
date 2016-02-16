#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	// intialzie dat dirt though
	for(int i=0;i < 64; i++) {
		for(int j=0; j <59;j++){
			m_dirt[i][j] = new Dirt(i,j,this);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	decLives();
	return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp(){

}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
		{
		}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
