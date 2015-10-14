#pragma once
#include "./sarray.h"
#include "ps.h"

namespace ps {

enum DataType {
  INT8, INT16, INT32, INT64,
  UINT8, UINT16, UINT32, UINT64,
  FLOAT, DOUBLE
};

/**
 * \brief a message that is passed between nodes
 */
struct Message {
  Message() : head(kEmpty) {}
  static const int kEmpty = -1;
  /**
   * \brief an int message head, often stores a command id
   */
  int head;

  /**
   * \brief a string body, carries a small amount of data
   */
  std::string body;

  /**
   * \brief keys
   */
  SArray<Key> key;

  /**
   * \brief values
   */
  std::vector<SArray<char> > value;

  /**
   * \brief the types of value
   */
  std::vector<DataType> value_type;

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
