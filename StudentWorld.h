#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
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

private:
	vector<Actor*> m_game_objects;
	FrackMan* m_frackMan;
	Dirt* m_dirt[64][59];
};

#endif // STUDENTWORLD_H_
