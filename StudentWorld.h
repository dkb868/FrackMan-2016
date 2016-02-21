#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
using namespace std;

class Actor;
class FrackMan;
class Dirt;

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

	bool isValidRadius(int x1, int y1, int x2, int y2);

	// insert a new game objecgt at a valid location
	void insertGameObject(int count, int xLower,int yLower,int xUpper,int yUpper);

	void addActor(Actor* actor);

	// If the FrackMan is within radius of a, return a pointer to the
	// FrackMan, otherwise null.
	Actor* findNearbyFrackMan(Actor* a, int radius) const;


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
