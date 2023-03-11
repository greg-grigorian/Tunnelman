// Gregory Grigorian
// 205340574
#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

GameObject::GameObject(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world)
	      : GraphObject(imageID,startX,startY,dir,size,depth) {
	setVisible(true); // updates all the game objects by default to be seen
	student_world = world;
	actor_alive = true;
	ID = imageID;
}

GameObject::~GameObject() {
}

StudentWorld* GameObject::getWorld() {
	return student_world; // returns the StudentWorld
}

Earth::Earth(int startX, int startY, StudentWorld* world)
	 : GameObject(TID_EARTH, startX, startY, right, 0.25, 3, world) { // initialize every Earth object with correct starting parameters
}

Earth::~Earth() {
}

void Earth::doSomething() {
}

void Earth::annoy(int a)
{
}

TunnelMan::TunnelMan(StudentWorld* world)
	     : GameObject(TID_PLAYER, 30, 60, right, 1.0, 0, world) { // initialize every TunnelMan object with correct starting parameters
	tman_hitPoints = 10;
	tman_water = 5;
	tman_sonarCharge = 1;
	tman_goldNuggets = 0;
}

TunnelMan::~TunnelMan() {
}

void TunnelMan::doSomething() {
	if (tman_hitPoints <= 0) {
		dead();
	}
	if (getAlive() == false) { // check if player is alive
		return;
	}
	if (getWorld()->dig(getX(),getY())) { // checks if the TunnelMan should dig any Earth and if so, the sound for digging should be played
		getWorld()->playSound(SOUND_DIG);
	}
	int ch;
	if (getWorld()->getKey(ch) == true) { // receive input from keyboard
		switch (ch) {
		case KEY_PRESS_LEFT:
			if (getDirection() != left) { // the following is the same for each direction: check if the player is facing the direction pressed
				setDirection(left);       // if not, update the direction the player is facing
			}                             // otherwise, if the player will still be in bounds, move the player one spot in the indicated direction
			else {
				if (getX() > 0 && (getWorld()->actorNearBoulder(getX() - 1, getY()) == false)) {
					moveTo(getX() - 1, getY());
				}
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right) {
				setDirection(right);
			}
			else {
				if (getX() < 60 && (getWorld()->actorNearBoulder(getX() + 1, getY()) == false)) {
					moveTo(getX() + 1, getY());
				}
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up) {
				setDirection(up);
			}
			else {
				if (getY() < 60 && (getWorld()->actorNearBoulder(getX(), getY() + 1) == false)) {
					moveTo(getX(), getY() + 1);
				}
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down) {
				setDirection(down);
			}
			else {
				if (getY() > 0 && (getWorld()->actorNearBoulder(getX(), getY() - 1) == false)) {
					moveTo(getX(), getY() - 1);
				}
			}
			break;
		case KEY_PRESS_ESCAPE: 
			dead();
			break;
		case KEY_PRESS_TAB:
			if (tman_goldNuggets >= 1) {
				getWorld()->addGoldNugget();
			}
			break;
		case KEY_PRESS_SPACE:
			if (tman_water >= 1) {
				SquirtsOfWater* water;
				switch (getDirection()) {
				case up:
					if (getWorld()->checkValidSpotToMove(getX(), getY() + 4) == true && (getWorld()->actorNearBoulder(getX(), getY() + 4) == false)) { 
						water = new SquirtsOfWater(getX(), getY() + 4, up, getWorld());
						getWorld()->addSquirt(water);
					}
					break;
				case down:
					if (getWorld()->checkValidSpotToMove(getX(), getY() - 4) == true && (getWorld()->actorNearBoulder(getX(), getY() - 4) == false)) { 
						water = new SquirtsOfWater(getX(), getY() - 4, down, getWorld());
						getWorld()->addSquirt(water);
					}
					break;
				case left:
					if (getWorld()->checkValidSpotToMove(getX() - 4, getY()) == true && (getWorld()->actorNearBoulder(getX() - 4, getY()) == false)) { 
						water = new SquirtsOfWater(getX() - 4, getY(), left, getWorld());
						getWorld()->addSquirt(water);
					}
					break;
				case right:
					if (getWorld()->checkValidSpotToMove(getX() + 4, getY()) == true && (getWorld()->actorNearBoulder(getX() + 4, getY()) == false)) { 
						water = new SquirtsOfWater(getX() + 4, getY(), right, getWorld());
						getWorld()->addSquirt(water);
					}
					break;
				}
				tman_water--;
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				break;
			}
			break;
		case 'z':
			if (tman_sonarCharge >= 1) {
				tman_sonarCharge--; // add revealing functionality
				getWorld()->useSonarCharge(getX(), getY());
			}
			break;
		case 'Z':
			if (tman_sonarCharge >= 1) {
				tman_sonarCharge--; // add revealing functionality
				getWorld()->useSonarCharge(getX(), getY());
			}
			break;
		}
	}
}

