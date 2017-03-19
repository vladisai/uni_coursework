//
//  horror_example.cc
//  horror-movie-2
//
//  Created by Patrycja Wegrzynowicz on 12/10/16.
//  Copyright (c) 2016 Patrycja Wegrzynowicz. All rights reserved.
//

#include "citizen.h"
#include "monster.h"
#include "smalltown.h"

#include <cassert>

using namespace std;

int main() {
    auto mummy = createMummy(90, 1);

    auto groupOfMonsters = createGroupOfMonsters(
        {mummy, mummy, createZombie(20, 1), createVampire(30, 1)});
    assert(groupOfMonsters->getHealth() > 0);
    auto adult = createAdult(100, 21);
    auto smallTown = SmallTown::Builder()
                         .monster(groupOfMonsters)
                         .startTime(3)
                         .maxTime(27)
                         .citizen(createSheriff(100, 50, 20))
                         .citizen(adult)
                         .citizen(adult)
                         .citizen(createTeenager(50, 14))
                         .build();

    smallTown.tick(18);
    smallTown.tick(3);


    auto status = smallTown.getStatus();
    assert(status.getMonsterName() == "GroupOfMonsters");
    assert(status.getMonsterHealth() == 80);
    assert(status.getAliveCitizens() == 3);

    return 0;
}
