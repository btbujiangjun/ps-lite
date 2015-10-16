#pragma once
#include <limits>
#include "ps/internal/utils.h"
namespace ps {

#if USE_KEY32
/*! \brief Use unsigned 32-bit int as the key type */
using Key = uint32_t;
#else
/*! \brief Use unsigned 64-bit int as the key type */
using Key = uint64_t;
#endif

/*! \brief The maximal allowed key value */
static const Key kMaxKey = std::numeric_limits<Key>::max();

/*
 * \brief node ID for the scheduler
 */
const static int kScheduler = 1;

/**
 * \brief the server node group ID
 */
const static int kServerGroup = 2;

/**
 * \brief the worker node group ID
 */
const static int kWorkerGroup = 4;

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
    return role ? std::string(role) : "worker";
  }
};


}  // namespace ps
