#include <future>
#include <iostream>
#include <thread>

using namespace std;

void f(const string& s, promise<unsigned int>& len_promise)
{ // len_promise is a placeholder for the result
    // warning: cout is thread-safe, but
    // using cout in multiple threads may result in mixed output
    // see log.h for a solution
    cout << "f() starts for " << s << endl;
    // we simulate some processing here
    this_thread::sleep_for(chrono::seconds(1));
    cout << "f() result is ready for " << s << endl;
    len_promise.set_value(s.length());
    // we simulate cleanup here
    cout << "f() completes for " << s << endl;
}

int main()
{
    unsigned int len1{ 0 }, len2{ 0 };
    promise<unsigned int> len_promise1, len_promise2;             // promise for the result
    future<unsigned int> len_future1 = len_promise1.get_future(); // represents the promised result
    future<unsigned int> len_future2 = len_promise2.get_future();
    const string s1{ "najkrwawsza to tragedia" };
    const string s2{ "gdy krew zalewa widzow" };
    cout << "main() starts" << endl;
    thread t1{ [&s1, &len_promise1] { f(s1, len_promise1); } };
    this_thread::sleep_for(chrono::seconds(1));
    thread t2{ [&s2, &len_promise2] { f(s2, len_promise2); } };
    t1.detach();
    t2.detach();
    len1 = len_future1.get(); // wait until len_promise1.set_value()
    cout << "len1: " << len1 << " correct? " << (len1 == s1.length()) << endl;
    len2 = len_future2.get();
    cout << "len2: " << len2 << " correct? " << (len2 == s2.length()) << endl;
    cout << "main() completes" << endl;
}
