
#include <stdexcept>
#include <limits>
#include <cmath>
#include <wchar.h>
#include "ft/Error.hpp"
#include "ft/Library.hpp"
#include "core/Font.hpp"

namespace core {

Font::Font(std::string const &path,
                         float size) :
  _face(path, size),
  _path(path),
  _size(size) {
  if (size <= 0)
    throw std::runtime_error("Font size should be positive.");
  _cache = _face.getCharacters();
  _atlasOffset = 1;
  _textureWidth = 0;
  _textureHeight = 0;
  _height = 0;
  _padding = 0;
  _ascender = 0;
  _descender = 0;
  _size = size;
  _ascender = _face.getAscender();
  _descender = _face.getDescender();
  _height = _face.getHeight();
  _linegap = _face.getLinegap();
}

Font::~Font() {
  for (auto glyph : _glyphs)
    delete glyph;
}

void Font::setPadding(size_t padding) {
  _padding = padding;
}

size_t Font::getSize() const {
  return _cache.size();
}

std::string Font::toJson() const {
  std::string json;

  if (_textureWidth == 0 || _textureHeight == 0)
    return "{}";
  std::string atlas_width = utils::convert<std::string>(_textureWidth);
  std::string atlas_height = utils::convert<std::string>(_textureHeight);
  std::string glyphs_number = utils::convert<std::string>(_glyphs.size());

  json += "{\n";
  json += "  \"atlas_width\": " + atlas_width + ",\n";
  json += "  \"atlas_height\": " + atlas_height + ",\n";
  json += "  \"glyphs_number\": " + glyphs_number + ",\n";
  json += "  \"glyphs\": {\n";
  for (size_t i = 1; i < _glyphs.size(); ++i) {
    json += "    " + _glyphs[i]->toJson();
    if (i + 1 < _glyphs.size())
      json += ",";
    json += "\n";
  }
  json += "  }\n";
  json += "}\n";
  return json;
}

size_t Font::generate(Atlas &atlas, size_t offset) {
  std::size_t s = _cache.size();
  if (offset > s)
    throw std::out_of_range(
        "charcode offset cannot be higher than cache size.");

  _textureWidth = atlas.getWidth();
  _textureHeight = atlas.getHeight();
  for (std::size_t i = offset; i < s; ++i) {

    /* Load glyphs data from face */
    std::size_t x = 0;
    std::size_t y = 0;
    ft::Glyph *glyph = _face.loadGlyph(_cache[i]);

    /* Place the texture into the atlas */
    size_t w = glyph->width + _padding;
    size_t h = glyph->height + _padding;
    math::Rect const &region = atlas.getRegion(w, h);

    /* Return the current cache offset if the atlas is full */
    if (region.x == std::numeric_limits<size_t>::max()) {
      ++_atlasOffset;
      return i;
    }

    /* Push glyph bitmap into atlas */
    x = region.x + (_padding >> 1);
    y = region.y + (_padding >> 1);
    atlas.setRegion(x, y, glyph->width, glyph->height,
                     glyph->buffer, glyph->pitch);

    /* Set current atlas */
    glyph->atlas = _atlasOffset;

    /* Deduce texture coordinates from atlas region */
    glyph->s0 = x / (float)atlas.getWidth();
    glyph->t0 = y / (float)atlas.getHeight();
    glyph->s1 = (x + glyph->width) / (float)atlas.getWidth();
    glyph->t1 = (y + glyph->height) / (float)atlas.getHeight();
    _glyphs.push_back(glyph);

  }
  _computeKerning();
  return s;
}

void Font::_computeKerning() {
  ft::Glyph *glyph, *prev;
  
  /* Compute kernings. */
  for (size_t i = 1; i < _glyphs.size(); ++i) {
    glyph = _glyphs[i];
    for (size_t j = 1; j < _glyphs.size(); ++j) {
      prev = _glyphs[j];
      math::Vector2i const &kerning =
          _face.getKerning(prev->charcode, glyph->charcode);
      if (kerning.x)
        glyph->kerning[prev->charcode] = kerning.x / (float)(64.0f * 64.0f);
    }
  }
}

}