void TunnelMan::annoy(int a)
{
}

void TunnelMan::addGoldNuggets()
{
	tman_goldNuggets++;
}

void TunnelMan::addSonarCharge()
{
	tman_sonarCharge++;
}

void TunnelMan::addWaterRefill()
{
	tman_water = tman_water + 5;
}

void TunnelMan::subtractGoldNuggets()
{
	tman_goldNuggets--;
}

void TunnelMan::setPlayerHitPoints(int hp)
{
	tman_hitPoints = tman_hitPoints - hp;
}

int TunnelMan::getPlayerHitPoints()
{
	return tman_hitPoints;
}

int TunnelMan::getPlayerWater()
{
	return tman_water;
}

int TunnelMan::getPlayerGold()
{
	return tman_goldNuggets;
}

int TunnelMan::getPlayerSonar()
{
	return tman_sonarCharge;
}

bool GameObject::getAlive()
{
	return actor_alive;
}

void GameObject::dead()
{
	actor_alive = false;
}

int GameObject::myGetID()
{
	return ID;
}


Boulders::Boulders(int startX, int startY, StudentWorld* world)
	    : GameObject(TID_BOULDER, startX, startY, down, 1.0, 1, world)
{
	boulderState = "stable";
	tickLife = 0;
}

Boulders::~Boulders()
{
}

void Boulders::doSomething()
{
	if (!getAlive()) {
		return;
	}
	if (boulderState == "stable") {
		if (checkUnder("Earth")) {
			return;
		}
		else {
			boulderState = "waiting";
		}		
	}
	if (boulderState == "waiting") {
		tickLife++;
		if (tickLife >= 30) {
			boulderState = "falling";
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
	}
	if (boulderState == "falling") { 
		getWorld()->fallingBoulderNearProtestor();
		if (sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 3) {
			getWorld()->getPlayer()->dead();
		}
		if (getY() == 0 || checkUnder("Earth") || checkUnder("Boulder")) {
			dead();
		}
		else {
			moveTo(getX(), getY() - 1);
		}
	}
}

bool Boulders::checkUnder(std::string type)
{
	bool under = false;
	for (int i = getX(); i <= getX() + 3; i++) {
		if (getWorld()->getGrid(i, getY()-1) == type) {
			under = true;
		}
	}
	return under;
}

void Boulders::annoy(int a)
{
}

SquirtsOfWater::SquirtsOfWater(int startX, int startY, Direction d, StudentWorld* world)
			  : GameObject(TID_WATER_SPURT, startX, startY, d, 1.0, 1, world)
{
	travelDistance = 4;
}

SquirtsOfWater::~SquirtsOfWater()
{
}

void SquirtsOfWater::doSomething()
{
	if (travelDistance == 0) {
		dead();
	}
	getWorld()->squirtNearProtestor();
	switch (getDirection()) {
	case up:
		if (getY() < 60 && getWorld()->getGrid(getX(), getY() + 1) != "Earth" && getWorld()->getGrid(getX(), getY() + 1) != "Boulder") {
			moveTo(getX(), getY() + 1);
			travelDistance--;
		}
		else {
			dead();
		}
		break;
	case down:
		if (getY() > 0 && getWorld()->getGrid(getX(), getY() - 1) != "Earth" && getWorld()->getGrid(getX(), getY() - 1) != "Boulder") {
			moveTo(getX(), getY() - 1);
			travelDistance--;
		}
		else {
			dead();
		}
		break;
	case right:
		if (getX() < 64 && getWorld()->getGrid(getX() + 1, getY()) != "Earth" && getWorld()->getGrid(getX() + 1, getY()) != "Boulder") {
			moveTo(getX() + 1, getY());
			travelDistance--;
		}
		else {
			dead();
		}
		break;
	case left:
		if (getX() > 0 && getWorld()->getGrid(getX() - 1, getY()) != "Earth" && getWorld()->getGrid(getX() - 1, getY()) != "Boulder") {
			moveTo(getX() - 1, getY());
			travelDistance--;
		}
		else {
			dead();
		}
		break;
	}
}

void SquirtsOfWater::annoy(int a)
{
}

Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* world, int ticks)
	   : GameObject(imageID, startX, startY, right, 1.0, 2, world)
{
	setVisible(false);
	tickLife = ticks;
}

