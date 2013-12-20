
#include <cstring>
#include <climits>
#include <stdexcept>
#include "core/Atlas.hpp"

namespace core {

Atlas::Atlas(size_t width, size_t height) :
  Texture(width, height),
  _used(0) {
  math::Vector3i node = {1, 1, static_cast<int>(width) - 2};
  _nodes.push_back(node);
}

Atlas::~Atlas() {}

void Atlas::clear() {
  _nodes.clear();
  _used = 0;

  math::Vector3i node;
  node.x = 1;
  node.y = 1;
  node.z = _width - 2;
  _nodes.push_back(node);
  std::memset(_data, 0, _width * _height * 4);
}

void Atlas::setRegion(size_t x,
                      size_t y,
                      size_t width,
                      size_t height,
                      unsigned char *data,
                      size_t stride) {
  if (x <= 0 || y <= 0 ||
      x >= _width - 1 ||
      x + width > _width - 1 ||
      y >= _height - 1 ||
      y + height > _height - 1)
    throw std::out_of_range("Cannot add the given region into this atlas.");
  for (size_t i = 0; i < height; ++i)
    std::memcpy(_data + ((y + i) * _width + x), data + (i * stride), width);
}

math::Vector4s Atlas::getRegion(size_t width, size_t height) {

  /* Find best index */
  math::Vector4s region(0, 0, width, height);
  int best_index = _findBestIndex(region, width, height);
  if (best_index == -1)
    return region;

  /* Add a node */
  math::Vector3i newNode;
  newNode.x = region.x;
  newNode.y = region.y + height;
  newNode.z = width;
  _nodes.insert(_nodes.begin() + best_index, newNode);

  /* Reorders the nodes */
  for(size_t i = best_index + 1; i < _nodes.size(); ++i) {
    math::Vector3i &node = _nodes[i];
    math::Vector3i &prev = _nodes[i - 1];
    if (node.x < prev.x + prev.z) {
      int k = prev.x + prev.z - node.x;
      node.x += k;
      node.z -= k;
      if (node.z <= 0) {
        _nodes.erase(_nodes.begin() + i);
        --i;
      }
      else
        break;
    }
    else
      break;
  }
  _merge();
  _used += width * height;
  return region;
}

int Atlas::_findBestIndex(math::Vector4s &region,
                          size_t width,
                          size_t height) {
  math::Vector3i node;
  int best_index = -1;
  int best_width = INT_MAX;
  int best_height = INT_MAX;
  region.x = -1;
  region.y = -1;
  region.width = 0;
  region.height = 0;
  for (size_t i = 0; i < _nodes.size(); ++i) {
    int y = _checkFit( i, width, height );
    if (y >= 0) {
      node = _nodes[i];
      if (y + height < best_height ||
         (y + height == best_height && node.z < best_width)) {
        best_height = y + height;
        best_index = i;
        best_width = node.z;
        region.x = node.x;
        region.y = y;
      }
    }
  }
  return best_index;
}

int Atlas::_checkFit(size_t index,
                     size_t width,
                     size_t height) const {
  math::Vector3i node;
  int x, y, width_left;
  size_t i;
  node = _nodes[index];
  x = node.x;
  y = node.y;
  width_left = width;
  i = index;
  if (x + width > _width - 1)
    return -1;
  y = node.y;
  while (width_left > 0) {
    node = _nodes[i];
    if (node.y > y)
      y = node.y;
    if (y + height > _height - 1)
      return -1;
    width_left -= node.z;
    ++i;
  }
  return y;
}

void Atlas::_merge() {
  for(size_t i = 0; i < _nodes.size() - 1; ++i) {
    math::Vector3i &node = _nodes[i];
    math::Vector3i &next = _nodes[i + 1];
    if (node.y == next.y) {
      node.z += next.z;
      _nodes.erase(_nodes.begin() + i + 1);
      --i;
    }
  }
}

}  // namespace core