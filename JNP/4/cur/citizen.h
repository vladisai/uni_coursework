#ifndef CITIZEN_H
#define CITIZEN_H

#include <cassert>
#include <type_traits>

using std::enable_if_t;

template <typename T, int minAge, int maxAge, bool canAttack>
class Citizen {
    T health;
    T age;
    T attackPower;

    public:
        template <bool CA = canAttack>
        Citizen(std::enable_if_t<!CA, T> health, T age) : health(health), age(age) {
            assert(age >= minAge);
            assert(age <= maxAge);
            assert(health >= 0);
        }

        template <bool CA = canAttack>
        Citizen(std::enable_if_t<CA, T> health, T age, T attackPower) : health(health), age(age), attackPower(attackPower) {
            assert(age >= minAge);
            assert(age <= maxAge);
            assert(health >= 0);
            assert(attackPower >= 0);
        }

        T getHealth() const { return health; }

        T getAge() const { return age; }

        template <bool CA = canAttack>
        std::enable_if_t<CA, T> getAttackPower() const {
                return attackPower;
            };

        void takeDamage(T damage) {
            health -= damage;
            health = health < 0 ? 0 : health;
        }
};

template <typename T>
using Adult = Citizen<T, 18, 100, false>;

template <typename T>
using Teenager = Citizen<T, 11, 17, false>;

template <typename T>
using Sheriff = Citizen<T, 18, 100, true>;

namespace IsSheriff {
    template<typename T> constexpr bool get = false;
    template<typename T> constexpr bool get<Sheriff<T>> = true;
}

#endif  // CITIZEN_H
