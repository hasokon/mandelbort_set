#include <iostream>
#include <ios>

#include "mandelbrot_set.h"

int
main () {
  auto interval = 0.1L;
  hasokon::mandelbrot::mandelbrot_set set{ interval };
  for (long double x = -2.0L; x <= 2.0L; x += interval) {
    for (long double y = -2.0L; y <= 2.0L; y += interval) {
      std::cout << set.get(x,y);
      std::cout << set.get(x,y);
    }
    std::cout << std::endl;
  }
}
