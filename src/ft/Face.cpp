
#include <iostream>
#include <limits>
#include "ft/Face.hpp"

namespace ft {

Face::Face(std::string const &path, std::size_t size, std::size_t hres) :
  _size(size),
  _hres(hres) {
  FT_Error error;
  FT_Matrix matrix = {(int)((1.0/hres) * 0x10000L),
                      (int)((0.0)       * 0x10000L),
                      (int)((0.0)       * 0x10000L),
                      (int)((1.0)       * 0x10000L)};

  /* Load face */
  error = FT_New_Face(ft::Library::getHandle(),
                      path.c_str(), 0, &_face);
  if (error)
    throw ft::Error(error, __FILE__, __LINE__);

  /* Select charmap */
  error = FT_Select_Charmap(_face, FT_ENCODING_UNICODE);
  if (error)
    throw ft::Error(error, __FILE__, __LINE__);

  /* Set char size */
  error = FT_Set_Char_Size(_face, (int)(size * 64),
                           0, 72 * hres, 72);
  if (error)
    throw ft::Error(error, __FILE__, __LINE__);

  /* Set transform matrix */
  FT_Set_Transform(_face, &matrix, NULL);
}

Face::~Face() {
  FT_Done_Face(_face);
}

std::size_t Face::getCharacterSize() const {
  return _size;
}

std::size_t Face::getHorizontalResolution() const {
  return _hres;
}

float Face::getAscender() const {
  return _face->size->metrics.ascender >> 6;
}

float Face::getDescender() const {
  return _face->size->metrics.descender >> 6;
}

float Face::getHeight() const {
  return _face->size->metrics.height >> 6;
}

float Face::getLinegap() const {
  return getHeight() - getAscender() + getDescender();
}

unsigned int Face::getCharIndex(wchar_t charcode) const {
  return FT_Get_Char_Index(_face, charcode);
}

ft::Glyph *Face::loadGlyph(wchar_t charcode, int flags) const {
  FT_Error error = FT_Load_Glyph(_face, getCharIndex(charcode), flags);
  if (error)
    throw ft::Error(error);
  ft::Glyph *glyph = new ft::Glyph;
  glyph->charcode = charcode;
  glyph->offset_x = _face->glyph->bitmap_left;
  glyph->offset_y = _face->glyph->bitmap_top;
  glyph->width = _face->glyph->bitmap.width;
  glyph->height = _face->glyph->bitmap.rows;
  glyph->advance_x = _face->glyph->advance.x / _hres;
  glyph->advance_y = _face->glyph->advance.y / _hres;
  glyph->buffer = _face->glyph->bitmap.buffer;
  glyph->pitch = _face->glyph->bitmap.pitch;
  return glyph;
}

float Face::getKerning(wchar_t prev,
                       wchar_t current,
                       unsigned int mode) const {
  math::Vector2i v;
  FT_Vector tmp;
  FT_Get_Kerning(_face, getCharIndex(prev), getCharIndex(current), mode, &tmp);
  v.x = tmp.x;
  v.y = tmp.y;
  return v.x / float(_hres * _hres);
}

std::wstring Face::getCharacters() const {
  std::wstring chars;
  wchar_t i = 0;
  wchar_t end = 65536;
  // wchar_t end = std::numeric_limits<wchar_t>::max();
  while (i < end) {
    if (getCharIndex(i) != 0)
      chars += i;
    ++i;
  }
  return chars;
}

}