Goodies::~Goodies()
{
}

int Goodies::getTickLife()
{
	return tickLife;
}

void Goodies::updateTickLife()
{
	tickLife--;
}

BarrelsOfOil::BarrelsOfOil(int startX, int startY, StudentWorld* world, int ticks)
			: Goodies(TID_BARREL, startX, startY, world, ticks)
{
	myVisible = false;
}

BarrelsOfOil::~BarrelsOfOil()
{
}

void BarrelsOfOil::doSomething()
{
	if (!getAlive()) {
		return;
	}
	if (myVisible == false && sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 4) {
		setVisible(true);
		setMyVisibleTrue();
		return;
	}
	if (sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 3) {
		dead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->foundBarrel();
	}
}

void BarrelsOfOil::annoy(int a)
{
}

void BarrelsOfOil::setMyVisibleTrue()
{
	myVisible = true;
}

GoldNuggets::GoldNuggets(int startX, int startY, StudentWorld* world, int ticks, std::string pickUp, std::string state)
		   : Goodies (TID_GOLD, startX, startY, world, ticks)
{
	whoCanPickUp = pickUp;
	nuggetState = state;
	myGoldVisible = false;
	if (nuggetState == "Temporary") {
		setVisible(true);
		myGoldVisible = true;
	}
}

GoldNuggets::~GoldNuggets()
{
}

void GoldNuggets::doSomething()
{
	if (!getAlive()) {
		return;
	}
	if (myGoldVisible == false && sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 4) {
		setVisible(true);
		setMyGoldVisible();
		return;
	}
	if (whoCanPickUp == "Tunnelman" && sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 3) {
		dead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->getPlayer()->addGoldNuggets();
	}
	if (whoCanPickUp == "Protestor" && getWorld()->isGoldNearProtestor(getX(), getY()) == true) {
		dead();
		getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	}
	if (nuggetState == "Temporary") {
		updateTickLife();
		if (getTickLife() <= 0) {
			dead();
		}
	}
}

void GoldNuggets::annoy(int a)
{
}

void GoldNuggets::setMyGoldVisible()
{
	myGoldVisible = true;
}

SonarKits::SonarKits(int startX, int startY, StudentWorld* world)
		 : Goodies(TID_SONAR, startX, startY, world, std::max(100, int(300 - 10*world->getLevel())))
{
	setVisible(true);
}

SonarKits::~SonarKits()
{
}

void SonarKits::doSomething()
{
	if (!getAlive()) {
		return;
	}
	updateTickLife();
	if (sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 3) {
		dead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addSonarCharge();
		getWorld()->increaseScore(75);
	}
	if (getTickLife() <= 0) {
		dead();
	}
}

void SonarKits::annoy(int a)
{
}

WaterRefills::WaterRefills(int startX, int startY, StudentWorld* world)
			: Goodies(TID_WATER_POOL, startX, startY, world, std::max(100, int(300 - 10 * world->getLevel())))
{
	setVisible(true);
}

WaterRefills::~WaterRefills()
{
}

void WaterRefills::doSomething()
{
	if (!getAlive()) {
		return;
	}
	updateTickLife();
	if (sqrt(pow((getWorld()->getPlayer()->getX() - getX()), 2) + pow((getWorld()->getPlayer()->getY() - getY()), 2)) <= 3) {
		dead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addWaterRefill();
		getWorld()->increaseScore(100);
	}
	if (getTickLife() <= 0) {
		dead();
	}
}

void WaterRefills::annoy(int a)
{
}

Protestor::Protestor(int imageID, StudentWorld* world, int hitPoints, int rest)
		 : GameObject(imageID, 60, 60, left, 1.0, 0, world)
{
	leaveOilField = false;
	p_hitpoints = hitPoints;
	ticksToWaitBetweenMoves = rest;
	restingTickCount = 0;
	nonRestingTickCount = 0;
	numSquaresToMoveInCurrentDirection = rand() % 60 + 8;
}

Protestor::~Protestor()
{
}


int Protestor::getNumSquaresToMoveInCurrentDirection()
{
	return numSquaresToMoveInCurrentDirection;
}

