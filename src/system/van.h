#pragma once
#include <unordered_map>
#include <mutex>
#include <string>
#include <thread>
#include "ps/base.h"
#include "ps/sarray.h"
#include "system/package.h"
#include "proto/node.pb.h"

namespace ps {



/**
 * @brief Van sends and receives packages
 */
class Van {
 public:
  Van();
  ~Van();

  /**
   * \brief connect to a node
   */
  void Connect(const Node& node);

  /**
   * \brief send packge,
   * \return the number of bytes sent. -1 if failed
   */
  size_t Send(const Package& pkg);

  /**
   * \brief receive a packge
   * \return the number of bytes received. -1 if failed
   */
  size_t Recv(Package* pkg);

 private:

  /**
   * return the IP address for given interface eth0, eth1, ...
   */
  void GetIP(const std::string& interface, std::string* ip);

  /**
   * \brief return the IP address and Interface the first interface which is not
   * loopback
   *
   * only support IPv4
   */
  void GetAvailableInterfaceAndIP(std::string* interface,
                                  std::string* ip);

  /**
   * \brief return an available port on local machine
   *
   * only support IPv4
   * \return 0 on failure
   */
  unsigned short GetAvailablePort();

  /**
   * \brief be smart on freeing recved data
   */
  static void FreeData(void *data, void *hint) {
    if (hint == NULL) {
      delete [] (char*)data;
    } else {
      delete (SArray<char>*)hint;
    }
  }
  // for scheduler: monitor the liveness of all other nodes
  // for other nodes: monitor the liveness of the scheduler
  void Monitoring();

  void *context_ = nullptr;
  void *receiver_ = nullptr;

  Node scheduler_;
  Node my_node_;

  /**
   * \brief the socket for sending data to remote nodes
   */
  std::unordered_map<int, void *> senders_;

  // for connection monitor
  std::unordered_map<int, int> fd_to_nodeid_;
  std::mutex fd_to_nodeid_mu_;
  std::thread* monitor_thread_;

  DISALLOW_COPY_AND_ASSIGN(Van);
};
}  // namespace ps
