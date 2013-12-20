
#pragma once

#include <string>
#include "ft/Library.hpp"
#include "ft/Glyph.hpp"
#include "math/Vector2.hpp"

namespace ft {

/**
 * FreeType font face wrapper. 
 */
class Face {
 public:

  /**
   * Create a Face.
   */
  explicit Face(std::string const &path,
                    std::size_t size = 30,
                    std::size_t hres = 64);

  /**
   * Free the memory allocated by the font Face.
   */
  ~Face();

  /**
   * Return the size of the characters.
   */
  std::size_t getCharacterSize() const;

  /**
   * Return the horizontal resolution.
   */
  std::size_t getHorizontalResolution() const;

  /**
   * Return ascender metric.
   */
  float getAscender() const;

  /**
   * Return descender metric.
   */
  float getDescender() const;

  /**
   * Return height metric.
   */
  float getHeight() const;

  /**
   * Return linegap metric.
   */
  float getLinegap() const;

  /**
   * Return the index for the given charcode.
   */
  unsigned int getCharIndex(wchar_t charcode) const;

  /**
   * Return the kerning value between the 2 given charcode.
   */
  math::Vector2i getKerning(wchar_t prev,
      wchar_t current,
      unsigned int mode = FT_KERNING_UNFITTED) const;

  /**
   * Load a glyph from its charcode.
   */
  ft::Glyph *loadGlyph(wchar_t charcode,
                       int flags = FT_LOAD_RENDER) const;

  /**
   * Return all characters presents into this font.
   */
  std::wstring getCharacters() const;

 private:
  FT_Face _face;
  std::size_t _size;
  std::size_t _hres;
};

}  // namespace ft
