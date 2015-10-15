#include "ps/internal/postoffice.h"
#include "ps/internal/message.h"
#include "ps/node_info.h"

namespace ps {

void Postoffice::Start() {
  // init node info.
  for (int i = 0; i < NumWorkers(); ++i) {
    int id = WorkerRankToID(i);
    for (int g : {id, kWorkerGroup, kWorkerGroup & kServerGroup,
            kWorkerGroup & kScheduler,
            kWorkerGroup & kServerGroup & kScheduler}) {
      node_ids_[g].push_back(id);
    }
  }

  for (int i = 0; i < NumServers(); ++i) {
    int id = ServerRankToID(i);
    for (int g : {id, kServerGroup, kWorkerGroup & kServerGroup,
            kServerGroup & kScheduler,
            kWorkerGroup & kServerGroup & kScheduler}) {
      node_ids_[g].push_back(id);
    }
  }

  van_->Start();
  Barrier(kWorkerGroup & kServerGroup & kScheduler);
}

void Postoffice::Finalize() {
  van_->Stop();
}


void Postoffice::AddCustomer(Customer* customer) {
  std::lock_guard<std::mutex> lk(mu_);
  int id = CHECK_NOTNULL(customer)->id();
  CHECK_EQ(customers_.count(id), (size_t)0) << "id " << id << " already exists";
  customers_[id] = customer;
}


void Postoffice::RemoveCustomer(Customer* customer) {
  std::lock_guard<std::mutex> lk(mu_);
  int id = CHECK_NOTNULL(customer)->id();
  customers_.erase(id);
}


Customer* Postoffice::GetCustomer(int id, int timeout) const {
  Customer* obj = nullptr;
  for (int i = 0; i < timeout*1000+1; ++i) {
    {
      std::lock_guard<std::mutex> lk(mu_);
      const auto it = customers_.find(id);
      if (it != customers_.end()) {
        obj = it->second;
        break;
      }
    }
    usleep(1000);
  }
  return obj;
}

void Postoffice::Barrier(int node_group) {
  std::lock_guard<std::mutex> lk(mu_);

  if (GetNodeIDs(node_group).size() <= 1) return;
  auto role = van_->my_node().role();
  if (role == Node::SCHEDULER) {
    CHECK(node_group & kScheduler);
  } else if (role == Node::WORKER) {
    CHECK(node_group & kWorkerGroup);
  } else if (role == Node::SERVER) {
    CHECK(node_group & kServerGroup);
  }

  std::unique_lock<std::mutex> ulk(barrier_mu_);
  barrier_done_ = false;
  Message req;
  req.recver = kScheduler;
  req.meta.set_request(true);
  req.meta.mutable_control()->set_cmd(Control::BARRIER);
  req.meta.mutable_control()->set_barrier_group(node_group);
  CHECK_GT(van_->Send(req), 0);

  barrier_cond_.wait(ulk, [this] {
      return barrier_done_;
    });
}

void Postoffice::Manage(const Message& recv) {
  CHECK(recv.meta.has_control());
  const auto& ctrl = recv.meta.control();
  if (ctrl.cmd() == Control::BARRIER) {
    if (recv.meta.request()) {
      if (barrier_count_.empty()) {
        barrier_count_.resize(8);
      }
      CHECK(ctrl.has_barrier_group());
      int group = ctrl.barrier_group();
      ++ barrier_count_[group];
      if (barrier_count_[group] == (int)GetNodeIDs(group).size()) {
        Message res;
        res.meta.set_request(false);
        res.meta.mutable_control()->set_cmd(Control::BARRIER);
        for (int r : GetNodeIDs(group)) {
          res.recver = r;
          CHECK_GT(van_->Send(res), 0);
        }
        barrier_count_[group] = 0;
      }
    } else {
      barrier_mu_.lock();
      barrier_done_ = true;
      barrier_mu_.unlock();
    }
  }
}
}  // namespace ps
