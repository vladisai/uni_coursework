#ifndef CITIZEN_H
#define CITIZEN_H

#include "helper.h"
#include <memory>
class Citizen : public _LivingEntity, public _AgeingEntity {
public:
	Citizen(HealthPoints health, Age age);

private:
};

class Adult : public Citizen {
public:
	Adult(HealthPoints health, Age age);

private:
	bool checkAge(Age age) override final;
};

class Teenager : public Citizen {
public:
	Teenager(HealthPoints health, Age age);
	bool checkAge(Age age) final;
};

class Sheriff : public Adult, public _AttackingEntity {
public:
	Sheriff(HealthPoints health, Age age, AttackPower attackPower);
};

std::shared_ptr<Adult> createAdult(HealthPoints health, Age age);

std::shared_ptr<Teenager> createTeenager(HealthPoints health, Age age);

std::shared_ptr<Sheriff> createSheriff(HealthPoints health, Age age, AttackPower attackPower);

#endif // CITIZEN_H
