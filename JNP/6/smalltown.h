#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <list>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <vector>
#include "helper.h"
#include "citizen.h"
#include "monster.h"


using Time = unsigned int;


namespace jnp
{

	class Checker { //abstract class for checking function
	public:
		virtual bool isTimeToAttack(Time time) const = 0;
	};

	class DefaultChecker : public Checker { //def checker(final class)
	public:
		bool isTimeToAttack(Time time) const {
			return (time % 3 == 0 || time % 13 == 0) && time % 7 != 0;
		}
	};
	class Clock { //the clock of town
	private:
		std::shared_ptr<Checker> checker;
		Time currentTime;
		Time maxTime;
	public:
		Clock(Time _currentTime, Time _maxTime, std::shared_ptr<Checker> _checkFunction);
		bool tick(Time timeStep);

	};
	class Status { //Status class for getStatus()
	public:
		Status(const std::string & monster, HealthPoints monsterHp, unsigned int citizensAlive)
			: monsterName(monster)
			, monsterHp(monsterHp)
			, citizensAlive(citizensAlive) {}

		std::string getMonsterName() const {
			return monsterName;
		}

		HealthPoints getMonsterHealth() const {
			return monsterHp;
		}

		unsigned int getAliveCitizens() const {
			return citizensAlive;
		}

	private:
		const std::string monsterName;
		HealthPoints monsterHp;
		unsigned int citizensAlive;
	};
}

class SmallTown {
public :
	void tick(Time timeStep);
	jnp::Status getStatus();
	class Status {
public:
	Status(const std::string & monster, HealthPoints monsterHp, unsigned int citizensAlive)
		: monsterName(monster)
		, monsterHp(monsterHp)
		, citizensAlive(citizensAlive) {}

	std::string getMonsterName() const {
		return monsterName;
	}

	HealthPoints getMonsterHealth() const {
		return monsterHp;
	}

	unsigned int getAliveCitizens() const {
		return citizensAlive;
	}

private:
	const std::string monsterName;
	HealthPoints monsterHp;
	unsigned int citizensAlive;
};
	class Builder {
	public:
		Builder();
		Builder & monster(std::shared_ptr<Monster> _monster);
		Builder & startTime(Time time);
		Builder & maxTime(Time time);
		Builder & citizen(std::shared_ptr<Citizen> _citizen);
		Builder & checker(std::shared_ptr<jnp::Checker> _checker);
		SmallTown build();
	private:
		std::shared_ptr<Monster> _monster;
		Time _startTime;
		Time _maxTime;
		vector < std::shared_ptr<Citizen> > _citizens;
		std::shared_ptr<jnp::Checker> _checker;
		bool maxTimeSet, checkerSet, monsterSet, startTimeSet;
	};
private :
	std::shared_ptr<Monster> monster;
	vector <std::shared_ptr <Citizen> > citizens;
	std::shared_ptr <jnp::Clock> clock;
	int citizensAlive;
	SmallTown(vector <std::shared_ptr<Citizen> > citizens, std::shared_ptr<Monster> monster, std::shared_ptr <jnp::Clock> clock);

};


#endif // SMALLTOWN_H