void Protestor::setNumSquaresToMoveInCurrentDirection(int c)
{
	numSquaresToMoveInCurrentDirection = c;
}

int Protestor::getRestingTickCount()
{
	return restingTickCount;
}

int Protestor::getTicksToWaitBetweenMoves()
{
	return ticksToWaitBetweenMoves;
}

void Protestor::setRestingTickCount(int count)
{
	restingTickCount = count;
}

bool Protestor::getState()
{
	return leaveOilField;
}

void Protestor::setState(bool s)
{
	leaveOilField = s;
}

int Protestor::getNonRestingTickCount()
{
	return nonRestingTickCount;
}

void Protestor::setNonRestingTickCount(int count)
{
	nonRestingTickCount = count;
}

int Protestor::getHp()
{
	return p_hitpoints;
}

void Protestor::subtractHp(int hp)
{
	p_hitpoints = p_hitpoints - hp;
}


RegularProtestor::RegularProtestor(StudentWorld* world)
				: Protestor(TID_PROTESTER, world, 5, std::max(0, int(3 - world->getLevel()/4)))
{
	canYell = 15;
	amAnnoyed = false;
	annoyedC = 0;
	perpendicularTurn = 200;
}

RegularProtestor::RegularProtestor(int imageID, StudentWorld* world, int hp)
				: Protestor(imageID, world, hp, std::max(0, int(3 - world->getLevel() / 4)))
{
	canYell = 15;
	amAnnoyed = false;
	annoyedC = 0;
	perpendicularTurn = 200;
}

RegularProtestor::~RegularProtestor()
{
}

