
#pragma once

#include "core/Texture.hpp"

namespace core {

/**
 * @class Distmap
 * Simple class to generate distmap texture from black and
 * white motifs.
 */
class Distmap : public core::Texture {
 public:

  /**
   * Create a distmap texture from its pixels data and the size of the image.
   */
  Distmap(size_t width, size_t height);

  /**
   * Free the memory allocated by the distmap.
   */
  virtual ~Distmap();

  /**
   * Compute the distmap from another texture.
   */
  void generate(Texture const &other);

};

}
