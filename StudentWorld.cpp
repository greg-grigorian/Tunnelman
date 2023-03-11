// Gregory Grigorian
// 205340574
#include "StudentWorld.h"
#include "Actor.h" 
#include <string>
using namespace std;

StudentWorld::StudentWorld(std::string assetDir)
	        : GameWorld(assetDir)
{
	player = nullptr;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init() {
	player = new TunnelMan(this);
	ticks = 0;
	for (int i = 0; i < 64; i++) { // makes the grid 64 wide and 60 tall
		for (int j = 0; j < 60; j++) {
			if ((i < 30 || i > 33) || (j < 4)) { // does not place Earth objects in the tunnel that stretches down the middle
				earth_grid[i][j] = new Earth(i, j, this); // make new Earth
				gridTracker[i][j] = "Earth"; // track the creation of the earth
			}
			else {
				gridTracker[i][j] = "Empty";
			}
		}
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 60; j < 64; j++) {
			gridTracker[i][j] = "Empty";
		}
	}
	int B = min(int(getLevel() / 2 + 2), 9);
	for (int i = 0; i < B; i++) { // create boulders
		int xPos, yPos;
		boulderCoordinate(xPos, yPos);
		for (int a = xPos; a < xPos + 4; a++) {
			for (int b = yPos; b < yPos + 4; b++) {
					delete earth_grid[a][b];
					earth_grid[a][b] = nullptr;
					gridTracker[a][b] = "Boulder";
			}
		}
		Boulders* b = new Boulders(xPos, yPos, this);
		GameObjects.push_back(b);
	}
	int G = max(int(5 - getLevel() / 2), 2);
	for (int i = 0; i < G; i++) {
		int xPos, yPos;
		objectCoordinate(xPos, yPos);
		GoldNuggets* g = new GoldNuggets(xPos, yPos, this, 100, "Tunnelman", "Permanent");
		GameObjects.push_back(g);
	}
	barrelsLeft = min(int(2 + getLevel()), 21);
	for (int i = 0; i < barrelsLeft; i++) {
		int xPos, yPos;
		objectCoordinate(xPos, yPos);
		BarrelsOfOil* bo = new BarrelsOfOil(xPos, yPos, this, 0);
		GameObjects.push_back(bo);
	}
	numProtestors = 0;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	setTextDisplay();
	ticks++;
	player->doSomething(); // have the player do something every tick

	unsigned int i = 0;
	while (i < GameObjects.size()) {
		if ((GameObjects[i])->getAlive()) {
			(GameObjects[i])->doSomething();
			if (barrelsLeft == 0) {
				playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
			if (!player->getAlive()) {
				playSound(SOUND_PLAYER_GIVE_UP);
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
		}
		i++;
	}
	if (ticks == 1) { 
		int random = rand() % min(90, int(getLevel() * 10 + 30));
		if (random == 0) {
			HardcoreProtestor* h = new HardcoreProtestor(this);
			GameObjects.push_back(h);
			numProtestors++;
			ticksSinceLastProtestor = ticks;
		}
		else {
			RegularProtestor* r = new RegularProtestor(this);
			GameObjects.push_back(r);
			numProtestors++;
			ticksSinceLastProtestor = ticks;
		}
	}
	if (ticks - ticksSinceLastProtestor == max(25, int(200 - getLevel()))) { 
		if (numProtestors < min(15, int(2 + getLevel() * 1.5))) {
			int rando = rand() % min(90, int(getLevel() * 10 + 30));
			if (rando == 0) {
				HardcoreProtestor* h = new HardcoreProtestor(this);
				GameObjects.push_back(h);
				numProtestors++;
				ticksSinceLastProtestor = ticks;
			}
			else {
				RegularProtestor* rp = new RegularProtestor(this);
				GameObjects.push_back(rp);
				numProtestors++;
				ticksSinceLastProtestor = ticks;
			}
		}
	}
	if ((rand() % (getLevel() * 25 + 300)) == 0) {
		if (rand() % 5 == 0) {
			SonarKits* s = new SonarKits(0, 60, this);
			GameObjects.push_back(s);
		}
		else {
			int x, y;
			findSpotForWater(x, y);
			WaterRefills* w = new WaterRefills(x, y, this);
			GameObjects.push_back(w);
		}
	}

	removeDeadActors();

	if (barrelsLeft == 0) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	if (!player->getAlive()) {
		playSound(SOUND_PLAYER_GIVE_UP);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if ((i < 30 || i > 33) || (j < 4)) {
				delete earth_grid[i][j]; // goes through the entire grid and deletes all Earth
			}
		}
	}
	vector<GameObject*>::iterator i = GameObjects.begin();
	while (i != GameObjects.end())
	{
		delete* i;
		i = GameObjects.erase(i);
	}
	delete player; // delete TunnelMan
}

bool StudentWorld::dig(int xPos, int yPos) {
	bool dig = false;
	for (int x = xPos; x <= xPos + 3; x++) { // looks in a 4x4 area around the tunnelman for earth
		for (int y = yPos; y <= yPos + 3; y++) {
			if (gridTracker[x][y] == "Earth") { // if there is earth that needs to be dug
				earth_grid[x][y]->setVisible(false); // make the earth in this spot invisible	
				gridTracker[x][y] = "Empty"; // update the tracker
				dig = true;
			}
		}
	}
	return dig;
}

std::string StudentWorld::getGrid(int xPos, int yPos)
{
	return gridTracker[xPos][yPos];
}

int StudentWorld::getTicks()
{
	return ticks;
}

void StudentWorld::removeDeadActors()
{
	vector<GameObject*>::iterator i = GameObjects.begin();
	while (i != GameObjects.end()) {
		if (!(*i)->getAlive()) {
			delete* i;
			i = GameObjects.erase(i);
		}
		else {
			i++;
		}
	}	
}

TunnelMan* StudentWorld::getPlayer()
{
	return player;
}

void StudentWorld::foundBarrel()
{
	barrelsLeft--;
}

void StudentWorld::addGoldNugget()
{
	GameObjects.push_back(new GoldNuggets(player->getX(), player->getY(), this, 100, "Protestor", "Temporary"));
	player->subtractGoldNuggets();
}

void StudentWorld::addSquirt(SquirtsOfWater* w)
{
	GameObjects.push_back(w);
}

void StudentWorld::boulderCoordinate(int& xPos, int& yPos)
{
	bool foundSpot = false;
	while (foundSpot == false) {
		do {
			xPos = rand() % (60 + 1);
			yPos = rand() % ((56 - 20) + 1) + 20;
		} while ((xPos > 26 && xPos <= 33));
		if (GameObjects.empty()) {
			foundSpot = true;
		}
		for (unsigned int i = 0; i < GameObjects.size(); i++) {
			if (sqrt(pow((GameObjects[i]->getX() - xPos), 2) + pow((GameObjects[i]->getY() - yPos), 2)) <= 6) {
				foundSpot = false;
				break;
			}
			foundSpot = true;
		}
	}
	return;
}

void StudentWorld::objectCoordinate(int& xPos, int& yPos)
{
	bool foundSpot = false;
	while (foundSpot == false) {
		do {
			xPos = rand() % (60 + 1);
			yPos = rand() % (56 + 1);
		} while ((xPos >= 30 && xPos <= 33 && yPos >= 4));
		if (GameObjects.empty()) {
			foundSpot = true;
		}
		for (unsigned int i = 0; i < GameObjects.size(); i++) {
			if (sqrt(pow((GameObjects[i]->getX() - xPos), 2) + pow((GameObjects[i]->getY() - yPos), 2)) <= 6) {
				foundSpot = false;
				break;
			}
			foundSpot = true;
		}
	}
	return;
}

bool StudentWorld::actorNearBoulder(int xPos, int yPos)
{
	for (unsigned int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i]->myGetID() == TID_BOULDER) { 
			if (sqrt(pow((GameObjects[i]->getX() - xPos), 2) + pow((GameObjects[i]->getY() - yPos), 2)) <= 3) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::checkValidSpotToMove(int xPos, int yPos)
{
	for (int x = xPos; x <= xPos + 3; x++) {
		for (int y = yPos; y <= yPos + 3; y++) {
			if (gridTracker[x][y] != "Empty") {
				return false;
			}
		}
	}
	return true;
}

void StudentWorld::fallingBoulderNearProtestor()
{
	for (unsigned int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i]->myGetID() == TID_PROTESTER || GameObjects[i]->myGetID() == TID_HARD_CORE_PROTESTER) {
			if (actorNearBoulder(GameObjects[i]->getX(), GameObjects[i]->getY())) {
				GameObjects[i]->annoy(100);
				increaseScore(500);
			}
		}
	}
}

