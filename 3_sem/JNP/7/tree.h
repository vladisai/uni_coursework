#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>

namespace __Utility {
template <typename R1, typename R2>
std::function<R1()> compose(std::function<R1(R2)> &&f1,
                            std::function<R2()> &&f2) {
    return [f1, f2]() { return f1(f2()); };
}
}

template <typename T> class Tree {
  public:
    struct Node {
        Node(T value)
            : value(value), isDeferred(false), left(createEmptyNode()),
              right(createEmptyNode()) {}

        Node(T value, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
            : value(value), isDeferred(false), left(left), right(right) {}

        template <typename Func>
        Node(Func func)
            : value(), isDeferred(true), func(func), left(createEmptyNode()),
              right(createEmptyNode()) {}

        template <typename Func>
        Node(Func func, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
            : value(), isDeferred(true), func(func), left(left), right(right) {}

        T getValue() const {
            if (isDeferred) {
                isDeferred = false;
                value = func();
            }
            return value;
        }

        std::shared_ptr<Node> getLeftSubtree() const { return left; }

        std::shared_ptr<Node> getRightSubtree() const { return right; }

        void setRightSubtree(std::shared_ptr<Node> right) {
            this->right = right;
        }

        std::function<T()> getFunction() const {
            if (isDeferred) {
                return func;
            } else {
                int x = value;
                return [x]() { return x; };
            }
        }

      private:
        mutable T value;
        mutable bool isDeferred;
        std::function<T()> func;
        std::shared_ptr<Node> left, right;
    };

    template <typename Func>
    static std::shared_ptr<Node> createLazyNode(Func func,
                                                std::shared_ptr<Node> left,
                                                std::shared_ptr<Node> right) {
        return std::make_shared<Node>(func, left, right);
    }

    template <typename Operation, typename R>
    R nodeFold(std::shared_ptr<Node> node, Operation op, R init) const {
        if (node == nullptr) {
            return init;
        } else {
            auto resultLeft = nodeFold(node->getLeftSubtree(), op, init);
            auto resultRight = nodeFold(node->getRightSubtree(), op, init);
            return op(node, resultLeft, resultRight);
        }
    }

    static std::list<T> inorder(T val, std::list<T> l, std::list<T> r) {
        l.push_back(val);
        l.splice(l.end(), r);
        return l;
    };

    static std::list<T> preorder(T val, std::list<T> l, std::list<T> r) {
        l.push_front(val);
        l.splice(l.end(), r);
        return l;
    };

    static std::list<T> postorder(T val, std::list<T> l, std::list<T> r) {
        l.splice(l.end(), r);
        l.push_back(val);
        return l;
    };

    Tree() { root = createEmptyNode(); }

    Tree(std::shared_ptr<Node> root) { this->root = root; }

    Tree(const Tree &t) {
        auto op = [](T t, std::shared_ptr<Node> l, std::shared_ptr<Node> r) {
            return createValueNode(t, l, r);
        };
        root = t.fold(op, createEmptyNode());
    }

    template <typename Operation, typename R>
    R fold(Operation op, R init) const {
        return fold(root, op, init);
    }

    template <typename Predicate> Tree<T> filter(Predicate p) const {
        auto op = [&p](T val, std::shared_ptr<Node> left,
                       std::shared_ptr<Node> right) {
            if (p(val)) {
                return createValueNode(val, left, right);
            } else {
                return join(left, right);
            }
        };
        return Tree<T>(fold(this->root, op, createEmptyNode()));
    }

    void print() const { print(inorder); }

    template <typename Order> void print(Order order) const {
        auto r = fold(this->root, order, std::list<T>());
        for (auto e : r) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }

    int size() const {
        auto op = [](T val, int l, int r) { return l + r + 1; };
        return fold(this->root, op, 0);
    }

    int height() const {
        auto op = [](T val, int l, int r) -> int { return std::max(l, r) + 1; };
        return fold(this->root, op, 0);
    }

    template <typename ApplyOperation, typename Order>
    void apply(ApplyOperation op, Order order) const {
        auto r = fold(this->root, order, std::list<T>());
        for (auto e : r) {
            op(e);
        }
    }

    template <typename AccumulateOpeartion, typename R, typename Order>
    R accumulate(AccumulateOpeartion op, R init, Order order) const {
        auto r = fold(this->root, order, std::list<T>());
        auto result = init;
        for (auto e : r) {
            result = op(result, e);
        }
        return result;
    }

    template <typename Transformer>
    auto map(Transformer t) const -> Tree<decltype(t(T()))> {
        using R = decltype(t(T()));
        auto op = [&t](T val, std::shared_ptr<typename Tree<R>::Node> l,
                       std::shared_ptr<typename Tree<R>::Node> r) {
            return Tree<R>::createValueNode(t(val), l, r);
        };
        auto ret = fold(this->root, op, Tree<R>::createEmptyNode());
        return Tree<R>(ret);
    }

    template <typename Transformer>
    auto lazy_map(Transformer t) const -> Tree<decltype(t(T()))> {
        using R = decltype(t(T()));
        auto op = [&t](std::shared_ptr<Node> x,
                       std::shared_ptr<typename Tree<R>::Node> l,
                       std::shared_ptr<typename Tree<R>::Node> r) {
            return Tree<R>::createLazyNode(
                __Utility::compose<R, T>(t, x->getFunction()), l, r);
        };
        auto ret = nodeFold(this->root, op, Tree<R>::createEmptyNode());
        return Tree<R>(ret);
    }

    bool is_bst() const {
        auto op = +[](T val, std::tuple<int, int, int> l,
                      std::tuple<int, int, int> r) {
            if (std::get<0>(l) == 0 || std::get<0>(r) == 0) {
                return std::make_tuple(0, 0, 0);
            } else if (std::get<0>(l) == 1 && val <= std::get<2>(l)) {
                return std::make_tuple(0, 0, 0);
            } else if (std::get<0>(r) == 1 && val >= std::get<1>(r)) {
                return std::make_tuple(0, 0, 0);
            } else if (std::get<0>(r) == -1 && std::get<0>(l) == -1) {
                return std::make_tuple(1, val, val);
            } else if (std::get<0>(r) == -1) {
                return std::make_tuple(1, std::get<2>(l), val);
            } else if (std::get<0>(l) == -1) {
                return std::make_tuple(1, val, std::get<2>(r));
            } else {
                return std::make_tuple(1, std::get<1>(l), std::get<2>(r));
            }
        };
        auto res = fold(this->root, op, std::make_tuple(-1, T(), T()));
        return std::get<0>(res) != 0;
    }

    static std::shared_ptr<Node> join(std::shared_ptr<Node> left,
                                      std::shared_ptr<Node> right) {
        auto ret = left;
        if (left == nullptr)
            return right;
        while (left->getRightSubtree() != nullptr) {
            left = left->getRightSubtree();
        }
        left->setRightSubtree(right);
        return ret;
    }

    static std::shared_ptr<Node> createEmptyNode() { return nullptr; }

    static std::shared_ptr<Node> createValueNode(T value) {
        return std::make_shared<Node>(value);
    }

    static std::shared_ptr<Node> createValueNode(T value,
                                                 std::shared_ptr<Node> left,
                                                 std::shared_ptr<Node> right) {
        return std::make_shared<Node>(value, left, right);
    }

    std::shared_ptr<Node> root;

    template <typename Operation, typename R>
    R fold(std::shared_ptr<Node> node, Operation op, R init) const {
        if (node == nullptr) {
            return init;
        } else {
            auto resultLeft = fold(node->getLeftSubtree(), op, init);
            auto resultRight = fold(node->getRightSubtree(), op, init);
            return op(node->getValue(), resultLeft, resultRight);
        }
    }

  private:
};

#endif
