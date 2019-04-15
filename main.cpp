#include <iostream>
#include <ios>

#include "mandelbrot_set.h"

int
main () {
  auto interval = 0.0005L;
  hasokon::mandelbrot::mandelbrot_set set{ interval };

  std::cout << "P1" << std::endl;
  std::cout << static_cast<unsigned long int>((2.0L / interval) * 2);
  std::cout << " ";
  std::cout << static_cast<unsigned long int>((2.0L / interval) * 2);
  std::cout << std::endl;

  for (long double x = -2.0L; x <= 2.0L; x += interval) {
    for (long double y = -2.0L; y <= 2.0L; y += interval) {
      std::cout << set.get(x,y);
      std::cout << " ";
    }
    std::cout << std::endl;
  }
}
