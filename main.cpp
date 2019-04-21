#include <iostream>
#include <ios>

#include "mandelbrot_set.h"

int
main () {
  auto sx = 0.2L;
  auto sy = 0.6L;
  auto len = 0.05L;
  auto interval = 0.00002L;
  hasokon::mandelbrot::mandelbrot_set set{ sx, sy, len, interval };

  std::cout << "P1" << std::endl;
  std::cout << set.getRelativeWidth();
  std::cout << " ";
  std::cout << set.getRelativeHeight();
  std::cout << std::endl;

  for (uint32_t y = 0; y < set.getRelativeHeight(); ++y) {
    for (uint32_t x = 0; x < set.getRelativeWidth(); ++x) {
      std::cout << set.getByRelativePoint(x,y);
      std::cout << " ";
    }
    std::cout << std::endl;
  }
}
