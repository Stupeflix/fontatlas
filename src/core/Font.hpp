
#pragma once

#include <vector>
#include "ft/Glyph.hpp"
#include "ft/Face.hpp"
#include "core/Atlas.hpp"

namespace core {

/**
 * This class can generate a texture altas from a list of
 * characters.
 */
class Font {
 public:

  /**
   * Create a font from a filename and the font size.
   */
  Font(std::string const &path, float size);

  /**
   * Free all memory allocated by the font. However atlas isnt
   * deleted.
   */
  ~Font();

  /**
   * Set the padding value.
   */
  void setPadding(size_t padding);

  /**
   * Save font meta-data to a json file.
   */
  std::string toJson() const;

  /**
   * Generate the texture from the chars into the texture fonts
   * and others settings (like padding).
   */
  size_t generate(Atlas &atlas, size_t offset = 0);

  /**
   * Load the font and store data into the given `FT_Face`.
   */
  void _loadFace(FT_Face *face, float k = 1);

  /**
   * Compute the kerning of all characters of the font.
   */
  void _computeKerning();

  std::vector<ft::Glyph *> _glyphs;
  ft::Face _face;
  std::string _path;
  std::wstring _cache;
  size_t _padding;
  size_t _textureWidth;
  size_t _textureHeight;
  float _size;
  float _height;
  float _linegap;
  float _ascender;
  float _descender;
};

}
