// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

template <class T>
class SharedPtr {
 public:
  SharedPtr();
  SharedPtr(T *ptr);
  SharedPtr(const SharedPtr &r);
  SharedPtr(SharedPtr &&rr);
  ~SharedPtr();

  auto operator=(const SharedPtr &r) -> SharedPtr &;
  auto operator=(SharedPtr &&rr) -> SharedPtr &;

  operator bool() const;
  auto operator*() const -> T &;
  auto operator->() const -> T *;

  auto get() -> T *;
  void reset();
  void reset(T *ptr);
  void swap(SharedPtr &r);

  auto use_count() const -> std::size_t;
};

#endif  // INCLUDE_HEADER_HPP_
