#include "citizen.h"
#include "monster.h"

#include "bits/stdc++.h"

using namespace std;

int main() {
    Sheriff<int> s(30, 30, 30);
    cout << s.getHealth();
    return 0;
}
