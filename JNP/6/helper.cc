#include "helper.h"
#include <cassert>
#include <algorithm>
#include <iostream>

using std::max;

_LivingEntity::_LivingEntity(HealthPoints health) : health(health) {}

HealthPoints _LivingEntity::getHealth() { return health; }
void _LivingEntity::takeDamage(AttackPower damage) { health = max(0, health - damage); }

_AgeingEntity::_AgeingEntity(Age age) : age(age) {}

Age _AgeingEntity::getAge() { return age; }

_AttackingEntity::_AttackingEntity(AttackPower attackPower)
    : attackPower(attackPower) {}

AttackPower _AttackingEntity::getAttackPower() { return attackPower; }

AttackPower _PeacefulEntity::getAttackPower() { return 0; }
