#include <cstddef>

template <size_t N, const char (*Input)[N]>
struct X;

struct A
{
  static constexpr const char _literal[] = "concat_";
  using Y = X<sizeof(_literal), &_literal>;
};

int main() {
    return 0;
}