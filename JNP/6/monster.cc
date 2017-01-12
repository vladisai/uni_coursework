#include "monster.h"

SingleMonster::SingleMonster(HealthPoints health, AttackPower attackPower)
    : _LivingEntity(health), _AttackingEntity(attackPower) {}

Zombie::Zombie(HealthPoints health, AttackPower attackPower)
    : SingleMonster(health, attackPower) {}

const string Zombie::CreatureName = "Zombie";

string Zombie::getName() { return Zombie::CreatureName; }

Vampire::Vampire(HealthPoints health, AttackPower attackPower)
    : SingleMonster(health, attackPower) {}

const string Vampire::CreatureName = "Vampire";

string Vampire::getName() { return Vampire::CreatureName; }

Mummy::Mummy(HealthPoints health, AttackPower attackPower)
    : SingleMonster(health, attackPower) {}

const string Mummy::CreatureName = "Mummy";

string Mummy::getName() { return Mummy::CreatureName; }

Zombie *createZombie(HealthPoints health, AttackPower attackPower) {
    return new Zombie(health, attackPower);
}

Vampire *createVampire(HealthPoints health, AttackPower attackPower) {
    return new Vampire(health, attackPower);
}

Mummy *createMummy(HealthPoints health, AttackPower attackPower) {
    return new Mummy(health, attackPower);
}

GroupOfMonsters::GroupOfMonsters(const vector<SingleMonster *> monsters)
    : monsters(monsters.begin(), monsters.end()) {}

GroupOfMonsters::GroupOfMonsters(
    const initializer_list<SingleMonster *> monsters)
    : monsters(monsters.begin(), monsters.end()) {}

string GroupOfMonsters::getName() {
    string ret = "Group{ ";
    for (auto monster : monsters) {
        ret += monster->getName();
        ret += " ,";
    }
    ret.pop_back(); // delete the last comma
    ret += "}";
    return ret;
}

void GroupOfMonsters::takeDamage(AttackPower attackPower) {
    for (auto monster : monsters) {
        monster->takeDamage(attackPower);
    }
}

HealthPoints GroupOfMonsters::getHealth() {
    HealthPoints result{};
    for (auto monster : monsters) {
        result += monster->getHealth();
    }
    return result;
}

AttackPower GroupOfMonsters::getAttackPower() {
    AttackPower result{};
    for (auto monster : monsters) {
        result += monster->getAttackPower();
    }
    return result;

}
