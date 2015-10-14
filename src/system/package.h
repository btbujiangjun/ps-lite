#pragma once
#include "proto/meta_message.pb.h"
#include "ps/message.h"
namespace ps {

/**
 * \brief Message with extra info
 */
struct Package {
  Package() : sender(-1), recver(-1) { }
  Package(const Message& msg) : Package() {
    if (msg.head != Message::kEmpty) {
      meta.set_head(msg.head);
    }
    if (msg.body.size()) {
      meta.set_body(msg.body);
    }
    if (msg.timestamp != Message::kEmpty) {
      meta.set_timestamp(msg.timestamp);
    }
    for (auto t : msg.value_type) {
      meta.add_value_type(t);
    }
    if (msg.key.size()) {
      meta.set_with_key(true);
      key = msg.key;
    }
    value = msg.value;
  }
  MetaMessage meta;

  SArray<Key> key;
  std::vector<SArray<char> > value;

  int sender;
  int recver;
};
}  // namespace ps
