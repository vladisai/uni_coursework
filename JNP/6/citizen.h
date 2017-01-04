class _LivingEntity {
  public:
    using HealthPoints = int;
    using AttackPower = int; // change this, should be general

    _LivingEntity(HealthPoints health) : health(health) {}

    HealthPoints getHealth() { return health; }

    void takeDamage(AttackPower damage) { health -= damage; }

  private:
    HealthPoints health;
};

class _AgeingEntity {
  public:
    using Age = int;

    _AgeingEntity(Age age) : age(age) {}

    Age getAge() { return age; }

  private:
    Age age;
};

class _AttackingEntity {
  public:
    using AttackPower = int; // change, should be general
    _AttackingEntity(AttackPower attackPower) : attackPower(attackPower) {}
    AttackPower getAttackPower() { return attackPower; }

  private:
    AttackPower attackPower;
};

class Citizen : public _LivingEntity, public _AgeingEntity {
  public:
    Citizen(HealthPoints health, Age age)
        : _LivingEntity(health), _AgeingEntity(age) {}

  private:
};

class Adult : public Citizen {

};

class Teenager: public Citizen {

};

class Sheriff : public Citizen {

};
