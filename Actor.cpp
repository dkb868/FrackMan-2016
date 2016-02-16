#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// DIRT CONSTANTS
const GraphObject::Direction DIRT_DIR = GraphObject::right;
const int DIRT_DEPTH = 3;
const double DIRT_SIZE = 0.25;

// PLAYER CONSTANTS

const GraphObject::Direction PLAYER_DIR = GraphObject::right;
const int PLAYER_DEPTH = 0;
const double PLAYER_SIZE = 1.0;
const int PLAYER_START_X = 30;
const int PLAYER_START_Y = 60;


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
// Intialzie
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world) :
        GraphObject(imageID, startX, startY, dir, size, depth) {
    m_world = world;
}

Actor::~Actor(){

}

StudentWorld* Actor::getWorld() {
    return m_world;
}

// Create new dirt object with dirt image id, it's own x and y, facing right and depth of 3 and size 2.5
Dirt::Dirt(int x, int y , StudentWorld* world) : Actor(IID_DIRT,x,y,DIRT_DIR, DIRT_SIZE, DIRT_DEPTH, world) {
    setVisible(true);
}

Dirt::~Dirt(){

}
void Dirt::doSomething(){
    return;
}

FrackMan::FrackMan(StudentWorld *world) :
    Actor(IID_PLAYER, PLAYER_START_X, PLAYER_START_Y, PLAYER_DIR, PLAYER_SIZE,PLAYER_DEPTH, world){
    setVisible(true);
}

FrackMan::~FrackMan() {

}

// make sure that the postition of fackman is valid
// TODO move to Actor class
bool FrackMan::isValidPosition(int x, int y) {
    if (x < 61 && x >= 0) {
        if (y >= 0 && y < 61) {
           return true;
        }
    }
    return false;
}

// TODO MAKE THIS SHIT DRY
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
            case KEY_PRESS_ESCAPE:
                //kill
                break;
        }
        // TODO only if player moved thou
        // delete blcoks all around
        for (int i=0; i <= 3; i++){
            for (int j=0; j <= 3; j++){
                getWorld()->deleteDirt(getX()+i,getY()+j);
            }
        }
    }
}
