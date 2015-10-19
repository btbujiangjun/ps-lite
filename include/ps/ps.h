/*!
 * @file   ps.h
 * \brief  The parameter server interface
 */
#pragma once

/**
 * \brief basic setups and functions in ps
 */
#include "ps/base.h"

/**
 * \brief communicating with a pair of (int, string). mainly used for sending
 * command
 */
#include "ps/simple_app.h"

/**
 * \brief communcating with a list of key-value paris. mainly used for data
 * synchronization.
 */
#include "ps/kv_app.h"

namespace ps {

/** \brief Returns the number of worker nodes */
int NumWorkers() { return Postoffice::Get()->num_workers(); }

/** \brief Returns the number of server nodes */
int NumServers() { return Postoffice::Get()->num_servers(); }

/** \brief Returns the rank of this node in its group */
int MyRank() { return Postoffice::Get()->my_rank(); }

/** \brief Returns true if this node is a worker node */
bool IsWorker() { return Postoffice::Get()->is_worker(); }

/** \brief Returns true if this node is a server node. */
bool IsServer() { return Postoffice::Get()->is_server(); }

/** \brief Returns true if this node is a scheduler node. */
bool IsScheduler() { return Postoffice::Get()->is_scheduler(); }

/**
 * \brief start the system
 *
 * All nodes should call this function before using any other functions provided
 * in ps. This function will block until every nodes are started.
 */
void Start(const char* argv0 = nullptr) {
  Postoffice::Get()->Start(argv0);
}

/**
 * \brief terminate the system
 *
 * All nodes should call this function before existing. It will block until
 * every node is finalized.
 */
void Finalize() {
  Postoffice::Get()->Finalize();
}

/**
 * \brief Register a callback function to the system which is called after Finalize
 */
void RegisterExitCallback(const std::function<void()>& cb) {
  Postoffice::Get()->RegisterExitCallback(cb);
}

}  // namespace ps
