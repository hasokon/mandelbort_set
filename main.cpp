#include <iostream>
#include <cstdint>

#include "mandelbrot_set.h"

using namespace hasokon::mandelbrot;

void print_color(uint32_t const & divergence) {
  if (divergence == DIVERGENCE) {
    std::cout << "255 255 255" << std::endl;
    return;
  }
  int32_t c = divergence*5;
  uint8_t b = c > 255 ? 255 : c;
  std::cout << "0 0 ";
  std::cout << static_cast<int>(b) << std::endl;
}

void
print_mandelbrot_set(mandelbrot_set<long double> const & ms) {
  std::cout << "P3" << std::endl;
  std::cout << ms.getRelativeWidth();
  std::cout << " ";
  std::cout << ms.getRelativeHeight();
  std::cout << std::endl;
  std::cout << 255 << std::endl;

  for (uint32_t y = 0; y < ms.getRelativeHeight(); ++y) {
    for (uint32_t x = 0; x < ms.getRelativeWidth(); ++x) {
      print_color(ms.getByRelativePoint(x,y));
    }
  }
}

int
main () {
  auto sx = -1.0L;
  auto sy = 1.0L;
  auto w = 1.0L;
  auto h = 1.0L;
  auto interval = 0.0005L;
//  auto sx = -0.27L;
//  auto sy = 1.1L;
//  auto w = 0.27L;
//  auto h = 0.5L;
//  auto interval = 0.00005L;
  mandelbrot_set<long double> set{ sx, sy, w, h, interval };
  //set.calc_all_multi_thread_simple();
  //set.calc_all_1_thread_simple();
  set.calc_all_4_thread_simple();
  //print_mandelbrot_set(set);
}
