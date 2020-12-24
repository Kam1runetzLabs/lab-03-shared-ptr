// Copyright 2020 Kam1runetzLabs <notsoserious2017@gmail.com>

#include <gtest/gtest.h>

#include <chrono>
#include <shared_ptr.hpp>
#include <thread>
#include <utility>

TEST(DefaultFeatures, DefaultConstructor) {
  shared_ptr<int> ptr;
  auto count = ptr.use_count();
  EXPECT_EQ(count, 0);
  EXPECT_FALSE(ptr);
}

TEST(DefaultFeatures, CopyingConstructor) {
  shared_ptr<int> ptr1(new int(123));
  shared_ptr<int> ptr2(ptr1);
  EXPECT_EQ(ptr1.use_count(), ptr2.use_count());
  EXPECT_EQ(ptr1.get(), ptr2.get());
}

TEST(DefaultFeatures, MovingConstructor) {
  shared_ptr<int> ptr1(new int(123));

  auto ptr1_obj = ptr1.get();
  auto ptr1_use_count = ptr1.use_count();

  shared_ptr<int> ptr2(std::move(ptr1));
  EXPECT_FALSE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 0);

  EXPECT_EQ(ptr2.get(), ptr1_obj);
  EXPECT_EQ(ptr2.use_count(), ptr1_use_count);
}

TEST(DefaultFeatures, CopyingAssignable) {
  shared_ptr<int> ptr1(new int(123));
  auto ptr2 = ptr1;
  EXPECT_EQ(ptr1.use_count(), ptr2.use_count());
  EXPECT_EQ(ptr1.get(), ptr2.get());
}

TEST(DefaultFeatures, MovingAssignable) {
  shared_ptr<int> ptr1(new int(123));

  auto ptr1_obj = ptr1.get();
  auto ptr1_use_count = ptr1.use_count();

  auto ptr2 = std::move(ptr1);
  EXPECT_FALSE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 0);

  EXPECT_EQ(ptr2.get(), ptr1_obj);
  EXPECT_EQ(ptr2.use_count(), ptr1_use_count);
}

TEST(DefaultFeatures, Scopes) {
  shared_ptr<int> ptr1(new int(123));
  {
    auto ptr2 = ptr1;
    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr1.get(), ptr2.get());
  }
  EXPECT_EQ(ptr1.use_count(), 1);
}

TEST(ThreadSafety, MultiThreading) {
  std::vector<std::thread> threads;
  std::atomic_bool stopped = false;
  shared_ptr<int> base_ptr(new int(123));

  for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back([&] {
      [[maybe_unused]] auto ptr = base_ptr;
      while (!stopped) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });
  }
  std::this_thread::sleep_for(std::chrono::seconds(5));
  EXPECT_EQ(std::thread::hardware_concurrency() + 1, base_ptr.use_count());
  stopped = true;
  for (auto &thread : threads) {
    thread.join();
  }
  EXPECT_EQ(base_ptr.use_count(), 1);
}
