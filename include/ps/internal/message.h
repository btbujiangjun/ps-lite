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

  SArray<Key> key;
  std::vector<SArray<char> > value;

  int sender;
  int recver;

};
}  // namespace ps
