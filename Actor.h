#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

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
    FrackMan(int x,int y, Direction dir, StudentWorld* world);
    ~FrackMan();
    void doSomething();

private:
};


#endif // ACTOR_H_
