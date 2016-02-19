#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	// initialise the dirt
	for(int i=0;i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			m_dirt[i][j] = new Dirt(i, j, this);
		}
	}

	// clear the central hole

	for(int i=30; i <= 33; i++) {
		for(int j = 4; j <= 59 ; j ++){
			delete m_dirt[i][j];
			// assign all deleted dirt to nullptr
			m_dirt[i][j] = nullptr;
		}
	}

	m_frackMan = new FrackMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_frackMan->doSomething();
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
	// set things to nullptr after being deleted to defend against double frees
	delete m_frackMan;
	m_frackMan = nullptr;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			delete m_dirt[i][j];
			m_dirt[i][j] = nullptr;
		}

	}
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
		{
			// make all teh dirt nullptr
			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 64; j++) {
					m_dirt[i][j] = nullptr;
				}

			}

		}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


// Destrucotr
StudentWorld::~StudentWorld() {
	// set things to nullptr after being deleted to defend against double frees
	delete m_frackMan;
	m_frackMan = nullptr;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			delete m_dirt[i][j];
			m_dirt[i][j] = nullptr;
		}

	}
}


// deletes dirt if possible and returns true if dirt is indeed deleted
bool StudentWorld::deleteDirt(int x, int y) {
	if (m_dirt[x][y] != nullptr){
		delete m_dirt[x][y];
		m_dirt[x][y] = nullptr;
		return true;
	}
	return false;
}
