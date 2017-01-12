#include "citizen.h"
#include <cassert>

using std::make_shared;

Citizen::Citizen(HealthPoints health, Age age)
    : _LivingEntity(health), _AgeingEntity(age) {}

Adult::Adult(HealthPoints health, Age age) : Citizen(health, age) {
    assert(checkAge(age));
}

bool Adult::checkAge(Age age) {
    if (age < 18 || age > 100) {
        return false;
    } else {
        return true;
    }
}

Teenager::Teenager(HealthPoints health, Age age) : Citizen(health, age) {
    assert(checkAge(age));
}

bool Teenager::checkAge(Age age) {
    if (age < 11 || age > 18) {
        return false;
    } else {
        return true;
    }
}

Sheriff::Sheriff(HealthPoints health, Age age, AttackPower attackPower)
    : Citizen(health, age), _AttackingEntity(attackPower) {}

bool Sheriff::checkAge(Age age) {
    if (age < 18 || age > 100) {
        return false;
    } else {
        return true;
    }
}

shared_ptr<Adult> createAdult(HealthPoints health, Age age) {
    return make_shared<Adult>(health, age);
}

shared_ptr<Teenager> createTeenager(HealthPoints health, Age age) {
    return make_shared<Teenager>(health, age);
}

shared_ptr<Sheriff> createSheriff(HealthPoints health, Age age,
                                  AttackPower attackPower) {
    return make_shared<Sheriff>(health, age, attackPower);
}
