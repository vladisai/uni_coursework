#ifndef MONSTER_H
#define MONSTER_H

#include "citizen.h"

enum monster { zombie, vampire, mummy };

template <typename T, monster id>
class Monster {
    T health;
    T attackPower;

    public:
        Monster(T health, T attackPower) : health(health), attackPower(attackPower) {
            assert(health >= 0);
            assert(attackPower >= 0);
        }

        T getHealth() const { return health; }
        T getAttackPower() const { return attackPower; }

        void takeDamage(T damage) {
            health -= damage;
            health = health < 0 ? 0 : health;
        }

        using valueType = T;

};

template <typename T>
using Zombie = Monster<T, zombie>;

template <typename T>
using Vampire = Monster<T, vampire>;

template <typename T>
using Mummy = Monster<T, mummy>;

namespace TypeName {
    template<typename T> constexpr const char *get = "unavailable";
    template<typename T> constexpr const char *get<Monster<T, zombie>> = "Zombie";
    template<typename T> constexpr const char *get<Monster<T, vampire>> = "Vampire";
    template<typename T> constexpr const char *get<Monster<T, mummy>> = "Mummy";
}

template <typename M, typename U>
typename std::enable_if<!IsSheriff::get<U>, void>::type attack(M &monster, U &victim) {
    victim.takeDamage(monster.getAttackPower());
}

template <typename M, typename U>
typename std::enable_if<IsSheriff::get<U>, void>::type attack(M &monster, U &victim) {
    monster.takeDamage(victim.getAttackPower());
    victim.takeDamage(monster.getAttackPower());
}

#endif // MONSTER_H
