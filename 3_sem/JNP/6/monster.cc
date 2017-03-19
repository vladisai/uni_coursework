#include "monster.h"
#include <algorithm>

using std::make_shared;
using std::find;

void Monster::attack(shared_ptr<Citizen> citizen) {
    bool isCitizenAliveBeforeFight = citizen->isAlive();
    if (this->isAlive()) {
        citizen->takeDamage(this->getAttackPower());
    }
    if (isCitizenAliveBeforeFight) {
        this->takeDamage(citizen->getAttackPower());
    }
}

SingleMonster::SingleMonster(HealthPoints health, AttackPower attackPower)
    : _LivingEntity(health), _AttackingEntity(attackPower) {}

Zombie::Zombie(HealthPoints health, AttackPower attackPower)
    : SingleMonster(health, attackPower) {}

const string Zombie::CreatureName = "Zombie";

string Zombie::getName() const { return Zombie::CreatureName; }

Vampire::Vampire(HealthPoints health, AttackPower attackPower)
    : SingleMonster(health, attackPower) {}

const string Vampire::CreatureName = "Vampire";

string Vampire::getName() const { return Vampire::CreatureName; }

Mummy::Mummy(HealthPoints health, AttackPower attackPower)
    : SingleMonster(health, attackPower) {}

const string Mummy::CreatureName = "Mummy";

string Mummy::getName() const { return Mummy::CreatureName; }

shared_ptr<Zombie> createZombie(HealthPoints health, AttackPower attackPower) {
    return make_shared<Zombie>(health, attackPower);
}

shared_ptr<Vampire> createVampire(HealthPoints health,
                                  AttackPower attackPower) {
    return make_shared<Vampire>(health, attackPower);
}

shared_ptr<Mummy> createMummy(HealthPoints health, AttackPower attackPower) {
    return make_shared<Mummy>(health, attackPower);
}

shared_ptr<GroupOfMonsters>
createGroupOfMonsters(const vector<shared_ptr<SingleMonster>> &monsters) {
    return make_shared<GroupOfMonsters>(monsters);
}

shared_ptr<GroupOfMonsters> createGroupOfMonsters(
    const initializer_list<shared_ptr<SingleMonster>> &monsters) {
    return make_shared<GroupOfMonsters>(monsters);
}

GroupOfMonsters::GroupOfMonsters(
    const vector<shared_ptr<SingleMonster>> &monsters) {
    for (auto monster : monsters) {
        if (find(this->monsters.begin(), this->monsters.end(), monster) ==
            this->monsters.end()) {
            this->monsters.push_back(monster);
        }
    }
}

GroupOfMonsters::GroupOfMonsters(
    const initializer_list<shared_ptr<SingleMonster>> &monsters)
    : GroupOfMonsters(vector<shared_ptr<SingleMonster>>(monsters.begin(),
                                                        monsters.end())) {}

const string GroupOfMonsters::CreatureName = "GroupOfMonsters";

string GroupOfMonsters::getName() const {
    return GroupOfMonsters::CreatureName;
}

void GroupOfMonsters::takeDamage(AttackPower attackPower) {
    for (auto monster : monsters) {
        monster->takeDamage(attackPower);
    }
}

HealthPoints GroupOfMonsters::getHealth() const {
    HealthPoints result{};
    for (auto monster : monsters) {
        result += monster->getHealth();
    }
    return result;
}

AttackPower GroupOfMonsters::getAttackPower() const {
    AttackPower result{};
    for (auto monster : monsters) {
        if (monster->isAlive()) {
            result += monster->getAttackPower();
        }
    }
    return result;
}
