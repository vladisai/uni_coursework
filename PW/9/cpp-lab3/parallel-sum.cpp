// code adopted from http://en.cppreference.com/w/cpp/thread/async
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
 
template <typename RAIter>
int parallel_sum(RAIter beg, RAIter end)
{
    auto len = end - beg;
    if (len < 1000) // sum short vectors sequentially
        return std::accumulate(beg, end, 0);
 
    RAIter mid = beg + len/2;
    std::future<int> handle = std::async(parallel_sum<RAIter>, mid, end);  // a new task: sum the second half
    int sum = parallel_sum(beg, mid); // sum the first half in this task
    return sum + handle.get(); // synchronization
}
 
int main()
{
    std::vector<int> v(10'000'000, 1);
    std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';
}
