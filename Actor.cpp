#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"



// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
// Intialzie

// ACTOR METHODS

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world, int hitpoints=1) :
        GraphObject(imageID, startX, startY, dir, size, depth), m_hitpoints(hitpoints) {
    m_world = world;
    alive = true;
}

Actor::~Actor(){

}


bool Actor::isAlive() const {
    // 0 hitpoints = false, anythign else is true
    return m_hitpoints;
}

void Actor::setDead() {
    m_hitpoints = 0;
}


void Actor::reduceHitpoints(int hp) {
    m_hitpoints -= hp;
}

bool Actor::moveForward() {
    switch (getDirection()) {
        case GraphObject::left:
            // if you're facing left, go left
            if (isValidPosition(getX() - 1, getY())) {
                moveTo(getX() - 1, getY());
                return true;
            } else {
                moveTo(getX(), getY());
                return false;
            }
            break;
        case GraphObject::right:
            // if you're facing right, go right
            if (isValidPosition(getX() + 1, getY())) {
                moveTo(getX() + 1 , getY());
                return true;
            } else {
                moveTo(getX(), getY());
                return false;
            }
            break;
        case GraphObject::down:
            if (isValidPosition(getX(), getY() - 1)) {
                moveTo(getX(), getY() - 1);
                return true;
            } else {
                moveTo(getX(), getY());
                return false;
            }
            break;
        case GraphObject::up:
            if (isValidPosition(getX(), getY() + 1)) {
                moveTo(getX(), getY() + 1);
                return true;
            } else {
                moveTo(getX(), getY());
                return false;
            }
            break;
    }
}

// ---- END ACTOR METHODS


// PROTESTER PEMTHODS

/* constructs a new protester att their statrting location TODO
Protester::Protester(StudentWorld *world) {

} */


StudentWorld* Actor::getWorld() {
    return m_world;
}

// PICKUP METHODS
// Cosntructor for Pickup
// hitpoints default to 1 since some objects have no use for hitpoints
// in other pickups, hitpoitns will be used to count ticks
Pickup::Pickup(int imageID, int startX, int startY, GraphObject::Direction dir, double size, unsigned int depth,
               StudentWorld *world, int hitpoints=1) : Actor(imageID, startX, startY, dir, size, depth, world, hitpoints)
{};

// PERSON METHODS

// Cosntructor for Person
Person::Person(int imageID, int startX, int startY, GraphObject::Direction dir, double size, unsigned int depth,
               StudentWorld *world, int hitpoints) : Actor(imageID, startX, startY, dir, size, depth, world, hitpoints)
{};




// DIRT METHODS
// Create new dirt object with dirt image id, it's own x and y, facing right and depth of 3 and size 2.5
Dirt::Dirt(int x, int y , StudentWorld* world) : Actor(IID_DIRT,x,y,DIRT_DIR, DIRT_SIZE, DIRT_DEPTH, world) {
    setVisible(true);
}

Dirt::~Dirt(){

}
void Dirt::doSomething(){
    return;
}

// BOULDER METHODS


Boulder::Boulder(StudentWorld *world, int x, int y) :
        Actor(IID_BOULDER, x, y, BOULDER_DIR, BOULDER_SIZE, BOULDER_DEPTH, world){
    setVisible(true);
}



void Boulder::doSomething() {
    // do Nothing
}

// OIL BARREL METHODS
OilBarrel::OilBarrel(StudentWorld *world, int x, int y) :
    Pickup(IID_BARREL, x, y, OIL_BARREL_DIR, OIL_BARREL_SIZE, OIL_BARREL_DEPTH, world){
    // TODO or nah
    setVisible(true);
}

// TODO someting with oil causing block of dirt to never be deleted
void OilBarrel::doSomething() {
    // if the barrel isn't alive, return immediately
    if (!isAlive()) return;
    if (getWorld()->findNearbyFrackMan(this, 3)){
        setDead();
        // sound effect TODO
        // add 1000 points TODO
        // decrement requriedo il barrel count TODO
    } else if (getWorld()->findNearbyFrackMan(this,4)){
        setVisible(true); // troll
        return;
    }

}

// --- END OIL BARREL METHODS


// SQUIRT METHODS

Squirt::Squirt(StudentWorld *world, int x, int y, Direction dir) :
    Actor(IID_WATER_SPURT, x, y, dir, SQUIRT_SIZE, SQUIRT_DEPTH, world), start_x(x), start_y(y){
    setVisible(true);
}

void Squirt::doSomething() {
    // move squirt 4 squares ahead of fracklegend
    if (getWorld()->calculateRadius(start_x,start_y,getX(),getY()) < 5) {
        moveForward();
    } else {
        setDead();
    }
}

// ---- END SQUIRT METHODS


// GOLD NUGGET METHODS

void GoldNugget::doSomething() {
    // if the nugget isn't alive, return immediately
    if (!isAlive()) return;
    // if FrackMan is within a radius of 3.0, the nugget activates
    FrackMan* frackMan = getWorld()->findNearbyFrackMan(this, 3);
    if (frackMan){
        setDead();
        // sound effect TODO
        // add 10 points TODO
        //  increment frackman nugggets yum TODO
        frackMan->addGold(1);
    }
    // else if frackman is within a radius of 4, set the nugget to visible
    else if (getWorld()->findNearbyFrackMan(this,4)){
        setVisible(true);
        return;
    }
}

