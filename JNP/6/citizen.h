class Citizen : public _LivingEntity, public _AgeingEntity {
  public:
    Citizen(HealthPoints health, Age age);

  private:
};

class Adult : public Citizen {
  public:
    Adult(HealthPoints health, Age age);
};

class Teenager : public Citizen {
  public:
    Teenager(HealthPoints health, Age age);
};

class Sheriff : public Adult, public _AttackingEntity {
  public:
    Sheriff(HealthPoints health, Age age, AttackPower attackPower);
};
