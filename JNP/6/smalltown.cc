#include "smalltown.h"

#include <iostream>
#include <algorithm>
#include <stdexcept>

using std::find;
using std::cout;
using std::endl;
using std::vector;
using std::exception;
using std::shared_ptr;

class valueAlreadySet : public exception {
  public:
    char const *what() const throw() {
        return "Exception: Tried to set value for the 2nd time";
    }
};

class notFullBuild : public exception {
  public:
    char const *what() const throw() {
        return "Exception: Builder not fully initialised";
    }
};

class illegalArgument : public exception {
  public:
    char const *what() const throw() { return "Exception: wrong attribute"; }
};

jnp::Clock::Clock(Time _currentTime, Time _maxTime,
                  shared_ptr<jnp::Checker> _checkFunction)
    : checker(_checkFunction), currentTime(_currentTime), maxTime(_maxTime) {}

bool jnp::Clock::tick(Time timeStep) {
    bool attackTime = checker->isTimeToAttack(currentTime);
    currentTime = currentTime + timeStep;
    currentTime %= maxTime;
    return attackTime;
}

jnp::Status::Status(string monsterName, HealthPoints monsterHp,
                    unsigned int citizensAlive)
    : monsterName(monsterName), monsterHp(monsterHp),
      citizensAlive(citizensAlive) {}

SmallTown::Builder::Builder() : _checker(new jnp::DefaultChecker()) {
    _monster = nullptr;
    _startTime = -1;
    _maxTime = -1;
    maxTimeSet = checkerSet = monsterSet = startTimeSet = false;
}

SmallTown::Builder &
SmallTown::Builder::checker(shared_ptr<jnp::Checker> _checker) {
    if (checkerSet)
        throw new valueAlreadySet();
    checkerSet = true;
    _checker = _checker;
    return *this;
}

SmallTown::Builder &
SmallTown::Builder::monster(shared_ptr<Monster> monster) {
    if (monsterSet)
        throw new valueAlreadySet();
    monsterSet = true;
    _monster = monster;
    return *this;
}

SmallTown::Builder &SmallTown::Builder::maxTime(Time time) {
    if (maxTimeSet)
        throw new valueAlreadySet();
    if (time < 0)
        throw new illegalArgument();
    maxTimeSet = true;
    _maxTime = time;
    return *this;
}

SmallTown::Builder &SmallTown::Builder::startTime(Time time) {
    if (startTimeSet)
        throw new valueAlreadySet();
    if (time < 0)
        throw new illegalArgument();
    startTimeSet = true;
    _startTime = time;
    return *this;
}

SmallTown::Builder &
SmallTown::Builder::citizen(shared_ptr<Citizen> citizen) {
    if (find(_citizens.begin(), _citizens.end(), citizen) == _citizens.end()) {
        _citizens.push_back(citizen);
    }
    return *this;
}

shared_ptr<jnp::Clock> createClock(Time start, Time max,
                                        shared_ptr<jnp::Checker> checker) {
    return shared_ptr<jnp::Clock>(new jnp::Clock(start, max, checker));
}

SmallTown SmallTown::Builder::build() const {
    if (!maxTimeSet || !startTimeSet || !monsterSet || _citizens.size() == 0)
        throw new notFullBuild();
    if (_maxTime < _startTime)
        throw new illegalArgument();
    shared_ptr<jnp::Clock> clock =
        createClock(_startTime, _maxTime, _checker);
    return SmallTown(_citizens, _monster, clock);
}

SmallTown::SmallTown(vector<shared_ptr<Citizen>> _citizens,
                     shared_ptr<Monster> _monster,
                     shared_ptr<jnp::Clock> _clock)
    : monster(_monster), citizens(_citizens), clock(_clock),
      citizensAlive(_citizens.size()) {}

jnp::Status SmallTown::getStatus() const {
    return jnp::Status(monster->getName(), monster->getHealth(), citizensAlive);
}

void SmallTown::tick(Time timeStep) {
    if (monster->getHealth() == 0 && citizensAlive == 0) {
        cout << "DRAW" << endl;
    } else if (citizensAlive == 0) {
        cout << "MONSTER WON" << endl;
    } else if (monster->getHealth() == 0) {
        cout << "CITIZENS WON" << endl;
    }
    if (clock->tick(timeStep)) // updates time and says if it is attack time
    {
        for (auto citizen : citizens) {
            if (citizen->getHealth() > 0) {
                monster->attack(citizen); // counterattack included
                if (citizen->getHealth() == 0)
                    citizensAlive--;
            }
        }
    }
}
