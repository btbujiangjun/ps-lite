#pragma once
#include <mutex>
#include "ps/range.h"
#include "ps/internal/customer.h"
#include "ps/internal/van.h"
namespace ps {


class Postoffice {
 public:
  /**
   * \brief return the singleton object
   */
  static Postoffice* Get() {
    static Postoffice e; return &e;
  }

  Van* van() { return van_; }

  void Start(const char* argv0);

  void Finalize();

  /**
   * \brief add an customer to the system. threadsafe
   */
  void AddCustomer(Customer* customer);

  /**
   * \brief remove a customer by given it's id. threasafe
   */
  void RemoveCustomer(Customer* customer);


  /**
   * \brief get the customer by id, threadsafe
   * \param id the customer id
   * \param timeout timeout in sec
   * \return return nullptr if doesn't exist and timeout
   */
  Customer* GetCustomer(int id, int timeout = 0) const;

  /**
   * \brief get the id of a node (group), threadsafe
   *
   * if it is a  node group, return the list of node ids in this
   * group. otherwise, return {node_id}
   */
  const std::vector<int>& GetNodeIDs(int node_id) const {
    const auto it = node_ids_.find(node_id);
    CHECK(it != node_ids_.cend()) << "node " << node_id << " doesn't exist";
    return it->second;
  }

  /**
   * \brief return the key ranges of all server nodes
   */

  const std::vector<Range>& GetServerKeyRanges() {

  }


  static inline int WorkerRankToID(int rank) {
    return rank * 2 + 9;
  }

  static inline int ServerRankToID(int rank) {
    return rank * 2 + 8;
  }

  static inline int IDtoRank(int id) {
    return std::max((id - 8) / 2, 0);
  }


  int num_workers() const { return num_workers_; }

  int num_servers() const { return num_servers_; }

  int rank() const { return IDtoRank(van_->my_node().id()); }

  int is_worker() const { return is_worker_; }

  int is_server() const { return is_server_; }

  int is_scheduler() const { return is_scheduler_; }


  void Barrier(int node_id);



  void Manage(const Message& recv);

 private:
  Postoffice();
  ~Postoffice() { delete van_; }
  Van* van_;
  mutable std::mutex mu_;
  DISALLOW_COPY_AND_ASSIGN(Postoffice);

  std::unordered_map<int, Customer*> customers_;
  std::unordered_map<int, std::vector<int>> node_ids_;

  bool is_worker_, is_server_, is_scheduler_;
  int num_servers_, num_workers_;

  bool barrier_done_;
  std::mutex barrier_mu_;
  std::condition_variable barrier_cond_;

};
}  // namespace ps
