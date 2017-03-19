#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include "citizen.h"
#include "helper.h"
#include "monster.h"

#include <memory>
#include <vector>

using Time = int;

using std::vector;
using std::shared_ptr;

namespace jnp {

class Checker { // abstract class for checking function
  public:
    virtual bool isTimeToAttack(Time time) const = 0;

    virtual ~Checker() = default;
};

class DefaultChecker : public Checker { // def checker(final class)
  public:
    bool isTimeToAttack(Time time) const {
        return (time % 3 == 0 || time % 13 == 0) && time % 7 != 0;
    }

    virtual ~DefaultChecker() = default;
};

class Clock { // the clock of town
  private:
    shared_ptr<Checker> checker;

    Time currentTime;

    const Time maxTime;

  public:
    Clock(Time _currentTime, Time _maxTime, shared_ptr<Checker> _checkFunction);

    bool tick(Time timeStep);
};

class Status { // Status class for getStatus()
  public:
    Status(string monsterName, HealthPoints monsterHp,
           unsigned int citizensAlive);

    string getMonsterName() const { return monsterName; }

    HealthPoints getMonsterHealth() const { return monsterHp; }

    unsigned int getAliveCitizens() const { return citizensAlive; }

  private:
    string monsterName;
    HealthPoints monsterHp;
    unsigned int citizensAlive;
};
}

class SmallTown {
  public:
    void tick(Time timeStep);

    jnp::Status getStatus() const;

    class Builder {
      public:
        Builder();

        Builder &monster(shared_ptr<Monster> _monster);

        Builder &startTime(Time time);

        Builder &maxTime(Time time);

        Builder &citizen(shared_ptr<Citizen> _citizen);

        Builder &checker(shared_ptr<jnp::Checker> _checker);

        SmallTown build() const;

      private:
        shared_ptr<Monster> _monster;

        Time _startTime;

        Time _maxTime;

        vector<shared_ptr<Citizen>> _citizens;

        shared_ptr<jnp::Checker> _checker;

        bool maxTimeSet, checkerSet, monsterSet, startTimeSet;
    };

  private:
    shared_ptr<Monster> monster;

    vector<shared_ptr<Citizen>> citizens;

    shared_ptr<jnp::Clock> clock;

    int citizensAlive;

    SmallTown(vector<shared_ptr<Citizen>> citizens, shared_ptr<Monster> monster,
              shared_ptr<jnp::Clock> clock);
};

#endif // SMALLTOWN_H
