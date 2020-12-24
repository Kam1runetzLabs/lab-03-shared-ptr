// Copyright 2020 Kam1runetzLabs <notsoserious2017@gmail.com>

#ifndef INCLUDE_SHARED_PTR_HPP_
#define INCLUDE_SHARED_PTR_HPP_

#include <atomic>
#include <utility>

template <class T>
class shared_ptr {
 public:
  shared_ptr();
  explicit shared_ptr(T *ptr);
  shared_ptr(const shared_ptr &r);
  shared_ptr(shared_ptr &&rr) noexcept;
  ~shared_ptr();

  auto operator=(const shared_ptr &r) -> shared_ptr &;
  auto operator=(shared_ptr &&rr) noexcept -> shared_ptr &;

  explicit operator bool() const;
  auto operator*() const -> T &;
  auto operator->() const -> T *;

  auto get() -> T *;
  void reset();
  void reset(T *ptr);
  void swap(shared_ptr &r);

  auto use_count() const -> std::size_t;

 private:
  T *_data;
  std::atomic_uint *_counter;
};

template <class T>
shared_ptr<T>::shared_ptr() : _data(nullptr), _counter(nullptr) {}

template <class T>
shared_ptr<T>::shared_ptr(T *ptr)
    : _data(ptr), _counter(new std::atomic_uint(1)) {}

template <class T>
shared_ptr<T>::shared_ptr(const shared_ptr &r)
    : _data(r._data), _counter(r._counter) {
  ++(*_counter);
}

template <class T>
shared_ptr<T>::shared_ptr(shared_ptr &&rr) noexcept
    : _data(nullptr), _counter(nullptr) {
  std::swap(_data, rr._data);
  std::swap(_counter, rr._counter);
}

template <class T>
shared_ptr<T>::~shared_ptr() {
  if (!_counter) return;
  if (--(*_counter) == 0) {
    delete _data;
    delete _counter;
  }
}

template <class T>
auto shared_ptr<T>::operator=(const shared_ptr &r) -> shared_ptr & {
  if (r == *this) return *this;
  reset();
  _counter = r._counter;
  _data = r._data;
  ++(*_counter);
  return *this;
}

template <class T>
auto shared_ptr<T>::operator=(shared_ptr &&r) noexcept -> shared_ptr & {
  if (r == *this) return *this;
  reset();
  _counter = r.counter;
  _data = r._data;
  r._counter = nullptr;
  r._data = nullptr;
  return *this;
}

template <class T>
shared_ptr<T>::operator bool() const {
  return _data != nullptr;
}

template <class T>
auto shared_ptr<T>::operator*() const -> T & {
  return *_data;
}

template <class T>
auto shared_ptr<T>::operator->() const -> T * {
  return _data;
}

template <class T>
auto shared_ptr<T>::get() -> T * {
  return _data;
}

template <class T>
void shared_ptr<T>::reset() {
  if (!_counter) return;
  --(*_counter);
  if (*_counter == 0) {
    delete _data;
    delete _counter;
  }
  _data = _counter = nullptr;
}

template <class T>
void shared_ptr<T>::reset(T *ptr) {
  reset();
  _data = ptr;
  _counter = new std::atomic_uint(1);
}

template <class T>
void shared_ptr<T>::swap(shared_ptr &r) {
  std::swap(_counter, r._counter);
  std::swap(_data, r._data);
}

template <class T>
auto shared_ptr<T>::use_count() const -> std::size_t {
  return _counter ? static_cast<unsigned int>(*_counter) : 0;
}

#endif  // INCLUDE_SHARED_PTR_HPP_
