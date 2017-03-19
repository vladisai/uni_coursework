#include "citizen.h"

template <typename T>
LivingEntity<T>::LivingEntity(T health)
    : health(health)
{
    assert(health > 0);
}

template <typename T>
void LivingEntity<T>::takeDamage(T damage)
{
    health = max(0, health - damage);
}

template <typename T>
T LivingEntity<T>::getHealth()
{
    return health;
}

template <typename T>
AttackingEntity<T>::AttackingEntity(T attackPower)
    : attackPower(attackPower)
{
    assert(attackPower > 0);
}

template <typename T>
void AttackingEntity<T>::getAttackPower()
{
    return attackPower;
}

template <typename T, int minAge, int maxAge>
Citizen<T, minAge, maxAge>::Citizen(T health, T age)
    : LivingEntity<T>(health)
    , age(age)
{
    staticassert(minAge <= age, "Minimum age is bigger than the age provided.");
    staticassert(maxAge >= age, "Maximum age is smaller than the age provided.");
}

template <typename T, int minAge, int maxAge>
T Citizen<T, minAge, maxAge>::getAge()
{
    return age;
}

template <typename T>
Sheriff<T>::Sheriff(T health, T age, T attackPower)
    : Adult<T>(health, age)
    , AttackingEntity<T>(attackPower)
{
}

template <typename T>
T Sheriff<T>::getAttackPower()
{
    return attackPower;
}
