
#pragma once

#include <map>
#include "ft/ft.hpp"
#include FT_GLYPH_H

namespace ft {

/**
 * @struct Glyph
 * A structure that describe glyph data.
 */
class Glyph {
 public:
  /**
   * Create a glyph.
   */
  Glyph();

  /**
   * Clear all memory allocated by the glyph.
   */
  ~Glyph();

  /**
   * Get the kerning between two horizontal glyphs.
   * @param charcode  charcode of the previous glyph
   * @return x kerning value
   */
  float getKerning(const wchar_t charcode) const;

  /**
   * Get glyph's data into json format.
   */
  std::string toJson() const;

  wchar_t                  charcode;
  size_t                   atlas;
  std::size_t              width;
  std::size_t              height;
  int                      offset_x;
  int                      offset_y;
  float                    advance_x;
  float                    advance_y;
  float                    s0, t0, s1, t1;
  unsigned char            *buffer;
  std::size_t              pitch;
  std::map<wchar_t, float> kerning;
};

}  // namespace ft