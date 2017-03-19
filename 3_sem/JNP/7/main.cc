#include <bits/stdc++.h>

#define F first
#define S second
#define pb push_back
#define INF (1 << 30)
#define SQR(a) ((a) * (a))

#include "tree.h"

using namespace std;

const int N = 1111;

int main()
{
    auto two = Tree<int>::createValueNode(2);
    auto one = Tree<int>::createValueNode(1, Tree<int>::createEmptyNode(), two);
    auto four = Tree<int>::createValueNode(4);
    auto five = Tree<int>::createValueNode(5,
                                           Tree<int>::createValueNode(4),
                                           Tree<int>::createEmptyNode());
    auto root = Tree<int>::createValueNode(3, one, five);
    Tree<int> tree(root);
    tree.print();
    tree.print(Tree<int>::preorder);
    tree.print(Tree<int>::postorder);
    tree.filter([](int e){ return e != 3; }).print(Tree<int>::postorder);

    std::cout << "size: " << tree.size() << std::endl;
    std::cout << "height: " << tree.height() << std::endl;
    std::cout << "bst: " << tree.is_bst() << std::endl;

    int x = 2;
    Tree<int> minus_x = tree.map([&](int e)->int{return e - x;});
    Tree<int> lazy_minus_x = tree.lazy_map([&](int e)->int{return e - x;});
    Tree<double> nxt = lazy_minus_x.lazy_map([&](int e)->double{return 2.123 * e;});
    x = 4;
    std::cout << "-2: ";
    minus_x.print();
    std::cout << "-4: ";
    lazy_minus_x.print();
    x = 0;
    nxt.print();

    Tree<int> odd = minus_x.filter([](int e)->bool{return (e & 1) == 1;});
    std::cout << "odd: ";
    odd.print();

    int sum1 = tree.accumulate(std::plus<int>(), 0, Tree<int>::inorder);
    std::cout << "sum: " << sum1 << std::endl;

    int sum2 = tree.fold([](int val, int l, int r)->int{return val + l + r;}, 0);
    std::cout << "sum: " << sum2 << std::endl;
    return 0;
}
