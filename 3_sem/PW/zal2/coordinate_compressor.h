#ifndef COORDINATE_COMPRESSOR_H
#define COORDINATE_COMPRESSOR_H

#include <bits/stdc++.h>

using namespace std;

class CoordinateCompressor {
    public:
        int compress(int x) {
            if (ids.find(x) == ids.end()) {
                ids[x] = nextId;
                nextId++;
            }
            return ids[x];
        }

        int size() {
            return nextId;
        }

    private:
        int nextId = 0;
        unordered_map<int, int> ids;
};

#endif // COORDINATE_COMPRESSOR_H

