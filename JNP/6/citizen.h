#ifndef CITIZEN_H
#define CITIZEN_H

#include "helper.h"
#include <memory>

using std::shared_ptr;

class Citizen : public _LivingEntity,
                public _AgeingEntity,
                public virtual _EntityWithAttackPower {
  public:
    Citizen(HealthPoints health, Age age);

  private:
};

class Adult : public Citizen, public _PeacefulEntity {
  public:
    Adult(HealthPoints health, Age age);

  private:
    bool checkAge(Age age);
};

class Teenager : public Citizen, public _PeacefulEntity {
  public:
    Teenager(HealthPoints health, Age age);

  private:
    bool checkAge(Age age);
};

class Sheriff : public Citizen, public _AttackingEntity {
  public:
    Sheriff(HealthPoints health, Age age, AttackPower attackPower);

  private:
    bool checkAge(Age age);
};

shared_ptr<Adult> createAdult(HealthPoints health, Age age);

shared_ptr<Teenager> createTeenager(HealthPoints health, Age age);

shared_ptr<Sheriff> createSheriff(HealthPoints health, Age age,
                                  AttackPower attackPower);

#endif // CITIZEN_H
