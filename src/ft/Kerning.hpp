
#pragma once

namespace ft {

/**
 * @struct Kerning
 * Simple structure which reprensents a link between a character
 * and its kerning.
 */
struct Kerning {
  wchar_t charcode; /** Left character code in the pair. */
  float kerning; /** Kerning value (in fractional pixels). */
};

}