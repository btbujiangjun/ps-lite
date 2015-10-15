/*!
 * @file   ps.h
 * \brief  The parameter server interface
 */
#pragma once
#include <functional>
#include "ps/base.h"

namespace ps {

/**
 * \brief start the system
 *
 * All nodes should call this function before using any other functions provided
 * in ps. This function will block until every nodes are started.
 */
void Start() {

}


/**
 * \brief terminate the system
 *
 * All nodes should call this function before existing. It will block until
 * every node is finalized.
 */
void Finalize() {

}

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

}  // namespace ps

#include "ps/simple_app.h"

#include "ps/node_info.h"
