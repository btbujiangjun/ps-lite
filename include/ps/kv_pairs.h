#pragma once
#include "./sarray.h"
#include "ps.h"

namespace ps {
/**
 * \brief a list of key-value pairs
 * \tparam Val the value type
 */
template <typename Val>
struct KVPairs {
  KVPairs() : cmd(kEmpty) {}
  static const int kEmpty = -1;

  /**
   * \brief an int optional command.
   */
  int cmd;

  /**
   * \brief a list of keys
   */
  SArray<Key> keys;

  /**
   * \brief the according values
   */
  SArray<Val> vals;

  /**
   * \brief optional value lengths
   */

  SArray<int> val_lens;

  /**
   * \brief the sender identity of this message. it will be assigned
   */
  int sender;

  /**
   * @brief Van sends (receives) packages to (from) a node The current
   */
  int timestamp;


};

}  // namespace ps
