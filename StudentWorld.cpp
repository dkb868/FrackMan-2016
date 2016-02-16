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
		for (int j = 0; j < 60; j++) {
			m_dirt[i][j] = new Dirt(i, j, this);
		}
	}

	for(int i=30; i <= 33; i++) {
		for(int j = 4; j <= 59 ; j ++){
			delete m_dirt[i][j];
		}
	}
	
	m_frackMan = new FrackMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
		{
		}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


// Destrucotr
// TODO delete other shits
StudentWorld::~StudentWorld() {
	delete m_frackMan;
}