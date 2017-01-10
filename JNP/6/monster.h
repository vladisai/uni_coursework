#ifndef MONSTER_H
#define MONSTER_H

#include "helper.h"
#include <initializer_list>
#include <set>
#include <string>
#include <vector>

using std::vector;
using std::initializer_list;
using std::string;
using std::set;

class Monster : public virtual _EntityWithHealth, public virtual _EntityWithAttackPower {
  public:
    virtual string getName() = 0;
};

class SingleMonster : public virtual Monster, public virtual _LivingEntity, public virtual _AttackingEntity {
  public:
    SingleMonster(HealthPoints health, AttackPower attackPower);
};

class Zombie : public virtual SingleMonster {
  public:
    Zombie(HealthPoints health, AttackPower attackPower);
    string getName() override;

  private:
    static const string CreatureName;
};

class Vampire : public virtual SingleMonster {
  public:
    Vampire(HealthPoints health, AttackPower attackPower);
    string getName() override;

  private:
    static const string CreatureName;
};

class Mummy : public virtual SingleMonster {
  public:
    Mummy(HealthPoints health, AttackPower attackPower);
    string getName() override;

  private:
    static const string CreatureName;
};

Zombie *createZombie(HealthPoints health, AttackPower attackPower);

Vampire *createVampire(HealthPoints health, AttackPower attackPower);

Mummy *createMummy(HealthPoints health, AttackPower attackPower);

class GroupOfMonsters : public Monster {
  public:
    GroupOfMonsters(const vector<SingleMonster *> monsters);

    GroupOfMonsters(const initializer_list<SingleMonster *> monsters);

    string getName() override;

    virtual void takeDamage(AttackPower attackPower) override;

    virtual AttackPower getAttackPower() override;

    HealthPoints getHealth() override;

  private:
    const set<SingleMonster *> monsters;
};

#endif // MONSTER_H
