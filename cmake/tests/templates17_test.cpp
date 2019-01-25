#include <functional>

template <typename R, typename... Args>
void store(std::function<R(Args...)>) {}

template <typename T>
int foo(int a) { return a; }

int main() {
    store<int, int>(&foo<char>);
    return 0;
}