void StudentWorld::squirtNearProtestor()
{
	for (unsigned int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i]->myGetID() == TID_PROTESTER) {
			for (unsigned int j = 0; j < GameObjects.size(); j++) {
				if (GameObjects[j]->myGetID() == TID_WATER_SPURT) {
					if (sqrt(pow((GameObjects[i]->getX() - GameObjects[j]->getX()), 2) + pow((GameObjects[i]->getY() - GameObjects[j]->getY()), 2)) <= 3) {
						GameObjects[i]->annoy(2);
						GameObjects[j]->dead();
					}
				}
			}
		}
		if (GameObjects[i]->myGetID() == TID_HARD_CORE_PROTESTER) {
			for (unsigned int j = 0; j < GameObjects.size(); j++) {
				if (GameObjects[j]->myGetID() == TID_WATER_SPURT) {
					if (sqrt(pow((GameObjects[i]->getX() - GameObjects[j]->getX()), 2) + pow((GameObjects[i]->getY() - GameObjects[j]->getY()), 2)) <= 3) {
						GameObjects[i]->annoy(2);
						GameObjects[j]->dead();
					}
				}
			}
		}
	}
}

bool StudentWorld::isGoldNearProtestor(int xPos, int yPos)
{
	for (unsigned int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i]->myGetID() == TID_PROTESTER) {
			if (sqrt(pow((GameObjects[i]->getX() - xPos), 2) + pow((GameObjects[i]->getY() - yPos), 2)) <= 3) {
				GameObjects[i]->annoy(21);
			    increaseScore(25);
				return true;
			}
		}
		if (GameObjects[i]->myGetID() == TID_HARD_CORE_PROTESTER) {
			if (sqrt(pow((GameObjects[i]->getX() - xPos), 2) + pow((GameObjects[i]->getY() - yPos), 2)) <= 3) {
				GameObjects[i]->annoy(21);
				increaseScore(50);
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::setTextDisplay()
{
	int level = getLevel();
	int lives = getLives();
	int health = player->getPlayerHitPoints() * 10;
	int squirts = player->getPlayerWater();
	int gold = player->getPlayerGold();
	int barrels = barrelsLeft;
	int sonar = player->getPlayerSonar();
	int score = getScore();

	ostringstream oss;
	oss.setf(ios::fixed);	oss << "Lvl: " << setw(2) << setfill(' ') << level << " Lives: " << setw(1) << lives << " Hlth: " << setw(3) << setfill(' ')  << health << "% Wtr: " 
		<< setw(2) << setfill(' ') << squirts
		<< " Gld: " << setw(2) << setfill(' ')  << gold << " Oil Left: " << setw(2) << setfill(' ') << barrels << " Sonar: " << setw(2) << setfill(' ') << sonar << " Scr: " << setw(6) << setfill('0')
		<< score;
	string displayText = oss.str();
	setGameStatText(displayText);
}

void StudentWorld::useSonarCharge(int xPos, int yPos)
{
	for (unsigned int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects[i]->myGetID() == TID_GOLD || GameObjects[i]->myGetID() == TID_BARREL) {
			if (sqrt(pow((GameObjects[i]->getX() - xPos), 2) + pow((GameObjects[i]->getY() - yPos), 2)) < 12) {
				GameObjects[i]->setVisible(true);
			}
		}
	}
}

void StudentWorld::findSpotForWater(int& xPos, int& yPos)
{
	do {
		xPos = rand() % (60 + 1);
		yPos = rand() % (60 + 1);
	} while (checkValidSpotToMove(xPos, yPos) == false);
}

void StudentWorld::changeNumProtestors()
{
	numProtestors--;
}

/*
void StudentWorld::findShortestPath(int xPos, int yPos)
{
	int x = xPos;
	int y = yPos;
	bool visited[VIEW_WIDTH][VIEW_HEIGHT];
	queue<pair<int, int>> q;
	vector<pair<int, int>> path;
	visited[x][y] = true;
	q.push(make_pair(x,y));
	while (!q.empty()) {
		pair<int, int> temp = q.front();
		x = temp.first;
		y = temp.second;
		if (x == 60 && y == 60) {

		}
	}
}
*/


