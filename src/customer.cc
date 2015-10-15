#include "ps/internal/customer.h"
#include "ps/internal/postoffice.h"
namespace ps {


Customer::Customer(int id, const Customer::RecvHandle& recv_handle)
    : id_(id), recv_handle_(recv_handle),
      recv_thread_(&Customer::Receiving, this) {
  Postoffice::Get()->AddCustomer(this);
}

Customer::~Customer() {
  Message msg; msg.terminate = true;
  recv_queue_.Push(msg);
  recv_thread_.join();
  Postoffice::Get()->RemoveCustomer(this);
}

void Customer::WaitRequest(int timestamp) {
  // std::
  std::unique_lock<std::mutex> lk(tracker_mu_);
  tracker_cond_.wait(lk, [this, timestamp]{
      return tracker_[timestamp].first == tracker_[timestamp].second;
    });
}

int Customer::NewRequest(int recver) {
  std::lock_guard<std::mutex> lk(tracker_mu_);
  int num = Postoffice::Get()->GetNodeIDs(recver).size();
  tracker_.push_back(std::make_pair(num, 0));
  return tracker_.size() - 1;
}

void Customer::Receiving() {
  while (true) {
    Message recv;
    recv_queue_.WaitAndPop(recv);
    if (recv.terminate) break;
    recv_handle_(recv);
    if (!recv.meta.request()) {
      std::lock_guard<std::mutex> lk(tracker_mu_);
      tracker_[recv.meta.timestamp()].second ++;
      tracker_cond_.notify_all();
    }
  }
}

}  // namespace ps
