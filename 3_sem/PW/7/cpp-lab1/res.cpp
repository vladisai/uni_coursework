#include <thread>
#include <iostream>

void f(const std::string& s, unsigned int& res) { // res is a placeholder for the result
    res = s.length();
}

int main() {
    unsigned int len1{0}, len2{0};
    const std::string s1{"na szyi zyrafy"};
    const std::string s2{"pchla zaczyna wierzyc w niesmiertelnosc"};
    std::cout << "main() starts" << std::endl;
    std::thread t1{[&s1, &len1]{ f(s1, len1); }}; // lambda has to capture len1 by reference, hence [&len1]
    std::thread t2{[&s2, &len2]{ f(s2, len2); }};
    t1.join();
    t2.join();
    std::cout << "len1: "<<len1<<" correct? "<< (len1==s1.length()) << std::endl;
    std::cout << "len2: "<<len2<<" correct? "<< (len2==s2.length()) << std::endl;
    std::cout << "main() completes" << std::endl;
}
