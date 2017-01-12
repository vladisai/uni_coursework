#ifndef MONSTER_H
#define MONSTER_H

#include "citizen.h"
#include "helper.h"
#include <initializer_list>
#include <memory>
#include <set>
#include <string>
#include <vector>

using std::vector;
using std::initializer_list;
using std::string;
using std::set;
using std::shared_ptr;

class Monster : public virtual _EntityWithHealth,
                public virtual _EntityWithAttackPower {
  public:
    virtual string getName() = 0;

    void attack(shared_ptr<Citizen> citizen);
};

class SingleMonster : public Monster,
                      public _LivingEntity,
                      public _AttackingEntity {
  public:
    SingleMonster(HealthPoints health, AttackPower attackPower);
};

class Zombie : public SingleMonster {
  public:
    Zombie(HealthPoints health, AttackPower attackPower);
    string getName() override;

  private:
    static const string CreatureName;
};

class Vampire : public SingleMonster {
  public:
    Vampire(HealthPoints health, AttackPower attackPower);
    string getName() override;

  private:
    static const string CreatureName;
};

class Mummy : public SingleMonster {
  public:
    Mummy(HealthPoints health, AttackPower attackPower);
    string getName() override;

  private:
    static const string CreatureName;
};

class GroupOfMonsters : public Monster {
  public:
    GroupOfMonsters(const vector<shared_ptr<SingleMonster>> &monsters);

    GroupOfMonsters(
        const initializer_list<shared_ptr<SingleMonster>> &monsters);

    string getName() override;

    virtual void takeDamage(AttackPower attackPower) override;

    virtual AttackPower getAttackPower() override;

    HealthPoints getHealth() override;

  private:
    const set<shared_ptr<SingleMonster>> monsters;
};

shared_ptr<Zombie> createZombie(HealthPoints health, AttackPower attackPower);

shared_ptr<Vampire> createVampire(HealthPoints health, AttackPower attackPower);

shared_ptr<Mummy> createMummy(HealthPoints health, AttackPower attackPower);

shared_ptr<GroupOfMonsters>
createGroupOfMonsters(const vector<shared_ptr<SingleMonster>> &monsters);

shared_ptr<GroupOfMonsters> createGroupOfMonsters(
    const initializer_list<shared_ptr<SingleMonster>> &monsters);

#endif // MONSTER_H
