using HealthPoints = int;
using Age = int;
using AttackPower = int; // change this, should be general

class _LivingEntity {
  public:
    _LivingEntity(HealthPoints health);

    HealthPoints getHealth();

    void takeDamage(AttackPower damage);

  private:
    HealthPoints health;
};

class _AgeingEntity {
  public:
    _AgeingEntity(Age age);

    Age getAge();

  private:
    Age age;
};

class _AttackingEntity {
  public:
    _AttackingEntity(AttackPower attackPower);
    AttackPower getAttackPower();

  private:
    AttackPower attackPower;
};