void RegularProtestor::doSomething()
{
	if (!getAlive()) {
		return;
	}
	if (amAnnoyed == true) {
		if (getHp() <= 0) {
			setState(true);
			amAnnoyed = false;
			return;
		}
		annoyedC++;
		if (annoyedC < std::max(50, int(100 - getWorld()->getLevel()*10))) {
			return;
		}
		amAnnoyed = false;	
		annoyedC = 0;
	}
	if (getRestingTickCount() < getTicksToWaitBetweenMoves()) {
		setRestingTickCount(getRestingTickCount() + 1);
		return;
	}
	else {
		setRestingTickCount(0);
	}
	setNonRestingTickCount(getNonRestingTickCount() + 1);
	canYell++;
	perpendicularTurn++;
	if (getState() == true) {
		if (getX() == 60 && getY() == 60) {
			dead();
			getWorld()->changeNumProtestors();
			return;
		}
		else {
			// suppoed to be maze search algorithm
			dead();
			getWorld()->changeNumProtestors();
			return;
		}
	}
	if (sqrt(pow(getX()-getWorld()->getPlayer()->getX(),2) + pow(getY() - getWorld()->getPlayer()->getY(),2)) <= 4
		&& facingTunnelman() == true) {
		if (canYell >= 15) {
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			getWorld()->getPlayer()->setPlayerHitPoints(2);
			canYell = 0;
			return;
		}
		else {
			return;
		}
	}
	if (inSightOfTunnelMan() != "nope" && sqrt(pow(getX() - getWorld()->getPlayer()->getX(), 2) + pow(getY() - getWorld()->getPlayer()->getY(), 2)) > 4) {
		if (inSightOfTunnelMan() == "left") {
			setDirection(left);
			moveTo(getX() - 1, getY());
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
		if (inSightOfTunnelMan() == "right") {
			setDirection(right);
			moveTo(getX() + 1, getY());
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
		if (inSightOfTunnelMan() == "up") {
			setDirection(up);
			moveTo(getX(), getY() + 1);
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
		if (inSightOfTunnelMan() == "down") {
			setDirection(down);
			moveTo(getX(), getY() - 1);
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
	}	
	setNumSquaresToMoveInCurrentDirection(getNumSquaresToMoveInCurrentDirection() - 1);
	if (getNumSquaresToMoveInCurrentDirection() <= 0) {
		std::string dir = chooseRandDir();
		if (dir == "left") {
			if (getWorld()->checkValidSpotToMove(getX() - 1, getY()) == true) {
				setDirection(left);
			}
			else {
				if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true) {
					setDirection(up);
				}
				else {
					if (getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
						setDirection(down);
					}
					else {
						if (getWorld()->checkValidSpotToMove(getX() + 1, getY()) == true) {
							setDirection(right);
						}
					}
				}
			}
		}
		if (dir == "right") {
			if (getWorld()->checkValidSpotToMove(getX() + 1, getY()) == true) {
				setDirection(right);
			}
			else {
				if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true) {
					setDirection(up);
				}
				else {
					if (getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
						setDirection(down);
					}
					else {
						if (getWorld()->checkValidSpotToMove(getX() - 1, getY()) == true) {
							setDirection(left);
						}
					}
				}
			}
		}
		if (dir == "up") {
			if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true) {
				setDirection(up);
			}
			else {
				if (getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
					setDirection(down);
				}
				else {
					if (getWorld()->checkValidSpotToMove(getX() + 1, getY()) == true) {
						setDirection(right);
					}
					else {
						if (getWorld()->checkValidSpotToMove(getX() - 1, getY()) == true) {
							setDirection(left);
						}
					}
				}
			}
		}
		if (dir == "down") {
			if (getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
				setDirection(down);
			}
			else {
				if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true) {
					setDirection(up);
				}
				else {
					if (getWorld()->checkValidSpotToMove(getX() + 1, getY()) == true) {
						setDirection(right);
					}
					else {
						if (getWorld()->checkValidSpotToMove(getX() - 1, getY()) == true) {
							setDirection(left);
						}
					}
				}
			}
		}
		setNumSquaresToMoveInCurrentDirection(rand() % 60 + 8);
	}
	if (perpendicularTurn >= 200) {
		std::string dir = checkPerpendicular();
		if (dir == "left") {
			setDirection(left);
			setNumSquaresToMoveInCurrentDirection(rand() % 60 + 8);
			perpendicularTurn = 0;
		}
		if (dir == "right") {
			setDirection(right);
			setNumSquaresToMoveInCurrentDirection(rand() % 60 + 8);
			perpendicularTurn = 0;
		}
		if (dir == "up") {
			setDirection(up);
			setNumSquaresToMoveInCurrentDirection(rand() % 60 + 8);
			perpendicularTurn = 0;
		}
		if (dir == "down") {
			setDirection(down);
			setNumSquaresToMoveInCurrentDirection(rand() % 60 + 8);
			perpendicularTurn = 0;
		}
	}
	switch (getDirection()) {
	case up:
		if (getY() < 60 && getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true) {
			moveTo(getX(), getY() + 1);
			return;
		}
		else {
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
	case down:
		if (getY() > 0 && getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
			moveTo(getX(), getY() - 1);
			return;
		}
		else {
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
	case left:
		if (getX() > 0 && getWorld()->checkValidSpotToMove(getX() - 1, getY()) == true) {
			moveTo(getX() - 1, getY());
			return;
		}
		else {
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
	case right:
		if (getX() < 60 && getWorld()->checkValidSpotToMove(getX() + 1, getY()) == true) {
			moveTo(getX() + 1, getY());
			return;
		}
		else {
			setNumSquaresToMoveInCurrentDirection(0);
			return;
		}
	}
}

bool RegularProtestor::facingTunnelman()
{
	if (getDirection() == up) {
		if (getY() < getWorld()->getPlayer()->getY()) {
			return true;
		}
	}
	if (getDirection() == down) {
		if (getY() > getWorld()->getPlayer()->getY()) {
			return true;
		}
	}
	if (getDirection() == left) {
		if (getX() > getWorld()->getPlayer()->getX()) {
			return true;
		}
	}
	if (getDirection() == right) {
		if (getX() < getWorld()->getPlayer()->getX()) {
			return true;
		}
	}
	return false;
}

std::string RegularProtestor::inSightOfTunnelMan()
{
	for (int i = getX(); i >= 0; i--) {
		if (getWorld()->checkValidSpotToMove(i,getY()) == true) {
			if (getWorld()->getPlayer()->getX() == i && getWorld()->getPlayer()->getY() == getY()) {
				return "left";
			}
		}
		else {
			break;
		}
	}
	for (int i = getX(); i < 64; i++) {
		if (getWorld()->checkValidSpotToMove(i, getY()) == true) {
			if (getWorld()->getPlayer()->getX() == i && getWorld()->getPlayer()->getY() == getY()) {
				return "right";
			}
		}
		else {
			break;
		}
	}
	for (int i = getY(); i >= 0; i--) {
		if (getWorld()->checkValidSpotToMove(getX(), i) == true) {
			if (getWorld()->getPlayer()->getY() == i && getWorld()->getPlayer()->getX() == getX()) {
				return "down";
			}
		}
		else {
			break;
		}
	}
	for (int i = getY(); i < 64; i++) {
		if (getWorld()->checkValidSpotToMove(getX(), i) == true) {
			if (getWorld()->getPlayer()->getY() == i && getWorld()->getPlayer()->getX() == getX()) {
				return "up";
			}
		}
		else {
			break;
		}
	}
	return "nope";
}

std::string RegularProtestor::chooseRandDir()
{
	int direction = rand() % 4 + 1;
	if (direction == 4) {
			return "up";
	}
	if (direction == 3) {
			return "down";
	}
	if (direction == 2) {
			return "left";
	}
	if (direction == 1) {
			return "right";
	}
	return "wrong";
}

void RegularProtestor::annoy(int a)
{
	if (getState() == true) {
		return;
	}
	if (a == 21 || a >= 100) {
		setState(true);
		return;
	}
	subtractHp(a);
	if (getHp() <= 0) {
		if (getState() != true) {
			setState(true);
			setRestingTickCount(0);
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
			getWorld()->increaseScore(100);
		}
	}
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	setAnnoyed(true);
}

void RegularProtestor::setAnnoyed(bool a)
{
	amAnnoyed = a;
}

std::string RegularProtestor::checkPerpendicular()
{
	switch (getDirection()) {
	case left:
		if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true && getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
			int r = rand() % 2;
			if (r == 0) {
				return "up";
			}
			else {
				return "down";
			}
		}
		if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) == true) {
			return "up";
		}
		if (getWorld()->checkValidSpotToMove(getX(), getY() - 1) == true) {
			return "down";
		}
		break;
	case right:
		if (getWorld()->checkValidSpotToMove(getX(), getY() + 1) && getWorld()->checkValidSpotToMove(getX(), getY() - 1)) {
			int r = rand() % 2;
			if (r == 0) {
				return "up";
			}
			else {
				return "down";
			}
		}
		if (getWorld()->checkValidSpotToMove(getX(), getY() + 1)) {
			return "up";
		}
		if (getWorld()->checkValidSpotToMove(getX(), getY() - 1)) {
			return "down";
		}
		break;
	case down:
		if (getWorld()->checkValidSpotToMove(getX() + 1, getY()) && getWorld()->checkValidSpotToMove(getX() - 1, getY())) {
			int r = rand() % 2;
			if (r == 0) {
				return "right";
			}
			else {
				return "left";
			}
		}
		if (getWorld()->checkValidSpotToMove(getX() + 1, getY())) {
			return "right";
		}
		if (getWorld()->checkValidSpotToMove(getX() - 1, getY())) {
			return "left";
		}
		break;
	case up:
		if (getWorld()->checkValidSpotToMove(getX() + 1, getY()) && getWorld()->checkValidSpotToMove(getX() - 1, getY())) {
			int r = rand() % 2;
			if (r == 0) {
				return "right";
			}
			else {
				return "left";
			}
		}
		if (getWorld()->checkValidSpotToMove(getX() + 1, getY())) {
			return "right";
		}
		if (getWorld()->checkValidSpotToMove(getX() - 1, getY())) {
			return "left";
		}
		break;
	}
	return "none";
}

