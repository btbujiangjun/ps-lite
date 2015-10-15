#pragma once
#include <string>
#include "ps/base.h"
namespace ps {

/**
 * \brief Returns the number of worker nodes
 */
inline int NumWorkers() {
  return GetEnv("DMLC_NUM_WORKER", 1);
}

/**
 * \brief Returns the number of server nodes
 */
inline int NumServers() {
  return GetEnv("DMLC_NUM_SERVER", 0);
}

/**
 * \brief query info about my node
 */
class MyNode {
 public:
  /**
   * \brief Returns the rank of this node in its group, which is in {0, ...,
   * \ref GroupSize() - 1} .
   */
  static int Rank() {
    return GetEnv("DMLC_RANK", 0);
  }

  /**
   * \brief Returns the group size
   */
  static int GroupSize() {
    return IsWorker() ? NumWorkers() : (IsServer() ? NumServers() : 1);
  }

  /**
   * \brief Returns true if this node is a worker node
   */
  static bool IsWorker() { return GetRole() == "worker"; }

  /**
   * \brief Returns true if this node is a server node.
   */
  static inline int IsServer() { return GetRole() == "server"; }

  /**
   * \brief Returns true if this node is a scheduler node.
   */
  static int IsScheduler() { return GetRole() == "scheduler"; }

 private:
  static std::string GetRole() {
    const char* role = getenv("DMLC_ROLE");
    if (role) {
      return std::string(role);
    } else {
      return "worker";
    }
  }
};

}  // namespace ps
