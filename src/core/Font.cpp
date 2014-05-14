
#include <stdexcept>
#include <limits>
#include <cmath>
#include <wchar.h>
#include <iostream>
#include "ft/Error.hpp"
#include "ft/Library.hpp"
#include "core/Font.hpp"

namespace core {

Font::Font(std::string const &path, float size) :
  _face(path, size),
  _path(path),
  _size(size) {
  _cache = _face.getCharacters();
  _atlasOffset = 1;
  _textureWidth = 0;
  _textureHeight = 0;
  _height = 0;
  _padding = 0;
  _ascender = 0;
  _descender = 0;
  _ascender = _face.getAscender();
  _descender = _face.getDescender();
  _height = _face.getHeight();
  _linegap = _face.getLinegap();
}

Font::Font(std::string const &path,
                         float size, std::wstring const &cache) :
  _face(path, size),
  _path(path),
  _size(size) {
  _cache = cache;
  _atlasOffset = 1;
  _textureWidth = 0;
  _textureHeight = 0;
  _height = 0;
  _padding = 0;
  _ascender = 0;
  _descender = 0;
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

std::wstring const &Font::getCache() const {
  return _cache;
}

size_t Font::getSize() const {
  return _cache.size();
}

std::string Font::cacheToJson(size_t begin, size_t end, bool readable) const {
  std::string json = "[";
  for (size_t i = begin; i < end; ++i) {
    json += utils::convert<std::string>(_cache[i]);
    if (i != end - 1)
      json += ", ";
  }
  json += "]";
  return json;
}

std::string Font::toJson(bool rd) const {
  std::string json;

  if (_textureWidth == 0 || _textureHeight == 0)
    return "{}";
  std::string font_size = utils::convert<std::string>(_size);
  std::string atlas_width = utils::convert<std::string>(_textureWidth);
  std::string atlas_height = utils::convert<std::string>(_textureHeight);
  std::string glyphs_number = utils::convert<std::string>(_glyphs.size());

  json += "{" + _newline(rd);
  json += _space(2, rd) + "\"font_size\":" + _space(2, rd) + font_size + "," + _newline(rd);
  json += _space(2, rd) + "\"atlas_width\":" + _space(2, rd) + atlas_width + "," + _newline(rd);
  json += _space(2, rd) + "\"atlas_height\":" + _space(2, rd) + atlas_height + "," + _newline(rd);
  json += _space(2, rd) + "\"glyphs_number\":" + _space(2, rd) + glyphs_number + "," + _newline(rd);
  json += _space(2, rd) + "\"glyphs\":" + _space(2, rd) + "{" + _newline(rd);
  for (size_t i = 1; i < _glyphs.size(); ++i) {
    json += _space(4, rd) + _glyphs[i]->toJson();
    if (i + 1 < _glyphs.size())
      json += "," + _newline(rd);
  }
  json += "}}";
  return json;
}

size_t Font::generate(Atlas &atlas, size_t offset) {
  std::size_t size = _cache.size();
  _textureWidth = atlas.getWidth();
  _textureHeight = atlas.getHeight();
  for (std::size_t i = 0; i < size; ++i) {

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
  return size;
}

void Font::_computeKerning() {
  for (size_t i = 1; i < _glyphs.size(); ++i) {
    for (size_t j = 1; j < _glyphs.size(); ++j) {
      _glyphs[j]->kerning[_glyphs[i]->charcode] =
        _face.getKerning(_glyphs[i]->charcode, _glyphs[j]->charcode);
    }
  }
}

std::string Font::_space(size_t nb, bool readable) const {
  if (!readable)
    return "";
  std::string spaces;
  for (size_t i = 0; i < nb; ++i)
    spaces += " ";
  return spaces;
}

std::string Font::_newline(bool readable) const {
  return readable ? "\n" : "";
}

}
