//
//  horror_example.cc
//  horror-movie-2
//
//  Created by Patrycja Wegrzynowicz on 12/10/16.
//  Copyright (c) 2016 Patrycja Wegrzynowicz. All rights reserved.
//

#include <bits/stdc++.h>

#include "citizen.h"
#include "monster.h"
#include "smalltown.h"

using namespace std;

int main() {
    shared_ptr<Adult> s = make_shared<Adult>(50, 50);
    shared_ptr<Mummy> m = createMummy(50, 50);
    cout << s->getHealth() << " " << s->getAttackPower() << endl;
    cout << m->getHealth() << " " << m->getAttackPower() << endl;

    m->attack(s);

    cout << s->getHealth() << endl;
    cout << m->getHealth() << endl;
    return 0;
}
