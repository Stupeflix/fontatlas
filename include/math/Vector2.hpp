
#pragma once

#include <cstddef>

namespace math {

template<typename Type>
struct Vector2 {
  union {
    struct { Type x, y; };
    struct { Type s, t; };
    struct { Type u, v; };
  };
};

typedef Vector2<float> Vector2f;
typedef Vector2<size_t> Vector2s;
typedef Vector2<int> Vector2i;

}