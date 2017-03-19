#include "citizen.h"

#include <cassert>
#include <stdexcept>

using std::make_shared;
using std::exception;
using std::runtime_error;
using std::string;
using std::to_string;

class IncorrectAgeException : public runtime_error {
  public:
    IncorrectAgeException(const string &message) : runtime_error(message) {}
};

Citizen::Citizen(HealthPoints health, Age age)
    : _LivingEntity(health), _AgeingEntity(age) {}

Adult::Adult(HealthPoints health, Age age) : Citizen(health, age) {
    if (!checkAge(age))
        throw IncorrectAgeException(
            "Got " + to_string(age) +
            ", expected value between 18 and 100 for Adult.");
}

bool Adult::checkAge(Age age) const {
    if (age < 18 || age > 100) {
        return false;
    } else {
        return true;
    }
}

Teenager::Teenager(HealthPoints health, Age age) : Citizen(health, age) {
    if (!checkAge(age))
        throw IncorrectAgeException(
            "Got " + to_string(age) +
            ", expected value between 11 and 18 for Teenager.");
}

bool Teenager::checkAge(Age age) const {
    if (age < 11 || age > 18) {
        return false;
    } else {
        return true;
    }
}

Sheriff::Sheriff(HealthPoints health, Age age, AttackPower attackPower)
    : Citizen(health, age), _AttackingEntity(attackPower) {
    if (!checkAge(age))
        throw IncorrectAgeException(
            "Got " + to_string(age) +
            ", expected value between 18 and 100 for Sheriff.");
}

bool Sheriff::checkAge(Age age) const {
    if (age < 18 || age > 100) {
        return false;
    } else {
        return true;
    }
}

shared_ptr<Adult> createAdult(HealthPoints health, Age age) {
    return make_shared<Adult>(health, age);
}

shared_ptr<Teenager> createTeenager(HealthPoints health, Age age) {
    return make_shared<Teenager>(health, age);
}

shared_ptr<Sheriff> createSheriff(HealthPoints health, Age age,
                                  AttackPower attackPower) {
    return make_shared<Sheriff>(health, age, attackPower);
}
