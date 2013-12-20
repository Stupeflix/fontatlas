
#pragma once

#include <vector>
#include "math/Vector3.hpp"
#include "math/Vector4.hpp"
#include "core/Texture.hpp"

namespace core {

/**
 * A texture atlas is used to pack several small regions into a single texture.
 */
class Atlas : public core::Texture {
 public:

  /**
   * Create a texture atlas from its width and height.
   */
  Atlas(size_t width, size_t height);

  /**
   * Delete the texture atlas and free all allocated memory.
   */
  virtual ~Atlas();

  /**
   * Clear the atlas.
   */
  void clear();

  /**
   * Place the given region into the atlas.
   */
  void setRegion(size_t x,
                 size_t y,
                 size_t width,
                 size_t height,
                 unsigned char *data,
                 size_t stride);

  /**
   * Determinate a region into the atlas for the given size.
   */
  math::Vector4s getRegion(size_t width, size_t height);

 private:

  int _findBestIndex(math::Vector4s &region,
                     size_t width,
                     size_t height);

  /**
   * Check if we can fit the given region on the height.
   */
  int _checkFit(size_t index,
                size_t width,
                size_t height) const;

  /**
   * Merge nodes if it is possible.
   */
  void _merge();

  std::vector<math::Vector3i> _nodes;
  size_t _used;
  unsigned int _id;

  /**
   * Atlas data
   */
};

}  // namespace core
