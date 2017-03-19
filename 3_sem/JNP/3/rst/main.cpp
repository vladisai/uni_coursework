#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif

#include "geometry.h"

#define F first
#define S second
#define pb push_back
#define INF (1 << 30)
#define SQR(a) ((a) * (a))

using namespace std;

const int N = 1111;

void testPositionInit()
{
	Position p(10, 10);
	assert(p.x() == 10);
	assert(p.y() == 10);

	p = Position();
	assert(p.x() == 0);
	assert(p.y() == 0);
}

void testPositionOrigin()
{
	assert(Position::origin().x() == 0);
	assert(Position::origin().y() == 0);
}

void testPositionAddVector()
{
	// += (vector) test for point
	Position p(5, 5);
	Vector v(3, 4);
	p += v;
	assert(p.x() == 3);
	assert(p.y() == 4);
}

void testPositionReflection()
{
	Position p(3, 4);
	p = p.reflection();
	assert(p.x() == 4);
	assert(p.y() == 3);
}

void testVectorInit()
{
	Vector v(3, 4);
	assert(v.x() == 3);
	assert(v.y() == 4);
}

void testVectorReflection()
{
	Vector v(3, 4);
	v = v.reflection();
	assert(v.x() == 4);
	assert(v.y() == 3);
}

void testPositionEqualsOperator()
{
	Position p1(1, 1);
	Position p2(2, 2);
	Position p3(2, 2);
	assert(!(p1 == p2));
	assert(p2 == p3);
}

void testVectorEqualsOperator()
{
	Vector v1(1, 1);
	Vector v2(2, 2);
	Vector v3(2, 2);
	assert(!(v1 == v2));
	assert(v2 == v3);
}

void testVectorAddVector()
{
	Vector v1(1, 1);
	Vector v2(2, 2);
	v2 += v1;
	assert(v2.x() == 3);
	assert(v2.y() == 3);
}

void testVectorPlusVector() {
	Vector v1(2, 3);
	Vector v2(10, 20);
	Vector res = v1 + v2;
	assert(res.x() == 12);
	assert(res.y() == 23);
}

void testVectorPlusVectorRval() {
	Vector v1(2, 3);
	Vector res = v1 + Vector(2, 3);
	assert(res.x() == 4);
	assert(res.y() == 6);
}

void testVectorRvalPlusVectorRval() {
	Vector res = Vector(2, 3) + Vector(2, 3);
	assert(res.x() == 4);
	assert(res.y() == 6);
}

void testVectorRvalPlusVector() {
	Vector v1(2, 3);
	Vector res = Vector(2, 3) + v1;
	assert(res.x() == 4);
	assert(res.y() == 6);
}

void testPositionPlusVector() {
	Vector v(2, 3);
	Position p(2, 3);
	Position res = v + p;
	assert(res.x() == 4);
	assert(res.y() == 6);
}

void testPositionPlusVectorRval() {
	Vector &&v = Vector(2, 3);
	Position p(2, 3);
	Position res = v + p;
	assert(res.x() == 4);
	assert(res.y() == 6);
}

void testPositionRvalPlusVector() {
	Vector v(2, 3);
	Position &&p = Position(2, 3);
	Position res = v + p;
	assert(res.x() == 4);
	assert(res.y() == 6);
}

void testPositionRvalPlusVectorRval() {
	Vector &&v = Vector(2, 3);
	Position &&p = Position(2, 3);
	Position res = v + p;
	assert(res.x() == 4);
	assert(res.y() == 6);
}

int main()
{
	testPositionInit();
	testPositionOrigin();
	testPositionAddVector();
	testPositionReflection();
	testPositionEqualsOperator();

	testVectorInit();
	testVectorReflection();
	testVectorAddVector();
	testVectorEqualsOperator();

	testPositionPlusVector();
	testVectorPlusVector();

	testVectorPlusVector();
	testVectorPlusVectorRval();
	testVectorRvalPlusVectorRval();
	testVectorRvalPlusVector();

	testPositionPlusVector();
	testPositionRvalPlusVector();
	testPositionPlusVectorRval();
	testPositionRvalPlusVectorRval();

	return 0;
}
