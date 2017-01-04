#include "citizen.h"
#include "helper.h"
#include <cassert>

Citizen::Citizen(HealthPoints health, Age age)
    : _LivingEntity(health), _AgeingEntity(age) {}

Adult::Adult(HealthPoints health, Age age) : Citizen(health, age) {
    assert(age >= 18);
    assert(age <= 100);
}

Teenager::Teenager(HealthPoints health, Age age) : Citizen(health, age) {
    assert(age >= 11);
    assert(age < 18);
}

Sheriff::Sheriff(HealthPoints health, Age age, AttackPower attackPower) : Adult(health, age), _AttackingEntity(attackPower) {}
