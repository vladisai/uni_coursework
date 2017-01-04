#ifndef CITIZEN_H
#define CITIZEN_H

#include <cassert>
#include <type_traits>

using std::enable_if_t;
using std::is_arithmetic;

template <typename T, int minAge, int maxAge, bool canAttack, typename = enable_if_t<is_arithmetic<T>::value> >
class Citizen;

template <typename T, int minAge, int maxAge>
class Citizen<T, minAge, maxAge, false> {
public:
    Citizen(T health, T age);
    T getHealth();
    void takeDamage(T damage);
    T getAge();

private:
    T health;
    T age;
};

template <typename T, int minAge, int maxAge>
class Citizen<T, minAge, maxAge, true> : public Citizen<T, minAge, maxAge, false> {
public:
    Citizen(T health, T age, T attackPower);
    T getAttackPower();

private:
    T attackPower;
};

template <typename T>
using Adult = Citizen<T, 18, 100, false>;

template <typename T>
using Teenager = Citizen<T, 11, 17, false>;

template <typename T>
using Sheriff = Citizen<T, 18, 100, true>;

// Definitions

template <typename T, int minAge, int maxAge>
Citizen<T, minAge, maxAge, false>::Citizen(T health, T age)
    : health(health)
    , age(age)
{
    static_assert(minAge <= maxAge, "Minimum age is greater than maximum age.");
    static_assert(minAge >= 0, "Minimum age is less than zero.");
    assert(age >= minAge);
    assert(age <= maxAge);
    assert(health >= 0);
}

template <typename T, int minAge, int maxAge>
Citizen<T, minAge, maxAge, true>::Citizen(T health, T age, T attackPower)
    : Citizen<T, minAge, maxAge, false>(health, age)
    , attackPower(attackPower)
{
    assert(attackPower >= 0);
}

template <typename T, int minAge, int maxAge>
T Citizen<T, minAge, maxAge, false>::getAge()
{
    return age;
}

template <typename T, int minAge, int maxAge>
T Citizen<T, minAge, maxAge, false>::getHealth()
{
    return health;
}

template <typename T, int minAge, int maxAge, bool canAttack>
void Citizen<T, minAge, maxAge, canAttack>::takeDamage(T damage)
{
    health -= damage;
    health = max(health, 0);
}

template <typename T, int minAge, int maxAge>
T Citizen<T, minAge, maxAge, true>::getAttackPower()
{
    return attackPower;
}

#endif // CITIZEN_H
