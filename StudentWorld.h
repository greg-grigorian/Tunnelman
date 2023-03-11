// Gregory Grigorian
// 205340574
#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <queue>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class GameObject;
class TunnelMan;
class Earth;
class SquirtsOfWater;
class BarrelsOfOil;
class GoldNuggets;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool dig(int xPos, int yPos); // checks if TunnelMan is in an Earth block
	std::string getGrid(int xPos, int yPos);
	int getTicks();
	void removeDeadActors();
	TunnelMan* getPlayer();
	void foundBarrel();
	void addGoldNugget();
	void addSquirt(SquirtsOfWater* w);
	void boulderCoordinate(int& xPos, int& yPos);
	void objectCoordinate(int& xPos, int& yPos);
	bool actorNearBoulder(int xPos, int yPos);
	bool checkValidSpotToMove(int xPos, int yPos);
	void fallingBoulderNearProtestor();
	void squirtNearProtestor();
	bool isGoldNearProtestor(int xPos, int yPos);
	void setTextDisplay();
	void useSonarCharge(int xPos, int yPos);
	void findSpotForWater(int& xPos, int& yPos);
	void changeNumProtestors();
	//void findShortestPath(int xPos, int yPos);
private:
	TunnelMan* player; // pointer to TunnelMan
	Earth* earth_grid[VIEW_WIDTH][VIEW_HEIGHT]; // 2D array to hold Earth objects
	std::string gridTracker[VIEW_WIDTH][VIEW_HEIGHT]; // keeps track of the items in the entire oil field
	int ticks;
	std::vector<GameObject*> GameObjects;
	int barrelsLeft;
	int numProtestors;
	int ticksSinceLastProtestor;
};

#endif // STUDENTWORLD_H_
