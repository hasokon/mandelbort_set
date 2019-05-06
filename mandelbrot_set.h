#ifndef _MANDELBROT_SET_H_
#define _MANDELBROT_SET_H_

#include <cstdint>
#include <vector>
#include <cstddef>
#include <thread>
#include <mutex>
//#include <iostream>

namespace hasokon {

namespace mandelbrot {

static const uint32_t DEFAULT_CALCSIZE { 100 };
static const uint32_t BOUNDARY { 2 };
static const uint32_t DIVERGENCE { 0 };
static const uint32_t DEFAULT_BLOCK_SIZE { 10000 };

template <typename FloatT>
uint32_t include_in_mandelbrot_set(FloatT const x, FloatT const y, uint32_t const calc_size = DEFAULT_CALCSIZE) {
  FloatT za = 0.0;
  FloatT zb = 0.0;
  FloatT ca = x;
  FloatT cb = y;
  FloatT t;
  constexpr auto boundary = static_cast<FloatT>(BOUNDARY) * static_cast<FloatT>(BOUNDARY);
  for (uint32_t i = 1; i <= calc_size; ++i) {
    if ((za * za + zb * zb) > boundary) return i;
    t = za * za - zb * zb + ca;
    zb = 2 * za * zb + cb;
    za = t;
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
  uint32_t _block_size;
  uint32_t _all_size;
  int32_t _current_point;
  std::mutex _mtx;
public:
  mandelbrot_set (FloatT const sx, FloatT const sy, FloatT w, FloatT h, FloatT const interval, uint32_t const bs = DEFAULT_BLOCK_SIZE) :
  _interval(interval),
  _startPointX(sx),
  _startPointY(sy),
  _width(w),
  _height(h),
  _relativeWidth(static_cast<uint32_t>(w/interval)),
  _relativeHeight(static_cast<uint32_t>(h/interval)),
  _block_size(bs)
  {
    _all_size = _relativeWidth * _relativeHeight;
    _current_point = 0;
    _data = std::vector<uint8_t>(_all_size, 0);
  }

  mandelbrot_set(FloatT const sx, FloatT const sy, FloatT const len, FloatT const interval) : mandelbrot_set(sx, sy, len, len, interval) {}
  mandelbrot_set(FloatT const s, FloatT const len, FloatT const interval) : mandelbrot_set(s, s, len, interval) {}
  mandelbrot_set(FloatT const interval) : mandelbrot_set(-1 * static_cast<FloatT>(BOUNDARY), static_cast<FloatT>(BOUNDARY * 2), interval) {}

  void calc(uint32_t const head, uint32_t const tail) {
    uint32_t rx = head % _relativeWidth;
    uint32_t ry = head / _relativeWidth;
    uint32_t erx = tail % _relativeWidth;
    uint32_t ery = tail / _relativeWidth;
    FloatT x = _startPointX + rx * _interval;
    FloatT y = _startPointY - ry * _interval;
    uint32_t point = head;
    for (; ry < ery; ++ry) {
      for (; rx < _relativeWidth; ++rx) {
        _data[point] = include_in_mandelbrot_set(x, y);
        x += _interval;
        ++point;
      }
      x = _startPointX;
      rx = 0;
      y -= _interval;
    }
    for (rx = 0; rx < erx; ++rx) {
      _data[point] = include_in_mandelbrot_set(x, y);
      x += _interval;
    }
  }

  void doWork() {
    while (true) {
      // Lock
      _mtx.lock();
      int32_t head = _current_point;
      int32_t next_point = _current_point + _block_size;
      if (head >= 0) {
        _current_point = next_point;
        if (_current_point > _all_size) {
          next_point = _all_size;
          _current_point = -1;
        }
      }
      // UnLock
      _mtx.unlock();

      if (head < 0) {
        break;
      }
      calc(head, next_point);
    }
  }

  void calc_all_1_thread_simple() {
    _data = std::vector<uint8_t>(_relativeWidth * _relativeHeight);
    _data.clear();
    FloatT x = _startPointX;
    FloatT y = _startPointY;
    for (uint32_t ry = 0; ry < _relativeHeight; ++ry) {
      x = _startPointX;
      for (uint32_t rx = 0; rx < _relativeWidth; ++rx) {
        _data.push_back(include_in_mandelbrot_set(x, y));
        x += _interval;
      }
      y -= _interval;
    }
 }
 
  void calc_all_2_thread_simple() {
    uint32_t const thread_num = 2;
    uint32_t size = _relativeHeight * _relativeWidth;
    uint32_t const block_size = size / thread_num;
    uint32_t head = 0;
    _data = std::vector<uint8_t>(size);

    std::thread th1([this, head, block_size]{calc(head, head + block_size);});

    calc(head + block_size, size);

    th1.join();
  }

  void calc_all_4_thread_simple() {
    uint32_t const thread_num = 4;
    uint32_t size = _relativeHeight * _relativeWidth;
    uint32_t const block_size = size / thread_num;
    uint32_t head = 0;
    _data = std::vector<uint8_t>(size);

    std::thread th1([this, head, block_size]{calc(head, head + block_size);});
    std::thread th2([this, head, block_size]{calc(head + block_size, head + block_size * 2);});
    std::thread th3([this, head, block_size]{calc(head + block_size + 2, head + block_size * 3);});

    calc(head + block_size * 3, size);

    th1.join();
    th2.join();
    th3.join();
  }

  void calc_all_multi_thread_simple() {
    uint32_t const cpu_thread_num = std::thread::hardware_concurrency();
    uint32_t size = _relativeHeight * _relativeWidth;
    uint32_t const block_size = size / cpu_thread_num;
    uint32_t head = 0;
    _data = std::vector<uint8_t>(size);
    std::vector<std::thread> threads(cpu_thread_num-1);

    for (; head < size - block_size; head += block_size) {
      threads.emplace_back([this, head, block_size]{calc(head, head + block_size);});
    }

    calc(head, size);

    for (auto& th : threads) {
      if (th.joinable()) {
        th.join();
      }
    }
  }

  void calc_all_multi_worker_thread() {
    uint32_t const cpu_thread_num = std::thread::hardware_concurrency();
    _current_point = 0;
    _data = std::vector<uint8_t>(_all_size);
    std::vector<std::thread> threads(cpu_thread_num-1);

    for (auto i = 0; i < cpu_thread_num - 1; ++i) {
      threads.emplace_back([this]{doWork();});
    }

    doWork();

    for (auto& th : threads) {
      if (th.joinable()) {
        th.join();
      }
    }
  }

  uint8_t getByRelativePoint(uint32_t const x, uint32_t const y) const {
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
