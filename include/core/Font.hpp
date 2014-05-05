
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
   * Create a font from a filename and the font size and the characters to load.
   */
  Font(std::string const &path, float size, std::wstring const &cache);

  /**
   * Create a font and load all its characters.
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
   * Return number of utf-8 chars generated from font.
   */
  size_t getSize() const;

  /**
   * Return the characters loaded in the font.
   */
  std::wstring const &getCache() const;

  /**
   * Return cache to JSON.
   */
  std::string cacheToJson(size_t begin, size_t end, bool readable = false) const;

  /**
   * Save font meta-data to a json file.
   */
  std::string toJson(bool readable = false) const;

  /**
   * Generate the texture from the chars into the texture fonts
   * and others settings (like padding). Note that it will also
   * store the generated chars into `_genCache` vector which is
   * useful for json meta-data generation.
   * @param atlas The atlas to fill by the generation.
   * @param offset The offset to use into the cache.
   * @return The current offset generated.
   */
  size_t generate(Atlas &atlas, size_t offset = 0);

 private:
  /**
   * Load the font and store data into the given `FT_Face`.
   */
  void _loadFace(FT_Face *face, float k = 1);

  /**
   * Compute the kerning of all characters of the font.
   */
  void _computeKerning();

  std::string _space(size_t nb, bool readable) const;

  std::string _newline(bool readable) const;

  std::vector<ft::Glyph *> _glyphs;
  ft::Face _face;
  std::string _path;
  std::wstring _cache;
  size_t _atlasOffset;
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
