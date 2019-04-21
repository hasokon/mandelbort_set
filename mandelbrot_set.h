#ifndef _MANDELBROT_SET_H_
#define _MANDELBROT_SET_H_

#include <string>
#include <complex>
#include <cstdint>
#include <vector>
#include <cstddef>

namespace hasokon {

namespace mandelbrot {

static const uint32_t DEFAULT_CALCSIZE { 100 };
static const uint32_t BOUNDARY { 2 };
static const uint32_t DIVERGENCE { 0 };

template <typename FloatT>
uint32_t include_in_mandelbrot_set(FloatT const x, FloatT const y, uint32_t const calc_size = DEFAULT_CALCSIZE) {
  std::complex const c { x, y };
  std::complex z{ static_cast<FloatT>(0.0), static_cast<FloatT>(0.0) };
  auto boundary = static_cast<FloatT>(BOUNDARY);
  for (uint32_t i = 1; i <= calc_size; ++i) {
    if (std::abs(z) > boundary) return i;
    z = z * z + c;
  }
  return 0;
}

template <typename FloatT>
class mandelbrot_set {
private:
  std::vector<uint8_t> _data;
  FloatT _interval;
  FloatT _startPointX;
  FloatT _startPointY;
  FloatT _width;
  FloatT _height;
  uint32_t _relativeWidth;
  uint32_t _relativeHeight;
public:
  mandelbrot_set (FloatT const sx, FloatT const sy, FloatT w, FloatT h, FloatT const interval) :
  _interval(interval),
  _startPointX(sx),
  _startPointY(sy),
  _width(w),
  _height(h),
  _relativeWidth(static_cast<uint32_t>(w/interval)),
  _relativeHeight(static_cast<uint32_t>(h/interval)),
  _data(static_cast<size_t>(_relativeWidth * _relativeHeight))
  {
    _data.clear();
    FloatT x = _startPointX;
    FloatT y = _startPointY;
    for (uint32_t ry = 0; ry < _relativeWidth; ++ry) {
      x = _startPointX;
      for (uint32_t rx = 0; rx < _relativeWidth; ++rx) {
        _data.push_back(include_in_mandelbrot_set(x, y));
        x += _interval;
      }
      y -= _interval;
    }
  }

  mandelbrot_set(FloatT const sx, FloatT const sy, FloatT const len, FloatT const interval) : mandelbrot_set(sx, sy, len, len, interval) {}
  mandelbrot_set(FloatT const s, FloatT const len, FloatT const interval) : mandelbrot_set(s, s, len, interval) {}
  mandelbrot_set(FloatT const interval) : mandelbrot_set(-1 * static_cast<FloatT>(BOUNDARY), static_cast<FloatT>(BOUNDARY * 2), interval) {}

  uint8_t const getByRelativePoint(uint32_t const x, uint32_t const y) const {
    return _data.at(y * _relativeWidth + x);
  }

  FloatT const getInterval() const noexcept {
    return _interval;
  }

  FloatT const getStartPointX() const noexcept {
    return _startPointX;
  }

  FloatT const getStartPointY() const noexcept {
    return _startPointY;
  }

  FloatT getWidth() const noexcept {
    return _width;
  }

  FloatT getHeight() const noexcept {
    return _height;
  }

  uint32_t getRelativeWidth() const noexcept {
    return _relativeWidth;
  }

  uint32_t getRelativeHeight() const noexcept {
    return _relativeHeight;
  }
};

}
}

#endif
