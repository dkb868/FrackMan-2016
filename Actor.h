#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

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


// Class for all Actors
class Actor : public GraphObject{
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);
    ~Actor();
    virtual void doSomething() = 0;
    StudentWorld* getWorld();

private:
    StudentWorld* m_world;
};

class Dirt : public Actor{
public:
    Dirt(int x, int y, StudentWorld* world);
    ~Dirt();
    void doSomething();
private:
};

class FrackMan : public Actor {
public:
    FrackMan(StudentWorld* world);
    ~FrackMan();
    void doSomething();

private:
    // make sure the actor is going to a valid palce
    bool isValidPosition(int x, int y);
};


#endif // ACTOR_H_
