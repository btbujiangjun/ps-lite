#pragma once
#include <vector>
#include "ps/sarray.h"
#include "ps/internal/meta_message.pb.h"
namespace ps {

/**
 * \brief messages that communicated amaong nodes.
 */

struct Message {
  static const int kInvalidNode = 0;
  Message() : sender(kInvalidNode), recver(kInvalidNode) { }
  MetaMessage meta;

  std::vector<SArray<char> > data;

  template <typename V>
  void AddData(const SArray<V>& val) {

  }

  int sender;
  int recver;


  SArray<char> key;
  std::vector<SArray<char> > value;
  template <typename V>
  void AddValue(const SArray<V>& val) {

  }
};
}  // namespace ps
