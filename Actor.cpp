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


bool Actor::canDigThroughDirt() const {
    return false;
}

bool Actor::canPickThingsUp() const {
    return false;
}

bool Actor::needsToBePickedUpToFinishLevel() const {
    return false;
}

bool Actor::huntsFrackMan() const {
    return false;
}

bool Actor::isAlive() const {
    // 0 hitpoints = false, anythign else is true
    return m_hitpoints != 0;
}

int Actor::getHitpoints() {
    return m_hitpoints;
}

void Actor::setDead() {
    m_hitpoints = 0;
}


void Actor::reduceHitpoints(int hp) {
    m_hitpoints -= hp;
}

bool Actor::moveForward() {
    switch (this->getDirection()) {
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

// TODO coordinate class might make this bullshit dry

bool Actor::canMoveForward(Direction dir) {
    switch (dir) {
        case GraphObject::left:
            // if you're facing left, go left
            if (isValidPosition(getX() - 1, getY())) {
                return true;
            } else {
                return false;
            }
            break;
        case GraphObject::right:
            // if you're facing right, go right
            if (isValidPosition(getX() + 1, getY())) {
                return true;
            } else {
                return false;
            }
            break;
        case GraphObject::down:
            if (isValidPosition(getX(), getY() - 1)) {
                return true;
            } else {
                return false;
            }
            break;
        case GraphObject::up:
            if (isValidPosition(getX(), getY() + 1)) {
                return true;
            } else {
                return false;
            }
            break;
    }
}


bool Actor::canActorsPassThroughMe() const {
    return true;
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
    m_state = BOULDER_STATE_STABLE;
    m_waitingTicks = 30;
}

bool Boulder::canActorsPassThroughMe() const {
    return false;
}

void Boulder::doSomething() {
    // if not alive, immediatey return
    if (!isAlive()) return;
    // check the state of the boulder and react appropriately
    switch(m_state){
        case BOULDER_STATE_STABLE:
            // check 4 squares immediately below for dirt
            // if there is no dirt transition boulder to waiting state
            if (getWorld()->isClear(getX(), 4, getY()-1, 1)){
                m_state = BOULDER_STATE_WAITING;
            }
            break;
        case BOULDER_STATE_WAITING:
            m_waitingTicks--;
            // if 30 ticks have elapsed
            if (m_waitingTicks == 0){
                // TODO play sound
                m_state = BOULDER_STATE_FALLING;
            }
            break;
        case BOULDER_STATE_FALLING:
            // move down 1 square
            if (!moveForward()){
                // if it hits the bottom of the field, set it do dead
                setDead();
            } // if it runs into another boulder or dirt
            else if (!(getWorld()->canActorMoveTo(this, getX(),getY()-1))){
                setDead();
            }
            // TODO else if in radius of prtestor, annoy

            break;
    }
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

SonarKit::SonarKit(StudentWorld *world)
// TODO change arbitrary lifetime of 10 ticks
    :Pickup(IID_SONAR, SONAR_KTI_START_X,SONAR_KIT_START_Y,SONAR_KIT_DIR, SONAR_KIT_SIZE, SONAR_KIT_DEPTH, world, 10){
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
        // TODO hacked, fix this bug
        setVisible(false);
        // sound effect TODO
        // add 100 points TODO
        // frackman gets 5 new squits of water
        frackMan->addWater(5);
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

// PROTESTER METHODS


void Protester::doSomething() {

}


Protester::Protester(int imageID, Direction dir, double size, unsigned int depth,
                     StudentWorld *world, int hitpoints) : Person(imageID, PROTESTER_START_X, PROTESTER_START_Y, dir, size, depth, world, hitpoints) {
    m_state = PROTESTER_STATE_REST; // TODO is this the start state?
}

bool Protester::annoy(unsigned int amount) {
    return false;
}

void Protester::addGold() {

}

bool Protester::huntsFrackMan() const {
    return true;
}



int Protester::getState() {
    return m_state;
}

void Protester::setState(int x) {
    m_state = x;
}

void Protester::setTicksToNextMove() {

}

// END PROTESTER METHODS

// REGULAR PROTESTOR METHODS



RegularProtester::RegularProtester( StudentWorld* world) :
    Protester(IID_PROTESTER, REGULAR_PROTESTER_DIR, REGULAR_PROTESTER_SIZE, REGULAR_PROTESTER_DEPTH
    ,world,REGULAR_PROTESTER_HITPOINTS){

    setVisible(true);
    m_ticksToWaitBetweenMoves = 10;
    m_restTicks = m_ticksToWaitBetweenMoves;
    m_numSquaresToMoveInCurrentDirection = 8;
    m_canShout = true;
    // non resting ticks
    m_shoutTicks = 15;
}


void RegularProtester::doSomething() {
    // if not alive, immediateley return
    if (!isAlive()) return;

    switch(getState()){
        case PROTESTER_STATE_REST:
            m_restTicks--;
            // if time runs out
            if (m_restTicks == 0){
                // reset rest ticks
                m_restTicks = m_ticksToWaitBetweenMoves;
                // change state
                setState(PROTESTER_STATE_ACTIVE);
            }
            break;
        case PROTESTER_STATE_LEAVING:
            // if it is at the endpoint, set it to dead
            if(getX() == 60 and getY() == 60) setDead();
            // else move closer to exit
            break;
        case PROTESTER_STATE_ACTIVE:
            if (!m_canShout) {
                m_shoutTicks--;
            }
            // if the ticks run out , then protester can shotu again!
            if (m_shoutTicks == 0){
                m_shoutTicks = 15;
                m_canShout = true;
            }
            // if within distance of 4 units of frackman and facing frackman
            if (getWorld()->findNearbyFrackMan(this,4) && getWorld()->facingTowardFrackMan(this)){
                // if the regular protester hasn't shouted within its last 15 ticks
                // play shout sound
                // annoy frackman
                m_canShout = false;
                return;
            }
            // otherwise if there is a line of sight to the frackman and more than 4 units away from frackman
            Direction frackmanDir = getWorld()->lineOfSightToFrackMan(this);
            if (frackmanDir!=none && !getWorld()->findNearbyFrackMan(this,4)){
                // if it could move the entire way to frackman with nothign in its path
                // TODO A Regular Protester cannot move to a location that is within a radius of 3 (<= 3.0) units of a Boulder.
                if (getWorld()->clearPathForwardToFrackman(this, frackmanDir)) {
                    // face teh direction of frackman
                    setDirection(frackmanDir);
                    // take oen step forward
                    moveForward();
                    // set numSquaresToMoveinCurrentDirection to 0, focing it to pick a new direction/distance to move
                    // during its next non-resting tick
                    m_numSquaresToMoveInCurrentDirection = 0;
                    return;
                }
            }
            // otherwise if regular protester cannot see frackman TODO
            // decrement numSquaresToMoveInCurrentDirection by one
            m_numSquaresToMoveInCurrentDirection--;
            // if the reg protester has finished walking numSquarestoMove in currently-selected direction
            if (m_numSquaresToMoveInCurrentDirection <= 0 ) {
                // if finished walking numsquarestomvoeinCurrentDirection,
                // pick a random direction
                Direction randomDir;
                while (true) {
                    switch (rand() % 4) {
                        case 0:
                            randomDir = up;
                            break;
                        case 1:
                            randomDir = down;
                            break;
                        case 2:
                            randomDir = left;
                            break;
                        case 3:
                            randomDir = right;
                            break;
                    }
                    // if it can move forward in thsi direction, move
                    if (canMoveForward(randomDir)) {
                        break;
                    }
                }
                setDirection(randomDir);
                m_numSquaresToMoveInCurrentDirection = rand()%53 + 8; // TODO make sure legit
                // pick new value for numSquarestoMoveincurrentDirection between 8 and 60
                // go to step 8 TODO
            }

            // otherwise if protester is sitting at intersection where it could turn and move in perp
            // direction eg facing left and can go up or down
            // has not made perpendicular turn in last 200 no resting ticks.
            // detrmine which of two directiosn are viable
            // pick one randomly
            // set direction to new direction
            // pick new numsquarestoMvoeValue
            // goto step 8

            // step 8
            // FINALLY FUCKING make a move one step in its currently facing direction
           if (!moveForward()){
                // if blocked for some reason, set numSquarestoMoveincurrent direction to 0
                // resulting in new direction being chosen during regular protester next non resting tick
                m_numSquaresToMoveInCurrentDirection = 0;
            }
            // change direction to new direction
            setState(PROTESTER_STATE_REST);
            break;
    }
}

void RegularProtester::addGold() {
    Protester::addGold();
}
// END REGULAR PROTESTOR METHODS

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
    bool flag = false;
    if (x < 61 && x >= 0) {
        if (y >= 0 && y < 61) {
           flag = true;
        }
    }

    if (flag) {
        flag = getWorld()->canActorMoveTo(this,x,y);
    }

    return flag;
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
                // shoot water
                if (getWater() > 0) {
                    Squirt *squirt = new Squirt(getWorld(), getX(), getY(), dir);
                    getWorld()->addActor(squirt);
                    m_water--;
                }
                break;
            case KEY_PRESS_ESCAPE:
                break;
            case 'z':
            case 'Z':
                if (getSonar() > 0){
                    // make shit visible TODO
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


// COORDINATE CLASS

Coordinate::Coordinate(int x, int y) : m_x(x), m_y(y) {
}


float Coordinate::getDistance(Coordinate coord) {
    return sqrt(pow(coord.getX()-m_x, 2) + pow(coord.getY()-m_y, 2));
}

int Coordinate::getX() {
    return m_x;
}

int Coordinate::getY() {
    return m_y;
}
