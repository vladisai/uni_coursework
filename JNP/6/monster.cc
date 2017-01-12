#include "monster.h"

using std::make_shared;

void Monster::attack(shared_ptr<Citizen> citizen) {
    citizen->takeDamage(this->getAttackPower());
    this->takeDamage(citizen->getAttackPower());
}

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

shared_ptr<GroupOfMonsters>
createGroupOfMonsters(const initializer_list<shared_ptr<SingleMonster>> &monsters) {
    return make_shared<GroupOfMonsters>(monsters);
}

GroupOfMonsters::GroupOfMonsters(const vector<shared_ptr<SingleMonster>> &monsters)
    : monsters(monsters.begin(), monsters.end()) {}

GroupOfMonsters::GroupOfMonsters(
    const initializer_list<shared_ptr<SingleMonster>> &monsters)
    : monsters(monsters.begin(), monsters.end()) {}

const string GroupOfMonsters::CreatureName = "GroupOfMonsters";

string GroupOfMonsters::getName() {
    return GroupOfMonsters::CreatureName;
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
