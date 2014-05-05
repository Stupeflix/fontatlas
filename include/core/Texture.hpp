
#pragma once

#include <string>
#include "math/Vector4.hpp"

namespace core {

class Texture {
 public:

  enum ColorType {
    AlphaRate,
    BlackAndWhite,
    Colored
  };

  /**
   * Default constructor.
   */
  Texture();

  /**
   * Create a texture from its size.
   */
  Texture(size_t width, size_t height);

  /**
   * Copy constructor.
   */
  Texture(Texture const &other);

  /**
   * Assignement operator.
   */
  Texture &operator=(Texture const &other);

  /**
   * Free the allocated memory.
   */
  virtual ~Texture();

  /**
   * Return the pixel data of the texture.
   */
  unsigned char *getData() const;

  /**
   * Return the width of the texture.
   */
  std::size_t getWidth() const;

  /**
   * Return the height of the texture.
   */
  std::size_t getHeight() const;

  /**
   * Load the image from the given png file.
   */
  void loadFromPng(std::string const &path);

  /**
   * Save the image to the given file path.
   */
  void saveToPng(std::string const &path,
                 ColorType type = BlackAndWhite);

 protected:

  unsigned char *_data;
  size_t _width;
  size_t _height;

};

}