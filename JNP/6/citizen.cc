#include "citizen.h"
#include <cassert>

Citizen::Citizen(HealthPoints health, Age age)
	: _LivingEntity(health), _AgeingEntity(age) {}

Adult::Adult(HealthPoints health, Age age) : Citizen(health, age) {
	checkAge(age);
}

bool Adult::checkAge(Age age) {
	if (age < 18 || age > 100)
		return false;
	return true;
}

Teenager::Teenager(HealthPoints health, Age age) : Citizen(health, age) {
	checkAge(age);
}

bool Teenager::checkAge(Age age) {
	if (age < 11 || age > 18)
		return false;
	return true;
}

Sheriff::Sheriff(HealthPoints health, Age age, AttackPower attackPower)
	: Adult(health, age), _AttackingEntity(attackPower) {}

std::shared_ptr<Adult> createAdult(HealthPoints health, Age age) {
	return  std::shared_ptr<Adult>( new Adult(health, age) );
}

std::shared_ptr<Teenager> createTeenager(HealthPoints health, Age age) {
	return std::shared_ptr<Teenager> ( new Teenager(health, age) );
}

std::shared_ptr<Sheriff> createSheriff(HealthPoints health, Age age, AttackPower attackPower) {
	return std::shared_ptr<Sheriff>(new Sheriff(health, age, attackPower) ) ;
}
