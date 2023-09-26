#ifndef PARALLELS_THREADS_POOLS_H
#define PARALLELS_THREADS_POOLS_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "thread_queue.h"

namespace Parallels {
class ThreadsPool {
  class JoinThreads {
   public:
    explicit JoinThreads(std::vector<std::thread>& threads)
        : threads_(threads) {}
    ~JoinThreads() {
      for (unsigned long i = 0; i < threads_.size(); ++i) {
        if (threads_[i].joinable()) {
          threads_[i].join();
        }
      }
    }

   private:
    std::vector<std::thread>& threads_;
  };

 public:
  ThreadsPool() : done_(false), joiner_(threads_) {
    unsigned const threads_count =
        std::thread::hardware_concurrency();  // think about
    try {
      for (unsigned i = 0; i < threads_count; ++i) {
        threads_.push_back(std::thread(&ThreadsPool::Run, this));
      }
    } catch (...) {
      done_ = true;
      throw std::runtime_error("ThreadsPool execution exception");
    }
  }
  ThreadsPool(unsigned const threads_count) : done_(false), joiner_(threads_) {
    // unsigned const threads_count = std::thread::hardware_concurrency(); //
    // think about
    try {
      for (unsigned i = 0; i < threads_count; ++i) {
        threads_.push_back(std::thread(&ThreadsPool::Run, this));
      }
    } catch (...) {
      done_ = true;
      throw std::runtime_error("ThreadsPool execution exception");
    }
  }

  ~ThreadsPool() { done_ = true; };

  template <typename F>
  void AddVoidTask(F func) {
    // std::cout << "AddTask " <<  std::this_thread::get_id() << std::endl;
    work_queue_.Push(std::function<void()>(func));
  }

 private:
  // std::queue<std::function<void()>> tasks_;
  // std::mutex mutex_;
  // std::condition_variable condition_;
  // bool stop{};

  std::atomic<bool> done_;
  ThreadSafeQueue<std::function<void()>> work_queue_;
  std::vector<std::thread> threads_;
  JoinThreads joiner_;

  void Run() {
    while (!done_) {
      std::function<void()> task;
      if (work_queue_.TryPop(task)) {
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }
};
};  // namespace Parallels

#endif  // PARALLELS_THREADS_POOLS_H
