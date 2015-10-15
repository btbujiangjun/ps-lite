#pragma once
#include "proto/meta_message.pb.h"
#include "ps/kv_pairs.h"
namespace ps {

enum DataType {
  INT8, INT16, INT32, INT64,
  UINT8, UINT16, UINT32, UINT64,
  FLOAT, DOUBLE
};

/**
 * \brief messages that communicated amaong nodes.
 */
struct Message {
  Message() : sender(-1), recver(-1) { }
  // Message(const Message& msg) : Message() {
  //   if (msg.head != Message::kEmpty) {
  //     meta.set_head(msg.head);
  //   }
  //   if (msg.body.size()) {
  //     meta.set_body(msg.body);
  //   }
  //   if (msg.timestamp != Message::kEmpty) {
  //     meta.set_timestamp(msg.timestamp);
  //   }
  //   for (auto t : msg.value_type) {
  //     meta.add_value_type(t);
  //   }
  //   if (msg.key.size()) {
  //     meta.set_with_key(true);
  //     key = msg.key;
  //   }
  //   value = msg.value;
  // }
  MetaMessage meta;

  SArray<Key> key;
  std::vector<SArray<char> > value;

  int sender;
  int recver;

  bool terminate;
};
}  // namespace ps
