#ifndef _MANDELBROT_SET_H_
#define _MANDELBROT_SET_H_

#include <string>
#include <complex>
#include <cstdint>
#include <vector>
#include <map>
#include <cstddef>

namespace hasokon {

namespace mandelbrot {

static const uint32_t DEFAULT_CALCSIZE { 100 };
static const uint32_t BOUNDARY { 2 };

template <typename FloatT>
bool include_in_mandelbrot_set(FloatT const x, FloatT const y, uint32_t const calc_size = DEFAULT_CALCSIZE) {
  std::complex const c { x, y };
  std::complex z{ static_cast<FloatT>(0.0), static_cast<FloatT>(0.0) };
  uint32_t i = calc_size;
  while (i--) {
    if (std::abs(z) > static_cast<FloatT>(BOUNDARY)) return false;
    z = z * z + c;
  }
  return true;
}

template <typename FloatT>
class mandelbrot_set {
private:
  std::map<FloatT, std::map<FloatT, bool>> _set{};
  FloatT _interval;
public:
  constexpr mandelbrot_set (FloatT const interval) noexcept : _interval(interval) {
    FloatT const boundary = static_cast<FloatT>(BOUNDARY);
    uint32_t const size = static_cast<uint32_t>( boundary / interval);
    for (FloatT y = -1 * boundary; y <= boundary; y += interval) {
      _set[y] = std::map<FloatT, bool>{};
      for (FloatT x = -1 * boundary; x <= boundary; x += interval) {
        _set[y][x] = include_in_mandelbrot_set(x, y);
      }
    }
  }

  bool const get(FloatT const x, FloatT const y) const {
    return _set.at(x).at(y);
  }

  void export_file(std::string const filename) const {
  }
};

}
}

#endif
