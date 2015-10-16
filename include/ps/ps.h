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

#include "ps/internal/postoffice.h"
namespace ps {

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

}  // namespace ps
