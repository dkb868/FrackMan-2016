#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <list>
#include <vector>
using namespace std;

class Actor;
class FrackMan;
class Dirt;
class Coordinate;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual int init();


	virtual int move();

	virtual void cleanUp();

	~StudentWorld();

	bool deleteDirt(int x, int y);

	float calculateRadius(int x1, int y1, int x2, int y2) const;

	bool isValidRadius(int x1, int y1, int x2, int y2) const;

    // are two actors within some radius
    bool isWithinRadius(Actor* a1, Actor* a2, int radius) const;

    // Check if Actor can move to x,y
    bool canActorMoveTo(Actor* a, int x, int y) const;

	// insert a new game objecgt at a valid location
	template<class T> void insertGameObject(int count, int xLower,int yLower,int xUpper,int yUpper);

	void addActor(Actor* actor);

	// If the FrackMan is within radius of a, return a pointer to the
	// FrackMan, otherwise null.
	FrackMan* findNearbyFrackMan(Actor* a, int radius) const;

	// return true if a xRange x yRange square is clear
	bool isClear(int x,int xRange, int y, int yRange) const;

    // Is the Actor a facing toward the FrackMan?
    bool facingTowardFrackMan(Actor* a) const;

    // If the Actor a has a clear line of sight to the FrackMan, return
    // the direction to the FrackMan, otherwise GraphObject::none.
    GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;

    // Reveal all objects within radius of x,y.
    void revealAllNearbyObjects(int x, int y, int radius);

    // Annoy all other actors within radius of annoyer, returning the
    // number of actors annoyed.
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);

    void killFrackman();

    bool clearPathForwardToFrackman(Actor* a, GraphObject::Direction dir) const;

    vector<Coordinate*> findAllCoordinatesWithinRadius(int x,int y,int radius);
private:
	// variable used to keep track of all Actors except Dirt
	list<Actor*> m_game_objects;
	// This StudentWorld's FrackMan
	FrackMan* m_frackMan;
	// Array to store all the World's Dirt
	Dirt* m_dirt[64][64];
	// store the boulder count for this level
	int m_boulderCount;
	int m_barrelCount;
	int m_nuggetCount;
};

#endif // STUDENTWORLD_H_
