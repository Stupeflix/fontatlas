
#pragma once

#include <vector>
#include "ft/ft.hpp"
#include "ft/Kerning.hpp"
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
   * @param charcode  codepoint of the preceding glyph
   * @return x kerning value
   */
  float getKerning(const wchar_t charcode) const;

  /**
   * Get glyph's data into json format.
   */
  std::string toJson() const;

  /** Glyph's charcode. */
  wchar_t charcode;

  /** Glyph's width in pixels. */
  std::size_t width;

  /** Glyph's height in pixels. */
  std::size_t height;

  /** Glyph's left offset in pixels. */
  int offset_x;

  /** Glyph's top offset in pixels. */
  int offset_y;

  /** Glyph's advancement relative to the previous char. */
  float advance_x;

  /** Glyph's advancement relative to the previous line. */
  float advance_y;

  /** Normalized texture coordinates. */
  float s0, t0, s1, t1;

  unsigned char *buffer;

  std::size_t pitch;

  /**
   * A vector of kerning pairs relative to this glyph.
   */
  std::vector<Kerning> kerning;
};

}  // namespace ft