GoldNugget::GoldNugget(StudentWorld *world, int x, int y) :
    Pickup(IID_GOLD, x, y, GOLD_NUGGET_DIR, GOLD_NUGGET_SIZE, GOLD_NUGGET_DEPTH, world){
    // TODO or nah
    setVisible(true);
}

// --- END GOLD NUGGET METHODS


// SONAR KIT METHODS

void SonarKit::doSomething() {
    // if dead, return immediately
    if (!isAlive()) return;
    // if frackman is within radius of 3.0
    FrackMan* frackMan = getWorld()->findNearbyFrackMan(this, 3);
    if (frackMan){
        setDead();
        // sound effect TODO
        // add 75 points TODO
        frackMan->addSonar(1);
    }
    // reduce the amount of ticks this object has remaning tolive
    reduceHitpoints(1);
}

SonarKit::SonarKit(StudentWorld *world, int x, int y)
// TODO change arbitrary lifetime of 10 ticks
    :Pickup(IID_SONAR, x,y,SONAR_KIT_DIR, SONAR_KIT_SIZE, SONAR_KIT_DEPTH, world, 10){
    setVisible(true);
}

//END SONAR KIT METHODS


// WATER POOL METHODS
void WaterPool::doSomething() {
    // if it is not alive, return immediatey
    if (!isAlive()) return;
    // if the frackman is within a radius of 3 of the pool
    FrackMan* frackMan = getWorld()->findNearbyFrackMan(this, 3);
    if (frackMan){
        // the water pool is dead so it may be removed
        setDead();
        // sound effect TODO
        // add 100 points TODO
        // frackman gets 5 new squits of water
        frackMan->addWater(5);
    }
        // else if frackman is within a radius of 4, set the nugget to visible
    else if (getWorld()->findNearbyFrackMan(this,4)){
        setVisible(true);
        return;
    }
    // reduce hitpoints which is essentially the 'tick lifetime'
    reduceHitpoints(1);
}

WaterPool::WaterPool(StudentWorld *world, int x, int y):
// TODO randomly assigned to last 10 ticks so use actual algorithm
    Pickup(IID_WATER_POOL,x,y,WATER_POOL_DIR,WATER_POOL_SIZE,WATER_POOL_DEPTH,world,10){
        setVisible(true);

}



// END WATER POOL METHODS


// FRACKMAN METHODS
FrackMan::FrackMan(StudentWorld *world) :
    Person(IID_PLAYER, PLAYER_START_X, PLAYER_START_Y, PLAYER_DIR, PLAYER_SIZE,PLAYER_DEPTH, world, PLAYER_HITPOINTS){
    setVisible(true);
    addWater(5);
    addSonar(1);
}

FrackMan::~FrackMan() {

}

// make sure that the postition of fackman is valid
// TODO move to 'Person' class
bool Actor::isValidPosition(int x, int y) {
    if (x < 61 && x >= 0) {
        if (y >= 0 && y < 61) {
           return true;
        }
    }
    return false;
}

// TODO improve dryness maybe use array to store coordinates and rewrite isValidPosition
void FrackMan::doSomething() {
    int ch;
    Direction dir = getDirection();
    if (getWorld()->getKey(ch)) {
        switch(ch){
            case KEY_PRESS_LEFT:
                // if you're facing left, go left
                if  (dir == GraphObject::left) {
                    int newLocation = getX() - 1;
                    if (isValidPosition(newLocation, getY())) {
                        moveTo(newLocation, getY());
                    } else {
                        moveTo(getX(),getY());
                    }
                }
                // otherwise start facing left
                else
                    setDirection(GraphObject::left);
                break;
            case KEY_PRESS_RIGHT:
                // if you're facing right, go right
                if (dir == GraphObject::right) {
                    int newLocation = getX() + 1;
                    if (isValidPosition(newLocation, getY())){
                        moveTo(newLocation, getY());
                    } else {
                        moveTo(getX(),getY());
                    }
                }
                else
                    setDirection(GraphObject::right);
                break;
            case KEY_PRESS_DOWN:
                if (dir==GraphObject::down) {
                    int newLocation = getY()-1;
                    if(isValidPosition(getX(), newLocation)){
                        moveTo(getX(), newLocation);
                    } else {
                        moveTo(getX(), getY());
                    }
                }
                else
                    setDirection(GraphObject::down);
                break;
            case KEY_PRESS_UP:
                if (dir == GraphObject::up) {
                    int newLocation = getY() + 1;
                    if(isValidPosition(getX(), newLocation)){
                        moveTo(getX(), newLocation);
                    } else {
                        moveTo(getX(), getY());
                    }
                }
                else
                    setDirection(GraphObject::up);
                break;
            case KEY_PRESS_SPACE:
                if (getWater() > 0) {
                    Squirt *squirt = new Squirt(getWorld(), getX(), getY(), dir);
                    getWorld()->addActor(squirt);
                    m_water--;
                }
                break;
        }
        // TODO only if player moved
        // delete blcoks all around
        for (int i=0; i <= 3; i++){
            for (int j=0; j <= 3; j++){
                getWorld()->deleteDirt(getX()+i,getY()+j);
            }
        }
    }
}


void FrackMan::addSonar(int a=1) {
    m_sonar += a;
}

void FrackMan::addGold(int a=1) {
    m_gold+=a;
}

void FrackMan::addWater(int a=1) {
    m_water+=a;
}


 int FrackMan::getGold() const {
    return m_gold;
}

 int FrackMan::getSonar() const {
    return m_sonar;
}

 int FrackMan::getWater() const {
    return m_water;
}

// ---- END FRACKMAN METHODS