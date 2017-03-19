#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

#include "citizen.h"
#include "monster.h"

namespace FibonacciComputer {
template <typename T, size_t N>
struct fibonacci : std::integral_constant<T, fibonacci<T, N - 1>::value +
                                                 fibonacci<T, N - 2>::value> {};

template <typename T> struct fibonacci<T, 0> : std::integral_constant<T, 1> {};

template <typename T> struct fibonacci<T, 1> : std::integral_constant<T, 1> {};
};

template <typename U, U maximum> class Fibonacci {
    template <size_t... Indices>
    static constexpr std::array<U, sizeof...(Indices)>
    build(std::index_sequence<Indices...>) {
        return {{FibonacciComputer::fibonacci<U, Indices>::value...}};
    }

    template <size_t len> static constexpr std::array<U, len> generateFib() {
        return build(std::make_index_sequence<len>());
    }

    static constexpr int lim = 92;

    using computationT = unsigned long long;

    template <int c = 0>
    static constexpr typename std::enable_if<c >= lim, size_t>::type
    calculateSize() {
        return lim;
    }

    template <int c = 0>
    static constexpr typename std::enable_if<c < lim, size_t>::type
    calculateSize() {
        return (maximum < FibonacciComputer::fibonacci<computationT, c>::value)
                   ? c
                   : calculateSize<c + 1>();
    }

  public:
    static constexpr size_t size = calculateSize();
    static constexpr std::array<U, size> numbers =
        generateFib<calculateSize()>();

    static constexpr bool isFib(int x) {
        return std::binary_search(numbers.begin(), numbers.end(), x);
    }
};

template <typename U, U maximum> constexpr size_t Fibonacci<U, maximum>::size;
template <typename U, U maximum>
constexpr std::array<U, Fibonacci<U, maximum>::size>
    Fibonacci<U, maximum>::numbers;

namespace TupleTool {
template <
    typename Tuple, typename F, int ind = 0,
    int N = std::tuple_size<typename std::remove_reference<Tuple>::type>::value>
typename std::enable_if<ind == N, void>::type applyToAll(Tuple &&t, F f) {}

template <
    typename Tuple, typename F, int ind = 0,
    int N = std::tuple_size<typename std::remove_reference<Tuple>::type>::value>
    typename std::enable_if < ind<N, void>::type applyToAll(Tuple &&t, F f) {
    f(std::get<ind>(t));
    applyToAll<Tuple, F, ind + 1, N>(t, f);
}
}

template <typename Monster, typename U, U t0, U t1, typename... Citizens>
class SmallTown {
    std::tuple<Citizens...> citizens;
    Monster monster;
    U currentTime;
    size_t alive;
    static constexpr size_t citizenNumber = sizeof...(Citizens);
    Fibonacci<U, t1> fibon;

  public:
    SmallTown(Monster monster, Citizens... _citizens)
        : citizens(std::make_tuple(_citizens...)), monster(monster),
          currentTime(t0) {
        static_assert(t0 <= t1, "Start time is bigger than end time");
        static_assert(0 <= t0, "End time is negative number");
        citizens = std::make_tuple(_citizens...);
        alive = citizenNumber;
    }

    std::tuple<std::string, typename Monster::valueType, size_t>
    getStatus() const {
        return make_tuple(std::string(TypeName::get<Monster>),
                          monster.getHealth(), alive);
    }

    void tick(U timeStep) {
        if (fibon.isFib(currentTime)) {
            if (monster.getHealth() == 0 && alive > 0) {
                std::cout << "CITIZENS WON" << std::endl;
            } else if (monster.getHealth() == 0 && alive == 0) {
                std::cout << "DRAW" << std::endl;
            } else if (monster.getHealth() > 0 && alive == 0) {
                std::cout << "MONSTER WON" << std::endl;
            } else {
                int newAlive = 0;
                TupleTool::applyToAll(citizens, [this, &newAlive](auto &c) {
                    if (c.getHealth() > 0)
                        attack(monster, c);
                    newAlive += (c.getHealth() > 0);
                });
                alive = newAlive;
            }
        }
        currentTime += timeStep;
        currentTime %= t1 + 1;
    }
};

#endif // SMALLTOWN_H
