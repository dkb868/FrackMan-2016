#include "StudentWorld.h"
#include "Actor.h"
#include <vector>
#include <queue>
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

            // if x,y is in the middle shaft continue
            // i.e if the 4x4 block fo x has an empty spot, continue
            // TODO make this a function?
            bool flag = false;
            for (int k=0; k <= 3; k++){
                for (int j=0; j <= 3; j++){
                    if (m_dirt[x+k][y+j] == nullptr) {
                        flag = true;
                        break;
                    }
                }
                if (flag) break;
            }
            if (flag) continue;
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

    // add a random protester
    m_game_objects.push_back(new RegularProtester(this));
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
            // TODO make this a function?
            bool flag = false;
            for (int k=0; k <= 3; k++){
                for (int j=0; j <= 3; j++){
                    if (m_dirt[x+k][y+j] == nullptr) {
                        flag = true;
                        break;
                    }
                }
                if (flag) break;
            }
            if (flag) continue;
            // Anything that is nullptr belongs to the central hole so should be ignored TODO only valid if used in intialzization not during game
            // ensure that there is dirt where the object is being places since the barrels and ngugest must be in dirt
            if (m_dirt[x][y] == nullptr) continue;
            // iterate game objects loop, check all positions
           // for (std::list<Actor*>::iterator curr_actor = m_game_objects.begin(); curr_actor != m_game_objects.end(); ++curr_actor){
               for (Actor* curr_actor : m_game_objects){
                if (!isValidRadius(x,y, (curr_actor)->getX(), (curr_actor)->getY())){
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
    // if frackman is dead, immediately restart the level
    if (!m_frackMan->isAlive()){
        return GWSTATUS_PLAYER_DIED;
    }
	m_frackMan->doSomething();
    // check if all oil barrels were picked up
    bool allOilPickedUp = true;
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
            // if there is still oil on the field, all the oil has not been picked up
            if ((*curr_actor)->needsToBePickedUpToFinishLevel()){
                allOilPickedUp = false;
            }

            ++curr_actor;
        }
	}
    // if all the oil was picked up, the level is won
    if (allOilPickedUp){
        // make sound
        return GWSTATUS_FINISHED_LEVEL;
    }


    // TODO actual probabuility algorithm
    // make a new goodie, either water or sonar if certain probability
    // TODO sonar
    // int probabilityOfGoodie = getLevel() * 25 + 300;
    int probabilityOfGoodie = 20; // TODO above
    // TODO or +1?
    if (rand() % probabilityOfGoodie == 1){
        // 1/5 probaility of sonar kit
        if(rand() % 5 ==1){
            // All sonar Kits must be added at x=0, y=60
            m_game_objects.push_back(new SonarKit(this));
        } else {
            vector<Coordinate*> potentialCoordiantes;
            // TODO put in different locations, i.e actually be random
            for(int i=0;i<64;i++){
                for(int j=0;j<64;j++){
                    if (isClear(i,4,j,4)){
                        potentialCoordiantes.push_back(new Coordinate(i,j));
                    }
                }
            }
            // pick random coord from the vector
            long randIndex = rand() % potentialCoordiantes.size();
            Coordinate choosenCoordiante = *potentialCoordiantes[randIndex];
            m_game_objects.push_back(new WaterPool(this,choosenCoordiante.getX(),choosenCoordiante.getY()));
            for (int i=0;i<potentialCoordiantes.size();i++){
                delete potentialCoordiantes[i];
            }
        }


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
    // delete all game objects
    for (std::list<Actor*>::iterator curr_actor = m_game_objects.begin(); curr_actor != m_game_objects.end();){
        // If the thing is dead
        // get the currrent actor to be deleted
        Actor* temp = *curr_actor;
        // erase actor from list
        curr_actor = m_game_objects.erase(curr_actor);
        // delete actor
        delete temp;
    }

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
bool StudentWorld::isValidRadius(int x1, int y1, int x2, int y2) const{
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)) > 6.0;
}

FrackMan* StudentWorld::findNearbyFrackMan(Actor *a, int radius) const {
    if (calculateRadius(a->getX(), a->getY(),m_frackMan->getX(), m_frackMan->getY()) <= radius) {
        return m_frackMan;
    }
    return nullptr;
}

