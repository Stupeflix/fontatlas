
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <png.h>
#include "core/Texture.hpp"

namespace core {

Texture::Texture() :
  _data(NULL),
  _width(0),
  _height(0) {
}

Texture::Texture(size_t width, size_t height) :
  _data(new unsigned char[width * height * 4]),
  _width(width),
  _height(height) {
  std::memset(_data, 0, width * height * 4);
}

Texture::Texture(Texture const &other) {
  *this = other;
}

Texture &Texture::operator=(Texture const &other) {
  if (this != &other) {
    if (_data)
      delete _data;
    _width = other._width;
    _height = other._height;
    _data = new unsigned char[_width * _height * 4];
    std::memcpy(_data, other._data, _width * _height * 4);
  }
  return *this;
}

Texture::~Texture() {
  if (_data)
    delete _data;
}

unsigned char *Texture::getData() const {
  return _data;
}

std::size_t Texture::getWidth() const {
  return _width;
}

std::size_t Texture::getHeight() const {
  return _height;
}

void Texture::loadFromPng(std::string const &path) {
  FILE *fp = fopen(path.c_str(), "rb");
  if (!fp)
    throw std::runtime_error("Cannot open png \"" + path + "\".");
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
    throw std::runtime_error("Cannot create png.");
  png_infop info = png_create_info_struct(png);
  if (!info)
    throw std::runtime_error("Cannot load png info.");
  if (setjmp(png_jmpbuf(png)))
    throw std::runtime_error("Cannot init png io.");
  png_init_io(png, fp);
  png_read_info(png, info);
  _width = png_get_image_width(png, info);
  _height = png_get_image_height(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);
  if (bit_depth == 16)
    png_set_strip_16(png);
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);
  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);
  if (color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
  if (color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);
  png_read_update_info(png, info);
  png_bytep *row_pointers;

  /* Read data from file */
  row_pointers = new png_bytep[_height];
  for (int y = 0; y < _height; y++)
    row_pointers[y] = new png_byte[png_get_rowbytes(png,info)];
  png_read_image(png, row_pointers);
  fclose(fp);

  /* Copy data. */
  if (_data)
    delete _data;
  _data = new unsigned char[_width * _height * 4];
  for (int y = 0; y < _height; y++) {
    png_bytep row = row_pointers[y];
    for (int x = 0; x < _width; x++) {
      png_bytep px = &(row[x * 4]);
      int i = (y * _width + x) * 4;
      _data[i] = px[0];
      _data[i + 1] = px[1];
      _data[i + 2] = px[2];
      _data[i + 3] = px[3];
    }
  }

  /* Free */
  png_destroy_read_struct(&png, &info, NULL);
  for (int y = 0; y < _height; y++)
    delete[] row_pointers[y];
  delete[] row_pointers;
}

void Texture::saveToPng(std::string const &path,
                        ColorType type) {
  int y;
  FILE *fp = fopen(path.c_str(), "wb");
  if (!fp)
    throw std::runtime_error("Cannot open png \"" + path + "\".");
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();
  png_infop info = png_create_info_struct(png);
  if (!info) abort();
  if (setjmp(png_jmpbuf(png))) abort();
  png_init_io(png, fp);
  png_set_IHDR(
    png,
    info,
    _width, _height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  /* Copy data */
 
  png_bytep *row_pointers;
  row_pointers = new png_bytep[_height];
  for (int y = 0; y < _height; y++) {
    row_pointers[y] = new png_byte[png_get_rowbytes(png,info)];
    for (int x = 0; x < _width; ++x) {
      png_bytep px = &(row_pointers[y][x * 4]);
      int i = (y * _width + x);
      switch (type) {
        case AlphaRate:
          px[0] = 255;
          px[1] = 255;
          px[2] = 255;
          px[3] = _data[i];
          break;
        case BlackAndWhite:
          px[0] = _data[i];
          px[1] = _data[i];
          px[2] = _data[i];
          px[3] = 255;
          break;
        default:
          px[0] = _data[i];
          px[1] = _data[i + 1];
          px[2] = _data[i + 2];
          px[3] = _data[i + 3];
          break;
      };
    }
  }

  /* Write to file. */

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  /* Free allocated memory. */
  png_destroy_write_struct(&png, &info);
  for (int y = 0; y < _height; y++)
    delete[] row_pointers[y];
  delete[] row_pointers;
  fclose(fp);
}

}
