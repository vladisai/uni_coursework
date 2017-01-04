#include "monster.h"

template <typename T>
Monster<T>::Monster(T health, T attackPower)
    : LivingEntity<T>(health)
    , AttackingEntity<T>(attackPower)
{
}

template <typename M, typename U>
void attack(M monster, U& victim)
{
    victim.takeDamage(monster.getAttackPower());
}

template <typename M, typename SheriffT>
void attack(M& monster, Sheriff<SheriffT>& victim)
{
    victim.takeDamage(monster.getAttackPower());
    monster.takeDamage(victim.getAttackPower());
}
