#pragma once
#include "system/message.h"
namespace ps {

class Customer {
 public:
  Customer(int id) {

  }
  virtual ~Customer() {
  }

  /**
   * \brief accept a received message
   * \param recved the received the message
   */
  void Accept(const Message& recved) {

  }



};
}  // namespace ps
