#include "smalltown.h"
#include <memory>
#include <assert.h>
#include <iostream>
class settedValue : public std::exception {
public:
	char const * what() const throw() {
		return "Exception: Tryed to set value for the 2nd time";
	}
};

class notFullBuild : public std::exception {
public:
	char const * what() const throw() {
		return "Exception: Builder not fully initialised";
	}
};



jnp::Clock::Clock(Time _currentTime, Time _maxTime, std::shared_ptr <jnp::Checker> _checkFunction)
	: currentTime(_currentTime)
	, maxTime(_maxTime)
	, checker(_checkFunction)
{}

bool jnp::Clock::tick(Time timeStep)
{
	bool attackTime = checker->isTimeToAttack(currentTime);
	currentTime = currentTime + timeStep;
	currentTime %= maxTime;
	return attackTime;

}



SmallTown::Builder::Builder()
	: _checker(new jnp::DefaultChecker())
{
	_monster = nullptr;
	_startTime = -1;
	_maxTime = -1;
	maxTimeSet = checkerSet = monsterSet = startTimeSet = false;
}

SmallTown::Builder & SmallTown::Builder::checker(std::shared_ptr<jnp::Checker> _checker)
{
	if (checkerSet)
		throw new settedValue();
	checkerSet = true;
	_checker = _checker;
	return *this;
}

SmallTown::Builder & SmallTown::Builder::monster(std::shared_ptr<Monster> monster)
{
	if (monsterSet)
		throw new settedValue();
	monsterSet = true;
	_monster = monster;
	return *this;
}

SmallTown::Builder & SmallTown::Builder::maxTime(Time time)
{
	if (maxTimeSet)
		throw new settedValue();
	maxTimeSet = true;
	_maxTime = time;
	return *this;
}

SmallTown::Builder & SmallTown::Builder::startTime(Time time)
{
	if (startTimeSet)
		throw new settedValue();
	startTimeSet = true;
	_startTime = time;
	return *this;
}

SmallTown::Builder & SmallTown::Builder::citizen(std::shared_ptr<Citizen> citizen)
{
	_citizens.push_back(citizen);
	return *this;
}

std::shared_ptr<jnp::Clock> createClock(Time start, Time max, std::shared_ptr<jnp::Checker> checker)
{
	return std::shared_ptr<jnp::Clock>(new jnp::Clock(start, max, checker));
}


SmallTown SmallTown::Builder::build()
{
	if (!maxTimeSet || !startTimeSet || !monsterSet || _citizens.size() == 0)
		throw new notFullBuild();
	std::shared_ptr<jnp::Clock> clock = createClock(_startTime, _maxTime, _checker);
	return SmallTown(_citizens, _monster, clock);

}

SmallTown::SmallTown
(vector <std::shared_ptr<Citizen> > _citizens, std::shared_ptr<Monster> _monster, std::shared_ptr <jnp::Clock> _clock)
	: citizens(_citizens)
	, monster(_monster)
	, clock(_clock)
	, citizensAlive(_citizens.size())
{}


jnp::Status SmallTown::getStatus()
{
	return jnp::Status(monster->getName(), monster->getHealth(), citizensAlive);
}

void SmallTown::tick(Time timeStep)
{
	if (monster->getHealth() == 0 && citizensAlive == 0)
	{
		std::cout << "DRAW" << std::endl;
	}
	else if (citizensAlive == 0)
	{
		std::cout << "MONSTER WON" << std::endl;
	}
	else if (monster->getHealth() == 0)
	{
		std::cout << "CITIZENS WON" << std::endl;
	}
	if (clock->tick(timeStep)) //updates time and says if it is attack time 
	{
		for (auto citizen : citizens)
		{
			if (citizen->getHealth() > 0)
			{
				monster->attack(citizen); //counterattack included
			}
		}
	}
}
