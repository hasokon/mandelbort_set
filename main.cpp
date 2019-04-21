#include <iostream>
#include <ios>

#include "mandelbrot_set.h"

void print_color(uint32_t const & divergence) {
  if (divergence == hasokon::mandelbrot::DIVERGENCE) {
    std::cout << "255 255 255" << std::endl;
    return;
  }
  int32_t c = divergence*5;
  uint8_t b = c > 255 ? 255 : c;
  std::cout << "0 0 ";
  std::cout << static_cast<int>(b) << std::endl;
}

int
main () {
  auto sx = -2.0L;
  auto sy = 2.0L;
  auto len = 2.0L;
  auto interval = 0.001L;
  hasokon::mandelbrot::mandelbrot_set set{ sx, sy, len, interval };

  std::cout << "P3" << std::endl;
  std::cout << set.getRelativeWidth();
  std::cout << " ";
  std::cout << set.getRelativeHeight();
  std::cout << std::endl;
  std::cout << 255 << std::endl;

  for (uint32_t y = 0; y < set.getRelativeHeight(); ++y) {
    for (uint32_t x = 0; x < set.getRelativeWidth(); ++x) {
      print_color(set.getByRelativePoint(x,y));
    }
  }
}
