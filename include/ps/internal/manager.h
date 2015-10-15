#pragma once
#include "ps/internal/message.h"
namespace ps {

class Manager {
 public:

  /**
   * \brief get the id of a node (group)
   *
   * if it is a  node group, return the list of node ids in this
   * group. otherwise, return {node_id}
   */
  const std::vector<int>& GetNodeIDs(int node_id) {

  }

  /**
   * \brief get the key ranges of a node (group)
   */
  const std::vector<Range>& GetKeyRanges(int node_id) {

  }

  /**
   * \brief accept a control message from a remote node
   */
  void Accept(const Message& msg) {

  }


};
}  // namespace ps
