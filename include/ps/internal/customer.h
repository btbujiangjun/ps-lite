#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <thread>
#include <memory>
#include "ps/internal/message.h"
#include "ps/internal/threadsafe_queue.h"
namespace ps {

class Customer {
 public:
  using RecvHandle = std::function<void(const Message& recved)>;
  Customer(int id, const RecvHandle& recv_handle);

  ~Customer();

  int id() { return id_; }
  /**
   * \brief accept a received message
   * \param recved the received the message
   */
  void Accept(const Message& recved) { recv_queue_.Push(recved); }

  void WaitRequest(int timestamp);

  /**
   * \brief call before issuing a request
   * \return return the
   */
  int NewRequest(int recver);

 private:
  void Receiving();
  int id_;

  RecvHandle recv_handle_;
  ThreadsafeQueue<Message> recv_queue_;
  std::unique_ptr<std::thread> recv_thread_;

  std::mutex tracker_mu_;
  std::condition_variable tracker_cond_;
  std::vector<std::pair<int, int>> tracker_;

};

}  // namespace ps
