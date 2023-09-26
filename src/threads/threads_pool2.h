#ifndef PARALLELS_THREADS_POOL2_H
#define PARALLELS_THREADS_POOL2_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "function_wrapper.h"
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

  template <typename FunctionType>
  void AddVoidTask(FunctionType func) {
    void_work_queue_.Push(std::function<void()>(func));
  }

  template <typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type> AddTask(
      FunctionType func) {
    std::cout << "AddTask FUNCTYPE" << std::this_thread::get_id() << std::endl;
    typedef typename std::result_of<FunctionType()>::type ResultType;
    std::packaged_task<ResultType()> task(std::move(func));
    std::future<ResultType> result = task.get_future();
    work_queue_.Push(std::move(task));
    return result;
  }

 private:
  // std::queue<std::function<void()>> tasks_;
  // std::mutex mutex_;
  // std::condition_variable condition_;
  // bool stop{};

  std::atomic<bool> done_;
  ThreadSafeQueue<FunctionWrapper> work_queue_;
  ThreadSafeQueue<std::function<void()>> void_work_queue_;
  std::vector<std::thread> threads_;
  JoinThreads joiner_;

  void Run() {
    while (!done_) {
      FunctionWrapper result_task;
      std::function<void()> task;
      if (work_queue_.TryPop(result_task)) {
        result_task();
      } else if (void_work_queue_.TryPop(task)) {
        task();
      } else {
        // std::cout << "Run  std::this_thread::yield(); " <<
        // std::this_thread::get_id() << std::endl;
        std::this_thread::yield();
      }
    }
  }
};
};  // namespace Parallels

#endif  // PARALLELS_THREADS_POOL2_H
