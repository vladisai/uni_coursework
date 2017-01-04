#include "helper.h"

_LivingEntity::_LivingEntity(HealthPoints health) : health(health) {}

HealthPoints _LivingEntity::getHealth() { return health; }
void _LivingEntity::takeDamage(AttackPower damage) { health -= damage; }

_AgeingEntity::_AgeingEntity(Age age) : age(age) {}

Age _AgeingEntity::getAge() { return age; }
_AttackingEntity::_AttackingEntity(AttackPower attackPower)
    : attackPower(attackPower) {}
AttackPower _AttackingEntity::getAttackPower() { return attackPower; }

