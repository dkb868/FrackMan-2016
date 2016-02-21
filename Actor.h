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

// BOULDER COSNTANTS
const GraphObject::Direction BOULDER_DIR = GraphObject::down;
const int BOULDER_DEPTH = 1;
const double BOULDER_SIZE = 1.0;


/*
 * Class for all Actors.
 * Actors extend GraphObject and are basically every object in the game that does something
 */
class Actor : public GraphObject{
public:
    // Basic constructor for an Actor
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);
    // Simple destructor for Actor
    ~Actor();
    /*
     * Every actor should be able to do something and most do something significant.
     * The only exception is Dirt whose sole purpose is to be destroyed but it is still considered an actor
     * since being destroyed is still considered doing something.
     */
    virtual void doSomething() = 0;
    // Return a pointer to the Student World object that this Actor belongs to.
    // useful because methods from the StudentWorld are eneded in some Actor functions.
    StudentWorld* getWorld();

private:
    // store the Student World that the Actor belongs to.
    StudentWorld* m_world;
};

/* Any object that can be 'picked-up' by the FrackMan
 * These objects have to deal with things that are within a certain radius
 */
class Pickup : public Actor{
public:
    // Basic constructor for a Pickup
    Pickup(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);

};

/* Dirt just gets destroyed by the Frackman when he steps on it */
class Dirt : public Pickup{
public:
    Dirt(int x, int y, StudentWorld* world);
    ~Dirt();
    void doSomething();
private:
};

// Boulders fall on people
class Boulder : public Actor {
public:
    Boulder(StudentWorld* world,int x, int y);

    virtual void doSomething();
};

/*
 * All Persons can be annoyed
 */
class Person : public Actor {
public:
    // Basic constructor for a Pickup
    Person(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);

};

/*
 * A FrackMan is controlled by the user
 * Takes in user input then does something every turn
 */
class FrackMan : public Person {
public:
    // Basic constructor
    FrackMan(StudentWorld* world);
    ~FrackMan();
    void doSomething();

private:
    // make sure the actor is going to a valid palce
    bool isValidPosition(int x, int y);
};


/*
 * A Protester is a bot
 * There are hardcore and regular protestors
 * They have almost everything in common except the way they seek the Frackman
 *
class Protester : public Person {
public:
    // Basic constructor
    Protester(StudentWorld* world);
}; */


// A position has an x and y value
// the Euclidean Distance between two values can be calculated
// you can check if two objects are within radius.
class Position {
public:
    Position(int x, int y);
    // gets the euclidean distance between two positions
    static float getDistance(Position pos1, Position pos2);

};

#endif // ACTOR_H_
