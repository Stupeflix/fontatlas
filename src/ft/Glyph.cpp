
#include <iostream>
#include "utils/convert.hpp"
#include "ft/Glyph.hpp"

namespace ft {

Glyph::Glyph() :
  width(0), height(0),
  offset_x(0), offset_y(0),
  advance_x(0), advance_y(0),
  s0(0), t0(0), s1(0), t1(0) {
}

Glyph::~Glyph() {}

float Glyph::getKerning(const wchar_t charcode) const {
  for (auto &k : kerning) {
    if (k.charcode == charcode)
      return k.kerning;
  }
  return 0;
}

std::string Glyph::toJson() const {
  std::string json;
  json += "\"" + utils::convert<std::string>(charcode) + "\":{";
  json += "\"offset_x\": " + utils::convert<std::string>(offset_x) + ",";
  json += "\"offset_y\": " + utils::convert<std::string>(offset_y) + ",";
  json += "\"advance_x\": " + utils::convert<std::string>(advance_x) + ",";
  json += "\"advance_y\": " + utils::convert<std::string>(advance_y) + ",";
  json += "\"width\": " + utils::convert<std::string>(width) + ",";
  json += "\"height\": " + utils::convert<std::string>(height) + ",";
  json += "\"s0\": " + utils::convert<std::string>(s0) + ",";
  json += "\"t0\": " + utils::convert<std::string>(t0) + ",";
  json += "\"s1\": " + utils::convert<std::string>(s1) + ",";
  json += "\"t1\": " + utils::convert<std::string>(t1) + "}";
  return json;
}

}  // namespace ft
