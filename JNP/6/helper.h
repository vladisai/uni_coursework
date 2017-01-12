#ifndef HELPER_H
#define HELPER_H

using HealthPoints = int;
using Age = int;
using AttackPower = int; // change this, should be general

class _EntityWithHealth {
  public:
    virtual HealthPoints getHealth() = 0;
    virtual void takeDamage(AttackPower attackPower) = 0;
};

class _LivingEntity : public virtual _EntityWithHealth {
  public:
    _LivingEntity(HealthPoints health = 0);

    virtual HealthPoints getHealth() override;

    virtual void takeDamage(AttackPower damage) override;

  private:
    HealthPoints health;
};

class _EntityWithAge {
  public:
    virtual Age getAge() = 0;
};

class _AgeingEntity : public virtual _EntityWithAge {
  public:
    _AgeingEntity(Age age = 0);

    virtual Age getAge() override;

  private:
    virtual bool checkAge(Age age) = 0;
    Age age;
};

class _EntityWithAttackPower {
  public:
    virtual AttackPower getAttackPower() = 0;
};

class _AttackingEntity : public virtual _EntityWithAttackPower {
  public:
    _AttackingEntity(AttackPower attackPower = 0);
    virtual AttackPower getAttackPower() override final;

  private:
    AttackPower attackPower;
};

class _PeacefulEntity : public virtual _EntityWithAttackPower {
    public:
        virtual AttackPower getAttackPower() override final;
};

#endif // HELPER_H
