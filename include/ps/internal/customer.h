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
  Customer() { }
  Customer(int id, const RecvHandle& recv_handle);

  ~Customer();

  int id() { return id_; }

  /**
   * \brief call before issuing a request. threadsafe
   * \return return the timestamp of this request
   */
  int NewRequest(int recver);


  /**
   * \brief wait until the request is finished. threadsafe
   * \param timestamp the timestamp of the request
   */
  void WaitRequest(int timestamp);

  /**
   * \brief return the number of responses received for the request. threadsafe
   * \param timestamp the timestamp of the request
   */
  int NumResponse(int timestamp);

  /**
   * \brief
   */
  void AddResponse(int timestamp, int num = 1);

  /**
   * \brief accept a received message from \ref Van. threadsafe
   * \param recved the received the message
   */
  void Accept(const Message& recved) { recv_queue_.Push(recved); }
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
