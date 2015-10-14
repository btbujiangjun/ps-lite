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
/**
 * \brief Use unsigned 32-bit int as the key
 */
using Key = uint32_t;
#else
/**
 * \brief Use unsigned 64-bit int as the key
 */
using Key = uint64_t;
#endif

/**
 * \brief The maximal allowed key
 */
static const Key kMaxKey = std::numeric_limits<Key>::max();


void Start(int *argc, char*** argv) {

}

void Start() {

}

void Stop(const std::function<void()>& fin_cb = std::function<void()>()) {

}

}  // namespace ps
