#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "ps/base.h"
namespace ps {

/**
 * \brief thread-safe queue allowing push and waited pop
 */
template<typename T> class ThreadsafeQueue {
 public:
  ThreadsafeQueue() { }
  ~ThreadsafeQueue() { }
  void Push(T new_value) {
    mu_.lock();
    queue_.push(std::move(new_value));
    mu_.unlock();
    cond_.notify_all();
  }

  void WaitAndPop(T* value) {
    std::unique_lock<std::mutex> lk(mu_);
    cond_.wait(lk, [this]{return !queue_.empty();});
    *value = std::move(queue_.front());
    queue_.pop();
  }
 private:
  mutable std::mutex mu_;
  std::queue<T> queue_;
  std::condition_variable cond_;
};

} // namespace ps


  // bool TryPop(T& value) {
  //   std::lock_guard<std::mutex> lk(mut);
  //   if(data_queue.empty())
  //     return false;
  //   value=std::move(data_queue.front());
  //   data_queue.pop();
  //   return true;
  // }