int RegularProtestor::getCanYell()
{
	return canYell;
}

int RegularProtestor::getAnnoyedC()
{
	return annoyedC;
}

bool RegularProtestor::getAnnoyed()
{
	return amAnnoyed;
}

int RegularProtestor::getPerpendicularTurn()
{
	return perpendicularTurn;
}

void RegularProtestor::setAnnoyedC(int c)
{
	annoyedC = c;
}

void RegularProtestor::setPerpendicularTurn(int p)
{
	perpendicularTurn = p;
}

void RegularProtestor::setCanYell(int y)
{
	canYell = y;
}


HardcoreProtestor::HardcoreProtestor(StudentWorld* world)
				 : RegularProtestor(TID_HARD_CORE_PROTESTER, world, 20)
{
}

HardcoreProtestor::~HardcoreProtestor()
{
}


void HardcoreProtestor::annoy(int a)
{
	if (getState() == true) {
		return;
	}
	if (a >= 100) {
		setState(true);
		return;
	}
	if (a == 21) {
		setAnnoyed(true);
		return;
	}
	subtractHp(a);
	if (getHp() <= 0) {
		if (getState() != true) {
			setState(true);
			setRestingTickCount(0);
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
			getWorld()->increaseScore(250);
		}
	}
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	setAnnoyed(true);
}
