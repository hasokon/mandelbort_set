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

template <typename FloatT>
bool include_in_mandelbrot_set(FloatT const x, FloatT const y, uint32_t const calc_size = DEFAULT_CALCSIZE) {
  std::complex const c { x, y };
  std::complex z{ static_cast<FloatT>(0.0), static_cast<FloatT>(0.0) };
  auto boundary = static_cast<FloatT>(BOUNDARY);
  uint32_t i = calc_size;
  while (i--) {
    if (std::abs(z) > boundary) return false;
    z = z * z + c;
  }
  return true;
}

template <typename FloatT>
class mandelbrot_set {
private:
  std::vector<bool> _data;
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
    for (FloatT y = sy; y >= sy - h; y -= interval) {
      for (FloatT x = sx; x <= sx + w; x += interval) {
        _data.push_back(include_in_mandelbrot_set(x, y));
      }
    }
  }

  mandelbrot_set(FloatT const sx, FloatT const sy, FloatT const len, FloatT const interval) : mandelbrot_set(sx, sy, len, len, interval) {}
  mandelbrot_set(FloatT const s, FloatT const len, FloatT const interval) : mandelbrot_set(s, s, len, interval) {}
  mandelbrot_set(FloatT const interval) : mandelbrot_set(-1 * static_cast<FloatT>(BOUNDARY), static_cast<FloatT>(BOUNDARY * 2), interval) {}

  bool const getByRelativePoint(uint32_t const x, uint32_t const y) const {
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
