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
		while (true) {
			bool validPositionFound = true;
			// boulders must be between x=0 and x=60 inclusive
			x = rand() % 61;
			// also between y=20 and y=56 inclusive
			y = rand() % 37 + 20;
            // Anything that is nullptr belongs to the central hole so should be ignored
            if (m_dirt[x][y] == nullptr) continue;
			// iterate game objects loop, check all positions
			for (std::list<Actor*>::iterator curr_actor = m_game_objects.begin(); curr_actor != m_game_objects.end(); ++curr_actor){
				if (!isValidRadius(x,y, (*curr_actor)->getX(), (*curr_actor)->getY())){
					validPositionFound = false;
				}
			}
			if (validPositionFound){
				break;
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
    insertGameObject<OilBarrel>(m_barrelCount,0,60,20,56);
    insertGameObject<GoldNugget>(m_nuggetCount,0,60,0,56);


	return GWSTATUS_CONTINUE_GAME;
}

// TODO things being placed with half in the middle hole
template<class T>
void StudentWorld::insertGameObject(int count, int xLower, int xUpper, int yLower,  int yUpper) {
    for(int i=0; i < count; i++) {
        int x,y;
        while (true) {
            bool validPositionFound = true;
            // boulders must be between x=0 and x=60 inclusive
            x = rand() % (xUpper+1 - xLower) + xLower;
            // also between y=20 and y=56 inclusive
            y = rand() % (yUpper+1 - yLower) + yLower;
            // Anything that is nullptr belongs to the central hole so should be ignored TODO only valid if used in intialzization not during game
            if (m_dirt[x][y] == nullptr) continue;
            // iterate game objects loop, check all positions
            for (std::list<Actor*>::iterator curr_actor = m_game_objects.begin(); curr_actor != m_game_objects.end(); ++curr_actor){
                if (!isValidRadius(x,y, (*curr_actor)->getX(), (*curr_actor)->getY())){
                    validPositionFound = false;
                }
            }
            if (validPositionFound){
                break;
            }
        }
        m_game_objects.push_back(new T(this, x, y));
        // delete dirt at the location of the boulder
        // TODO THIS CODE IS COPYPASTA FROM ACTOR>CPP
        // TODO PUT FOR LOOP IN DeleteDirt method
        for (int k=0; k <= 3; k++){
            for (int j=0; j <= 3; j++){
                // donothign rn TODO
            }
        }
    }
}

int StudentWorld::move()
{
	m_frackMan->doSomething();
    // Clean up all dead objects
	for (std::list<Actor*>::iterator curr_actor = m_game_objects.begin(); curr_actor != m_game_objects.end();){
        // If the thing is dead
        if(!(*curr_actor)->isAlive()){
            // get the currrent actor to be deleted
            Actor* temp = *curr_actor;
            // erase actor from list
            curr_actor = m_game_objects.erase(curr_actor);
            // delete actor
            delete temp;

        } else {
            (*curr_actor)->doSomething();
            ++curr_actor;
        }
	}

    // TODO actual probabuility algorithm
    // make a new goodie, either water or sonar if certain probability
    // TODO sonar
    if (rand() % 50 == 1){
		// TODO put in different locations, i.e actually be random
        for(int i=0;i<64;i++){
            // flag used to break out of outer loop
            bool flag = false;
            for(int j=0;j<64;j++){
                if (isClear(i,j)){
                    m_game_objects.push_back(new WaterPool(this,i,j));
                    flag = true;
                    break;
                }
            }
            if (flag) break;
        }
        // All sonar Kits must be added at x=0, y=60
		m_game_objects.push_back(new SonarKit(this,0,60));

	}
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::addActor(Actor *actor) {
    m_game_objects.push_back(actor);
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
	// free entire list
	m_game_objects.clear();

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
    m_barrelCount = 2;
    m_nuggetCount = 5;

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

float StudentWorld::calculateRadius(int x1, int y1, int x2, int y2) const{
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

// check if the radius si valid
// since no objects can be within 6 squares of any other object
bool StudentWorld::isValidRadius(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)) > 6.0;
}

FrackMan* StudentWorld::findNearbyFrackMan(Actor *a, int radius) const {
    if (calculateRadius(a->getX(), a->getY(),m_frackMan->getX(), m_frackMan->getY()) <= radius) {
        return m_frackMan;
    }
    return nullptr;
}

// check if 4x4 block is clear
bool StudentWorld::isClear(int x,int y) const{
    for (int k=0; k <= 3; k++){
        for (int j=0; j <= 3; j++){
            if (m_dirt[x+k][y+j] != nullptr) {
                return false;
            }
        }
    }
    return true;
}
