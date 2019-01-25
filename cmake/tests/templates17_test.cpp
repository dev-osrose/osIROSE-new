#include <functional>

template <typename R>
struct Storage {
    template <typename... Args>
    static void store(std::function<R(Args...)>) {}
};

template <typename T>
int foo(int a) { return a; }

int main() {
    Storage<int>::store<int>(&foo<char>);
}