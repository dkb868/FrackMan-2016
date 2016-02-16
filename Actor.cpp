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

FrackMan::FrackMan(int x, int y, GraphObject::Direction dir, StudentWorld *world) :
    Actor(IID_PLAYER, PLAYER_START_X, PLAYER_START_Y, PLAYER_DIR, PLAYER_SIZE,PLAYER_DEPTH, world){
    setVisible(true);
}

FrackMan::~FrackMan() {

}

void FrackMan::doSomething() {
    return;
}
