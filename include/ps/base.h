#pragma once
#include <limits>
#include "glog/logging.h"

namespace ps {

#ifdef _MSC_VER
typedef signed char      int8_t;
typedef __int16          int16_t;
typedef __int32          int32_t;
typedef __int64          int64_t;
typedef unsigned char    uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <inttypes.h>
#endif

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
const static int kScheduler = 0;

/**
 * \brief the server node group ID
 */
const static int kServerGroup = 1;

/**
 * \brief the worker node group ID
 */
const static int kWorkerGroup = 2;


/*!
 * \brief Get environment variable as int with default.
 * \param key the name of environment variable.
 * \param default_val the default value of environment vriable.
 * \return The value received
 */
template<typename V>
inline V GetEnv(const char *key, V default_val) {
  const char *val = getenv(key);
  if (val == nullptr) {
    return default_val;
  } else {
    return atoi(val);
  }
}

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)
#endif

}  // namespace ps
