
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include "vendors/edtaa3func.hpp"
#include "core/Distmap.hpp"

namespace core {

Distmap::Distmap(size_t width, size_t height) :
  Texture(width, height) {
}

Distmap::~Distmap() {}

void Distmap::generate(Texture const &other) {
  if (other.getWidth() != _width || other.getHeight() != _height)
    throw std::runtime_error(
        "Distmap cannot be generated from a different size texture.");

  short *xdist = (short *)malloc(_width * _height * sizeof(short));
  short *ydist = (short *)malloc(_width * _height * sizeof(short));
  double *gx = (double *)calloc(_width * _height, sizeof(double));
  double *gy = (double *)calloc(_width * _height, sizeof(double));
  double *data = (double *)calloc(_width * _height, sizeof(double));
  double *outside = (double *)calloc(_width * _height, sizeof(double));
  double *inside = (double *)calloc(_width * _height, sizeof(double));
  size_t i;

  if (!xdist || !ydist || !gx || !gy || !data || !outside || !inside)
    throw std::runtime_error("Cannot generated distmap, size is too big.");


  // Convert img into double (data)
  unsigned char *img = other.getData();
  double img_min = 255;
  double img_max = -255;
  for (i = 0; i < _width * _height; ++i) {
    double v = img[i];
    data[i] = v;
    if (v > img_max) img_max = v;
    if (v < img_min) img_min = v;
  }

  // Rescale image levels between 0 and 1
  for (i = 0; i < _width * _height; ++i) {
    data[i] = (img[i]-img_min)/img_max;
  }

  // Compute outside = edtaa3(bitmap); % Transform background (0's)
  computegradient(data, _width, _height, gx, gy);
  edtaa3(data, gx, gy, _height, _width, xdist, ydist, outside);
  for (i = 0; i < _width * _height; ++i)
    if (outside[i] < 0)
      outside[i] = 0.0;

  // Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
  memset(gx, 0, sizeof(double) * _width * _height);
  memset(gy, 0, sizeof(double) * _width * _height);
  for (i = 0; i < _width * _height; ++i)
    data[i] = 1 - data[i];
  computegradient(data, _width, _height, gx, gy);
  edtaa3(data, gx, gy, _height, _width, xdist, ydist, inside);
  for (i = 0; i < _width * _height; ++i) {
    if (inside[i] < 0)
      inside[i] = 0.0;
  }

  // distmap = outside - inside; % Bipolar distance field
  for (i = 0; i < _width * _height; ++i) {
    outside[i] -= inside[i];
    outside[i] = 128+outside[i]*16;
    if (outside[i] < 0) outside[i] = 0;
    if (outside[i] > 255) outside[i] = 255;
    _data[i] = 255 - (unsigned char) outside[i];
    _data[i + 1] = 255 - (unsigned char) outside[i];
    _data[i + 2] = 255 - (unsigned char) outside[i];
    _data[i + 3] = 255 - (unsigned char) outside[i];
  }

  free(xdist);
  free(ydist);
  free(gx);
  free(gy);
  free(data);
  free(outside);
  free(inside);
}

}