// check if 4x4 block is clear
bool StudentWorld::isClear(int x,int xRange, int y, int yRange) const{
    for (int k=0; k <= xRange-1; k++){
        for (int j=0; j <= yRange-1; j++){
            if (m_dirt[x+k][y+j] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

bool StudentWorld::canActorMoveTo(Actor *a, int x, int y) const {
    // if the spot is empty, return true, else false.
    // if the position is invalid return false immediately,
    // TODO copypasta fro mactor
    if (x >= 61 || x< 0) {
        if (y < 0 || y >= 61) {
            return false;
        }
    }
    // iterate game objects loop, check to see if any are at that spot that can't be passed through
    for (std::list<Actor*>::const_iterator curr_actor = m_game_objects.begin(); curr_actor != m_game_objects.end(); ++curr_actor) {
        // if the actor is at that location and cannot be passed through, return false
        // double for loops added to check the 4x4 block
        for (int k = 0; k <= 3; k++) {
            for (int j = 0; j <= 3; j++) {
                if (!(*curr_actor)->canActorsPassThroughMe()) {
                    if ((*curr_actor)->getX() + k == x && (*curr_actor)->getY() + j == y) {
                        return false;
                    }
                } else{
                    break;
                }
            }
        }
    }


    // if actor is protester
    if (a->huntsFrackMan()){
        return isClear(x,4,y,4);
    }

    // if actor is a boulder
    if(!a->canActorsPassThroughMe())
        // check for dirt, only valid for boulderse
        return isClear(x, 4, y, 1);

    return true;
}

// TODO maybe use lineOfSightToFrackman
bool StudentWorld::facingTowardFrackMan(Actor *a) const {
    int x = a->getX();
    int y = a->getY();
    switch (a->getDirection()) {
        case GraphObject::left:
            // if you're facing left, go left
            while(a->isValidPosition(x,y)){
                // if frackman is found, then you are facing it
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep decrementing x to go left
                else {
                    x--;
                }
            }
            break;
        case GraphObject::right:
            // if you're facing right, go right
            while(a->isValidPosition(x,y)){
                // if frackman is found, then you are facing it
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep incrementing x to go right
                else {
                    x++;
                }
            }
            break;
        case GraphObject::down:
            // if you're facing down, go down
            while(a->isValidPosition(x,y)){
                // if frackman is found, then you are facing it
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep decrementing y to go down
                else {
                    y--;
                }
            }
            break;

        case GraphObject::up:
            // if you're facing up, go up
            while(a->isValidPosition(x,y)){
                // if frackman is found, then you are facing it
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep incrementing y to go up
                else {
                    y++;
                }
            }
            break;
    }
}

GraphObject::Direction StudentWorld::lineOfSightToFrackMan(Actor *a) const {
    int actor_x = a->getX();
    int actor_y = a->getY();
    // if the x values are equal there is either a lien of sight up or down
    // TODO fix the fact thatits a 4x4 square or nah
    if (m_frackMan->getX() == actor_x){
        if (m_frackMan->getY() < actor_y){
            return GraphObject::down;
        } else {
            // TODO what if equal?
            return GraphObject::up;
        }
    }
    // if the y values are equal, there is either a line of sight right or left
    else if (m_frackMan->getY() == actor_y){
        if(m_frackMan->getX() < actor_x){
            return GraphObject::left;
        } else {
            // TODO same
            return GraphObject::right;
        }
    } else {
        return GraphObject::none;
    }
}

bool StudentWorld::isClearPathForwardToFrackman(Actor *a, GraphObject::Direction dir) const {
    int x = a->getX();
    int y = a->getY();
    switch (dir) {
        case GraphObject::left:
            // if you're facing left, go left
            while(a->isValidPosition(x,y)){
                // if that spot has dirt or boulder there is no clear path
                if (m_dirt[x][y] != nullptr || !canActorMoveTo(a,x,y)) {
                    return false;
                }
                    // if frackman is found there is a clear path
                if (m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep decrementing x to go left
                else {
                    x--;
                }
            }
            break;
        case GraphObject::right:
            // if you're facing right, go right
            while(a->isValidPosition(x,y)){
                // if that spot has dirt or boulder there is no clear path
                if (m_dirt[x][y] != nullptr || !canActorMoveTo(a,x,y)) {
                    return false;
                }
                // if frackman is found there is a clear path
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep incrementing x to go right
                else {
                    x++;
                }
            }
            break;
        case GraphObject::down:
            // if you're facing down, go down
            while(a->isValidPosition(x,y)){
                // if that spot has dirt or boulder there is no clear path
                if (m_dirt[x][y] != nullptr || !canActorMoveTo(a,x,y)) {
                    return false;
                }
                // if frackman is found there is a clear path
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep decrementing y to go down
                else {
                    y--;
                }
            }
            break;

        case GraphObject::up:
            // if you're facing up, go up
            while(a->isValidPosition(x,y)){
                // if that spot has dirt or boulder there is no clear path
                if (m_dirt[x][y] != nullptr || !canActorMoveTo(a,x,y)) {
                    return false;
                }
                // if frackman is found there is a clear path
                if(m_frackMan->getX()==x & m_frackMan->getY()==y){
                    return true;
                } // otherwise keep incrementing y to go up
                else {
                    y++;
                }
            }
            break;
    }
}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {
    // for every actor
    for (Actor* actor : m_game_objects) {

        // check every coordinate nearby
        for (Coordinate *coord : findAllCoordinatesWithinRadius(x, y, radius)) {
            if(actor->getX() == coord->getX() && actor->getY() == coord->getY()){
                // if the actor at that coordinate is invisble, make it visible
                if(!actor->isVisible()){
                    actor->setVisible(true);
                }
            }
        }
    }
}

int StudentWorld::annoyAllNearbyActors(Actor *annoyer, int points, int radius) {
    // store the count
    int count = 0;
    // coordinates within radius
    // for every actor
    // only look for protesters to annoy if the annoyer is not a protester
    // because they can't annoy themselves
    if(!annoyer->huntsFrackMan()) {
        for (Actor *actor : m_game_objects) {
            if (isWithinRadius(actor, annoyer, radius)) {
                // if the actor at that coordinate is annoyable, annoy it and increment cuont
                if (actor->annoy(points)) {
                    count++;
                }
            }
        }
    }

    // check the frackman, who is only annoyed by the boulder and protesters
    // so if it is a boulder then check
    if (!annoyer->canActorsPassThroughMe() || annoyer->huntsFrackMan()) {
        if (isWithinRadius(m_frackMan, annoyer, radius)) {
            m_frackMan->annoy(points);
            count++;
        }
    }

    return count;
}

vector<Coordinate*> StudentWorld::findAllCoordinatesWithinRadius(int x, int y, int radius) {
    std::vector<Coordinate*> withinRadius;
    for (int k=0; k <= radius-1; k++){
        for (int j=0; j <= radius-1; j++){
            withinRadius.push_back(new Coordinate(x+k,y+j));
        }
    }
    return withinRadius;
}


// TODO delete since this was made to test annoy
void StudentWorld::killFrackman() {
    m_frackMan->annoy(999);
}

bool StudentWorld::isWithinRadius(Actor *a1, Actor *a2, int radius) const {
    return (calculateRadius(a1->getX(),a1->getY(),a2->getX(),a2->getY()) <= radius);
}


void StudentWorld::populateDistanceMap() {
    queue<Coordinate> coordQueue;
    // boolean array to keep track of whichc locations can be moved to
    bool m_validLocations[64][64];
    coordQueue.push(Coordinate(60,60)); // put the starting poitn (AKA the protester end point)
    // mark starting location as encountered
    m_validLocations[60][60] = false;
    for (int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            // if its nto nullptr there is no dirt
            if (m_dirt[i][j] != nullptr){
                m_validLocations[i][j] = true;
            } else {
                m_validLocations[i][j] = false;
            }
        }
    }

    // all the boulders are also unpassable
    // find all of them and update validLocations
    for (Actor* actor : m_game_objects){
        if(!actor->canActorsPassThroughMe()){
            m_validLocations[actor->getX()][actor->getY()] = false;
        }
    }

    // begin exploring

    while(!coordQueue.empty()){
        Coordinate curr = coordQueue.front(); // get current location being explored
        // add this location to the map and store its distance from the end (which is the start 60 60)
        distanceMap.insert(std::pair<Coordinate, float>(curr,calculateRadius(curr.getX(),curr.getY(),60,60)));
        coordQueue.pop();

        // check all the directions
        // first check LEFT
        // if we can move left and haven't been to taht cell yet
        if (m_validLocations[curr.getX()-1][curr.getY()]){
            coordQueue.push(Coordinate(curr.getX()-1, curr.getY()));
            // mark as explored
            m_validLocations[curr.getX()-1][curr.getY()] = false;
        }
        // if we can move RIGHT and havent' been to that cell yet, check there
        if (m_validLocations[curr.getX() +1][curr.getY()]){
            coordQueue.push(Coordinate(curr.getX()+1,curr.getY()));
            // mark as explored
            m_validLocations[curr.getX()+1][curr.getY()] = false;
        }
        // if we can move DOWN and havent' been to that cell yet, check there
        if (m_validLocations[curr.getX()][curr.getY()-1]){
            coordQueue.push(Coordinate(curr.getX(),curr.getY()-1));
            // mark as explored
            m_validLocations[curr.getX()][curr.getY()-1] = false;
        }
        // if we can move UP and havent' been to that cell yet, check there
        if (m_validLocations[curr.getX()][curr.getY()+1]){
            coordQueue.push(Coordinate(curr.getX(),curr.getY()+1));
            // mark as explored
            m_validLocations[curr.getX()][curr.getY()+1] = false;
        }
    }

}
