
#pragma once

#include <cstddef>

namespace math {

template<typename Type>
struct Vector3 {
  union {
    struct { Type x, y, z; };
    struct { Type r, g, b; };
  };
};

typedef Vector3<float> Vector3f;
typedef Vector3<size_t> Vector3s;
typedef Vector3<int> Vector3i;

}