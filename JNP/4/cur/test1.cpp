#include "citizen.h"
#include "monster.h"
#include "smalltown.h"

using namespace std;

int main() {
    typedef float type;

    //test0
    auto smallTown = SmallTown<Mummy<float>, int, 1, 23, Sheriff<int>, Adult<int>, Teenager<long>>(
            Mummy<float>(100.0f, 1.5f),
                    Sheriff<int>(100, 35, 20),
                    Adult<int>(100, 21),
                    Teenager<long>(50, 14)
    );

    smallTown.tick(1);
    smallTown.tick(22);

    auto status = smallTown.getStatus();
    assert(get<0>(status) != "");
    assert(get<1>(status) == 60);
    assert(get<2>(status) == 3);

    cout<<"TEST 0: CORRECT"<<endl;

    //test1 - citizen.h

    Sheriff<type> s1(100, 50, 30);
    Adult<type> a1(129, 45);
    Teenager<type> t1(115, 11);
    assert(s1.getHealth() == 100 && s1.getAttackPower() == 30 && s1.getAge() == 50);
    assert(a1.getHealth() == 129 && a1.getAge() == 45);
    assert(t1.getHealth() == 115 && t1.getAge() == 11);

    s1.takeDamage(50);
    a1.takeDamage(30);
    t1.takeDamage(25);

    assert(s1.getHealth() == 50 && s1.getAttackPower() == 30 && s1.getAge() == 50);
    assert(a1.getHealth() == 99 && a1.getAge() == 45);
    assert(t1.getHealth() == 90 && t1.getAge() == 11);

    s1.takeDamage(5000);
    a1.takeDamage(3000);
    t1.takeDamage(2500);

    assert(s1.getHealth() == 0 && s1.getAttackPower() == 30 && s1.getAge() == 50);
    assert(a1.getHealth() == 0 && a1.getAge() == 45);
    assert(t1.getHealth() == 0 && t1.getAge() == 11);

    cout<<"TEST 1: CORRECT"<<endl;

    //test2 - monster.h
    Mummy<type> m2(100, 50);
    Vampire <type> v2(200, 60);
    Zombie <type> z2(300, 70);
    assert(m2.getHealth() == 100 && m2.getAttackPower() == 50);
    assert(v2.getHealth() == 200 && v2.getAttackPower() == 60);
    assert(z2.getHealth() == 300 && z2.getAttackPower() == 70);

    m2.takeDamage(50);
    v2.takeDamage(50);
    z2.takeDamage(50);

    assert(m2.getHealth() == 50 && m2.getAttackPower() == 50);
    assert(v2.getHealth() == 150 && v2.getAttackPower() == 60);
    assert(z2.getHealth() == 250 && z2.getAttackPower() == 70);

    m2.takeDamage(5000);
    v2.takeDamage(50000);
    z2.takeDamage(50000);

    assert(m2.getHealth() == 0 && m2.getAttackPower() == 50);
    assert(v2.getHealth() == 0 && v2.getAttackPower() == 60);
    assert(z2.getHealth() == 0 && z2.getAttackPower() == 70);

    cout<<"TEST 2: CORRECT"<<endl;

    //test3 -attack
    Mummy<type> m3(1000, 50);
    Sheriff<type> s3(1000, 20, 100);
    Adult<type> a3(2000, 20);

    attack(m3, s3);
    attack(m3, s3);
    attack(m3, s3);

    assert(s3.getHealth() == 850 && m3.getHealth() == 700);

    attack(m3, a3);
    attack(m3, a3);
    attack(m3, a3);

    assert(a3.getHealth() == 1850 && m3.getHealth() == 700);


    cout<<"TEST 3: CORRECT"<<endl;


    //test4 - monster won

    auto smallTown4 = SmallTown<Mummy<float>, int, 1, 5, Sheriff<int>, Sheriff<int>, Sheriff<int>, Adult<int>, Teenager<long>>(
            Mummy<float>(1000, 50),
            Sheriff<int>(100, 35, 25),
            Sheriff<int>(100, 35, 25),
            Sheriff<int>(100, 35, 25),
            Adult<int>(250, 21),
            Teenager<long>(60, 14)
    );

    smallTown4.tick(0); // dla 1
    assert(get<0>(smallTown4.getStatus()) == "Mummy");
    assert(get<1>(smallTown4.getStatus()) == 925);
    assert(get<2>(smallTown4.getStatus()) == 5);
    smallTown4.tick(1); // dla 1
    assert(get<0>(smallTown4.getStatus()) == "Mummy");
    assert(get<1>(smallTown4.getStatus()) == 850);
    assert(get<2>(smallTown4.getStatus()) == 1);
    smallTown4.tick(3); // dla 2
    assert(get<0>(smallTown4.getStatus()) == "Mummy");
    assert(get<1>(smallTown4.getStatus()) == 850);
    assert(get<2>(smallTown4.getStatus()) == 1);
    smallTown4.tick(5); // dla 5
    assert(get<0>(smallTown4.getStatus()) == "Mummy");
    assert(get<1>(smallTown4.getStatus()) == 850);
    assert(get<2>(smallTown4.getStatus()) == 1);
    smallTown4.tick(1); // dla 4
    assert(get<0>(smallTown4.getStatus()) == "Mummy");
    assert(get<1>(smallTown4.getStatus()) == 850);
    assert(get<2>(smallTown4.getStatus()) == 1);
    smallTown4.tick(1); // dla 5
    assert(get<0>(smallTown4.getStatus()) == "Mummy");
    assert(get<1>(smallTown4.getStatus()) == 850);
    assert(get<2>(smallTown4.getStatus()) == 0);
    smallTown4.tick(1); // dla 0

    cout<<"TEST 4: CORRECT"<<endl;
    //test5 - citizens won
    auto smallTown5 = SmallTown<Zombie<float>, int, 1, 5, Sheriff<int>, Sheriff<int>, Sheriff<int>, Adult<int>, Teenager<long>>(
            Zombie<float>(1000, 50),
            Sheriff<int>(100, 35, 300),
            Sheriff<int>(100, 35, 300),
            Sheriff<int>(100, 35, 300),
            Adult<int>(250, 21),
            Teenager<long>(50, 14)
    );
    smallTown5.tick(1); // dla 1
    assert(get<0>(smallTown5.getStatus()) == "Zombie");
    assert(get<1>(smallTown5.getStatus()) == 100);
    assert(get<2>(smallTown5.getStatus()) == 4);
    smallTown5.tick(1); // dla 2
    assert(get<0>(smallTown5.getStatus()) == "Zombie");
    assert(get<1>(smallTown5.getStatus()) == 0);
    assert(get<2>(smallTown5.getStatus()) == 1);
    smallTown5.tick(1); // dla 3

    cout<<"TEST 5: CORRECT"<<endl;
    //test6 - draw
    auto smallTown6 = SmallTown<Vampire <float>, int, 1, 5, Sheriff<int>, Sheriff<int>, Sheriff<int>, Adult<int>, Teenager<long>>(
            Vampire<float>(1000, 50),
            Sheriff<int>(100, 35, 300),
            Sheriff<int>(50, 35, 300),
            Sheriff<int>(50, 35, 300),
            Adult<int>(100, 21),
            Teenager<long>(50, 14)
    );
    smallTown6.tick(1); // dla 1
    assert(get<0>(smallTown6.getStatus()) == "Vampire");
    assert(get<1>(smallTown6.getStatus()) == 100);
    assert(get<2>(smallTown6.getStatus()) == 2);
    smallTown6.tick(1); // dla 2
    assert(get<0>(smallTown6.getStatus()) == "Vampire");
    assert(get<1>(smallTown6.getStatus()) == 0);
    assert(get<2>(smallTown6.getStatus()) == 0);
    smallTown6.tick(1); // dla 3

    cout<<"TEST 6: CORRECT"<<endl;
    return 0;
}
