#include "citizen.h"
#include "monster.h"
#include "smalltown.h"
#include <iostream>
//using namespace std;

int main() {
    auto s = Sheriff<int>(30, 10, 30);
	auto smallTown = SmallTown<Mummy<float>, int, 0, 23, Sheriff<int>, Adult<int>, Teenager<long>>(
		Mummy<float>(100.0f, 1.0f),
		Sheriff<int>(100, 35, 1),
		Adult<int>(100, 21),
		Teenager<long>(50, 14)
		);
	auto smallTown2 = SmallTown<Mummy<float>, int, 0, 23, Sheriff<int>, Adult<int>, Teenager<long>>(
		Mummy<float>(100.0f, 26.0f),
		Sheriff<int>(100, 35, 1),
		Adult<int>(100, 21),
		Teenager<long>(50, 14)
		);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown2.tick(1);
	smallTown.tick(24);
	smallTown.tick(1);
	smallTown.tick(24);
	smallTown.tick(24);
	auto z = Mummy<int>(10, 10);
	auto status = smallTown.getStatus();
	std::cout << std::get<0>(status) << std::endl;
	/*assert(get<0>(status) != "");
	assert(get<1>(status) == 60);
	assert(get<2>(status) == 3);*/
	/*int z1;
	std::cin >> z1;*/
	return 0;
}
