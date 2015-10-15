/*!
 * @file   ps.h
 * \brief  The parameter server interface
 */
#pragma once
#include <functional>
#include "ps/base.h"
#include "ps/internal/postoffice.h"
namespace ps {

/**
 * \brief start the system
 *
 * All nodes should call this function before using any other functions provided
 * in ps. This function will block until every nodes are started.
 */
void Start() {
  Postoffice::Get()->Start();
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

#include "ps/simple_app.h"

#include "ps/node_info.h"
