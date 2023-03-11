// Gregory Grigorian
// 205340574
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class GameObject : public GraphObject {
public:
	GameObject(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world); 
	virtual ~GameObject();
	virtual void doSomething() = 0;
	StudentWorld* getWorld();
	bool getAlive();
	void dead();
	int myGetID();
	virtual void annoy(int a) = 0;
private:
	StudentWorld* student_world; // pointer to the StudentWorld, will allow access for the two classes to communicate
	bool actor_alive;
	int ID;
};

class Earth : public GameObject { 
public:
	Earth(int startX, int startY, StudentWorld* world); // startX, startY, and world are the only parameters that will be passed in every time
	virtual ~Earth();
	virtual void doSomething();
	virtual void annoy(int a);
};

class TunnelMan : public GameObject {
public:
	TunnelMan(StudentWorld* world);
	virtual ~TunnelMan();
	virtual void doSomething();
	virtual void annoy(int a);
	void addGoldNuggets();
	void addSonarCharge();
	void addWaterRefill();
	void subtractGoldNuggets();
	void setPlayerHitPoints(int hp);
	int getPlayerHitPoints();
	int getPlayerWater();
	int getPlayerGold();
	int getPlayerSonar();
private: 
	int tman_hitPoints; // private member variables to keep track of the TunnelMan's characteristics
	int tman_water;
	int tman_sonarCharge;
	int tman_goldNuggets;
};

class Protestor : public GameObject {
public:
	Protestor(int imageID, StudentWorld* world, int hitPoints, int rest);
	virtual ~Protestor();
	virtual void doSomething() = 0;
	virtual void annoy(int a) = 0;
	int getNumSquaresToMoveInCurrentDirection();
	void setNumSquaresToMoveInCurrentDirection(int c);
	int getRestingTickCount();
	int getTicksToWaitBetweenMoves();
	void setRestingTickCount(int count);
	bool getState();
	void setState(bool s);
	int getNonRestingTickCount();
	void setNonRestingTickCount(int count);
	int getHp();
	void subtractHp(int hp);
	virtual bool facingTunnelman() = 0;
	virtual std::string inSightOfTunnelMan() = 0;
	virtual std::string chooseRandDir() = 0;
private:
	int p_hitpoints;
	bool leaveOilField;
	int ticksToWaitBetweenMoves;
	int restingTickCount;
	int nonRestingTickCount;
	int numSquaresToMoveInCurrentDirection;
};

class RegularProtestor : public Protestor {
public:
	RegularProtestor(StudentWorld* world);
	RegularProtestor(int imageID, StudentWorld* world, int hp);
	virtual ~RegularProtestor();
	virtual void doSomething();
	virtual bool facingTunnelman();
	virtual std::string inSightOfTunnelMan();
	virtual std::string chooseRandDir();
	virtual void annoy(int a);
	void setAnnoyed(bool a);
	std::string checkPerpendicular();
	int getCanYell();
	int getAnnoyedC();
	bool getAnnoyed();
	int getPerpendicularTurn();
	void setAnnoyedC(int c);
	void setPerpendicularTurn(int p);
	void setCanYell(int y);
private:
	int canYell;
	bool amAnnoyed;
	int annoyedC;
	int perpendicularTurn;
};

class HardcoreProtestor : public RegularProtestor {
public:
	HardcoreProtestor(StudentWorld* world);
	virtual ~HardcoreProtestor();
	virtual void annoy(int a);
};

class Goodies : public GameObject {
public:
	Goodies(int imageID, int startX, int startY, StudentWorld* world, int ticks);
	virtual ~Goodies();
	virtual void doSomething() = 0;
	int getTickLife();
	void updateTickLife();
	virtual void annoy(int a) = 0;
private:
	int tickLife;
};

class SquirtsOfWater : public GameObject {
public:
	SquirtsOfWater(int startX, int startY, Direction d, StudentWorld* world);
	virtual ~SquirtsOfWater();
	virtual void doSomething();
	virtual void annoy(int a);
private:
	int travelDistance;
};

class BarrelsOfOil : public Goodies {
public:
	BarrelsOfOil(int startX, int startY, StudentWorld* world, int ticks);
	virtual ~BarrelsOfOil();
	virtual void doSomething();
	virtual void annoy(int a);
	void setMyVisibleTrue();
private:
	bool myVisible;
};

class Boulders : public GameObject {
public:
	Boulders(int startX, int startY, StudentWorld * world);
	virtual ~Boulders();
	virtual void doSomething();
	bool checkUnder(std::string type);
	virtual void annoy(int a);
private:
	std::string boulderState;
	int tickLife;
};

class GoldNuggets : public Goodies {
public:
	GoldNuggets(int startX, int startY, StudentWorld* world, int ticks, std::string pickUp, std::string state);
	virtual ~GoldNuggets();
	virtual void doSomething();
	virtual void annoy(int a);
	void setMyGoldVisible();
private:
	std::string whoCanPickUp;
	std::string nuggetState;
	bool myGoldVisible;
};

class SonarKits : public Goodies {
public:
	SonarKits(int startX, int startY, StudentWorld* world);
	virtual ~SonarKits();
	virtual void doSomething();
	virtual void annoy(int a);
};

class WaterRefills : public Goodies {
public:
	WaterRefills(int startX, int startY, StudentWorld* world);
	virtual ~WaterRefills();
	virtual void doSomething();
	virtual void annoy(int a);
};

#endif // ACTOR_H_
