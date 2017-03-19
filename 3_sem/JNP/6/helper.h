#ifndef HELPER_H
#define HELPER_H

using HealthPoints = int;
using Age = int;
using AttackPower = int; // change this, should be general

class _EntityWithHealth {
  public:
    virtual HealthPoints getHealth() const = 0;

    virtual void takeDamage(AttackPower attackPower) = 0;

    bool isAlive() const;

    virtual ~_EntityWithHealth() = default;
};

class _LivingEntity : public virtual _EntityWithHealth {
  public:
    _LivingEntity(HealthPoints health);

    virtual HealthPoints getHealth() const override;

    virtual void takeDamage(AttackPower damage) override;

    virtual ~_LivingEntity() = default;

  private:
    HealthPoints health;
};

class _EntityWithAge {
  public:
    virtual Age getAge() const = 0;

    virtual ~_EntityWithAge() = default;
};

class _AgeingEntity : public virtual _EntityWithAge {
  public:
    _AgeingEntity(Age age);

    virtual Age getAge() const override;

    virtual ~_AgeingEntity() = default;

  private:
    virtual bool checkAge(Age age) const = 0;
    Age age;
};

class _EntityWithAttackPower {
  public:
    virtual AttackPower getAttackPower() const = 0;

    virtual ~_EntityWithAttackPower() = default;
};

class _AttackingEntity : public virtual _EntityWithAttackPower {
  public:
    _AttackingEntity(AttackPower attackPower);

    virtual AttackPower getAttackPower() const override final;

    virtual ~_AttackingEntity() = default;

  private:
    AttackPower attackPower;
};

class _PeacefulEntity : public virtual _EntityWithAttackPower {
  public:
    virtual AttackPower getAttackPower() const override final;

    virtual ~_PeacefulEntity() = default;
};

#endif // HELPER_H
