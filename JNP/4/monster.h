#ifndef MONSTER_H
#define MONSTER_H

#include "citizen.h"

template <typename T, int id, typename = enable_if_t<is_arithmetic<T>::value>>
class Monster;

template <typename T, int id>
class Monster<T, id> {
public:
    Monster(T health, T attackPower);
    T getHealth();
    T getAttackPower();
    void takeDamage(T damage);
    using valueType = T;

private:
    T health;
    T attackPower;
};

template <typename T>
using Zombie = Monster<T, 1>;

template <typename T>
using Vampire = Monster<T, 2>;

template <typename T>
using Mummy = Monster<T, 3>;

template <typename M, typename U>
void attack(M monster, U& victim);

// Definitions

template <typename T, int id>
Monster<T, id>::Monster(T health, T attackPower)
    : health(health)
    , attackPower(attackPower) {
    assert(health >= 0);
    assert(attackPower >= 0);
}

template <typename T, int id>
T Monster<T, id>::getHealth() {
    return health;
}

template <typename T, int id>
T Monster<T, id>::getAttackPower() {
    return attackPower;
}

template <typename T, int id>
void Monster<T, id>::takeDamage(T damage) {
    health -= damage;
    health = max(0, health);
}

template <typename M, typename U>
void attack(M monster, U& victim) {
    victim.takeDamage(monster.getAttackPower());
}

template <typename M, typename SheriffT>
void attack(M& monster, Sheriff<SheriffT>& victim) {
    victim.takeDamage(monster.getAttackPower());
    monster.takeDamage(victim.getAttackPower());
}

#endif  // MONSTER_H
