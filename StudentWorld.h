#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <list>
#include <vector>

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

    bool isClearPathForwardToFrackman(Actor *a, GraphObject::Direction dir) const;

    std::vector<Coordinate> findAllCoordinatesWithinRadius(int x,int y,int radius) const;

    // Determine the direction of the first move a quitting protester
    // makes to leave the oil field.
    GraphObject::Direction determineFirstMoveToExit(int x, int y);

    // Determine the direction of the first move a hardcore protester
    // makes to approach the FrackMan.
    GraphObject::Direction determineFirstMoveToFrackMan(int x, int y);

    // update a spcific point on the distancemap
    void updateDistanceMap(int x, int y);

    bool isWithinGrid(int x, int y);


    // populate the distanceMap, mapping each Coordinate to the amount of steps to the endpoint
    // from that coordinate
    void populateDistanceMap();
	void populateDistanceMapNEW();
	bool isWithinGridNEW(int x, int y);

private:
	// variable used to keep track of all Actors except Dirt
	std::list<Actor*> m_game_objects;
	// This StudentWorld's FrackMan
	FrackMan* m_frackMan;
	// Array to store all the World's Dirt
	Dirt* m_dirt[64][64];
	// store the boulder count for this level
	int m_boulderCount;
	int m_barrelCount;
	int m_nuggetCount;
    // structure to make navigation very efficient.
    // stores the distance to a certain location from every coordinate
    int m_distanceMap[64][64];
	int m_distanceMapNEW[16][16];


	// get the neighbours of a specific point
    std::vector<Coordinate> getNeighbours(int x, int y);
	std::vector<Coordinate> getNeighboursNew(int x, int y);

	// caculatel the distance for one point on the map
    int calculateDistanceForOnePoint(int x, int y);



};

// TODO check around frackman instaed of checking every actor



#endif // STUDENTWORLD_H_
