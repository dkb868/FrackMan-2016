#include "StudentWorld.h"
#include "Actor.h"
#include <cstdlib>
#include <cmath>
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


	// craete the bouldersTM
	// random coordiantes for now TODO
	for(int i=0; i < m_boulderCount; i++) {
		int x,y;
		bool validPositionFound = false;
		while (!validPositionFound) {
			// boulders must be between x=0 and x=60 inclusive
			x = rand() % 61;
			// also between y=20 and y=56 inclusive
			y = rand() % 37 + 20;
			// iterate game objects loop, check all positions
			if (isValidRadius(x,y,m_frackMan->getX(), m_frackMan->getY())){
				validPositionFound = true;
			}
		}
		m_game_objects.push_back(new Boulder(this, x, y));
		// delete dirt at the location of the boulder
		// TODO THIS CODE IS COPYPASTA FROM ACTOR>CPP
		// TODO PUT FOR LOOP IN DeleteDirt method
		for (int k=0; k <= 3; k++){
			for (int j=0; j <= 3; j++){
				deleteDirt(x+k,y+j);
			}
		}
		// assign to nullptr TODO get rid of all nullptrs
		m_dirt[x][y] = nullptr;

	}

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
	// TODO delete boulders
}

// Cosntructor
StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) {
	// make all teh dirt nullptr
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			m_dirt[i][j] = nullptr;
		}
	}
	// assign the boulder count for this level
	m_boulderCount = 2; // TODO algorithm

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

float StudentWorld::calculateRadius(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

// check if the radius si valid
// since no objects can be within 6 squares of any other object
bool StudentWorld::isValidRadius(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)) > 6.0;
}
