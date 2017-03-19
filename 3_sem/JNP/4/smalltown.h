#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <cassert>
#include <list>
#include <string>
#include <tuple>

#include "citizen.h"
#include "monster.h"

template<typename U, U maximum>
class Fibonacci {
    static constexpr U* calculateFib();
    static constexpr size_t calculateSize();
    public:
        static constexpr U numbers[] = calculateFib();
        static constexpr size_t size = calculateSize();
};

template<typename U, U maximum>
bool isFib(U currentTime) {
    bool result = false;
    size_t start = 0;
    size_t end = Fibonacci<U, maximum>::size;
    while(start != end) {
        size_t cur = (start + end)/2;
        if(currentTime < Fibonacci<U, maximum>::numbers[cur]) {
            end = cur - 1;
        } else if(currentTime > Fibonacci<U, maximum>::numbers[cur]) {
            start = cur + 1;
        } else {
            start = end = cur;
            result = true;
        }
    }
    return result;
};

template<
        template<typename, int> class Monster, typename T, int id,
        typename U, U t0, U t1,
        typename ...Citizens
>
class SmallTown {
    std::tuple<Citizens... > citizens;
    Monster<T, id> monster;
    U currentTime;
    size_t alive;
    static constexpr size_t citizenNumber = sizeof...(Citizens);

    public:
        SmallTown(Monster<T, id> monster, Citizens... _citizens) : monster(monster), currentTime(t0) {
            static_assert(t0 <= t1, "Start time is bigger than end time");
            static_assert(0 <= t0, "End time is negative number");
            citizens = std::make_tuple(_citizens...);
            alive = citizenNumber;
        }

        std::tuple<std::string, T, size_t> getStatus() const {
            return make_tuple(std::string(TypeName::get<Monster<T, id>>), monster.getHealth(), alive);
        }

        void tick(U timeStep) {
            if (isFib<U, t1>(currentTime)) {
                for (size_t i = 0; i < citizenNumber; i++) {
                    attack(monster, std::get<i>(citizens));
                }
                currentTime += timeStep;
                currentTime %= t1;
            }
        }
};

#endif //SMALLTOWN_H
