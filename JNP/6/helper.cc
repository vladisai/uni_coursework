#include "helper.h"
#include <algorithm>
#include <cassert>
#include <iostream>

using std::max;

bool _EntityWithHealth::isAlive() const {
    return this->getHealth() > 0;
}

_LivingEntity::_LivingEntity(HealthPoints health) : health(health) {}

HealthPoints _LivingEntity::getHealth() const { return health; }
void _LivingEntity::takeDamage(AttackPower damage) {
    health = max(0, health - damage);
}

_AgeingEntity::_AgeingEntity(Age age) : age(age) {}

Age _AgeingEntity::getAge() const { return age; }

_AttackingEntity::_AttackingEntity(AttackPower attackPower)
    : attackPower(attackPower) {}

AttackPower _AttackingEntity::getAttackPower() const { return attackPower; }

AttackPower _PeacefulEntity::getAttackPower() const { return 0; }
