/*!
 * @file   ps.h
 * \brief  The parameter server interface
 */
#pragma once
#include <limits>
#include <functional>
#include "ps/base.h"

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

}  // namespace ps

#include "ps/simple_app.h"
