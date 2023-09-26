#ifndef PARALLELS_THREAD_QUEUE_H
#define PARALLELS_THREAD_QUEUE_H

#include <queue>
#include <condition_variable>
#include <mutex>

namespace Parallels {
template<typename T>
class ThreadSafeQueue {
    private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable condition_;

    public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void Push(T new_value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(new_value);
        condition_.notify_one();
    } 

    void WaitAndPop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }

    std::shared_ptr<T> WaitAndPop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty(); });
        auto value = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        return value;
    }    

    bool TryPop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> TryPop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        auto value = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        return value;
}

    bool Empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

};
};  // namespace Parallels

#endif  // PARALLELS_THREA_QUEUE_H