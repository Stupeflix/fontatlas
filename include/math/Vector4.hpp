
#pragma once

#include <cstddef>

namespace math {

template<typename Type>
struct Vector4 {

  Vector4(Type x, Type y, Type z, Type w) :
    data{x, y, z, w} {
  }

  union {
    Type data[4];
    struct { Type x, y, z, w; };
    struct { Type r, g, b, a; };
    struct { Type _1, _2, width, height; };
  };
};

typedef Vector4<unsigned char> Color;
typedef Vector4<float> Colorf;

typedef Vector4<float> Rectf;
typedef Vector4<size_t> Rect;
typedef Vector4<int> Recti;

typedef Vector4<float> Vector4f;
typedef Vector4<size_t> Vector4s;
typedef Vector4<int> Vector4i;

}