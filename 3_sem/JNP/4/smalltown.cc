#include "smalltown.h"

template<typename M, typename U, U t0, U t1, typename ...C>
SmallTown::SmallTown(M monster, C... citizens) :
    citizens(citizens),
    monster(monster),
    currentTime(t0) {
    static_assert(t0 <= t1);
    static_assert(0 <= t0);
}

namespace TypeName {
    // default implementation
    template<typename T>
    struct Get {
        static const char *operator()() {
            return typeid(T).name();;
        }
    };

    #define ENABLE_TYPENAME(A) template<> struct Get<A> { static const char *operator()() { return #A; }};
    ENABLE_TYPENAME(Vampire)
    ENABLE_TYPENAME(Zombie)
    ENABLE_TYPENAME(Mummy)
}

template<typename M, typename U, U t0, U t1, typename ...C>
std::tuple<std::string, T, size_t>
SmallTown::getStatus() const {
    return make_tuple(TypeName::Get<M>(), monster.getHealth(), citizens.size());
}

template<typename U>
constexpr vector<U> calculateFibs(U maximum);

template<typename U, U maximum>
constexpr bool isFibonacci(U currentTime) {
    static vector<U> fibs(calculateFibs<U>(maximum));
};

template<typename M, typename U, U t0, U t1, typename ...C>
void
SmallTown::tick(U timeStep) {
    if(isFibonnacii<U, t1>(currentTime)) {

    }
    currentTime += timeStep;
    currentTime %= t1;
}
