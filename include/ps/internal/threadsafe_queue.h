#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
namespace ps {

/**
 * \brief thread-safe queue allowing push and waited pop
 */
template<typename T> class ThreadsafeQueue {
 public:
  void Push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(std::move(new_value));
    data_cond.notify_all();
  }

  void WaitAndPop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this]{return !data_queue.empty();});
    value = std::move(data_queue.front());
    data_queue.pop();
  }
 private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;
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
