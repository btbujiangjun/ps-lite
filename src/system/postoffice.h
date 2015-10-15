#pragma once
#include "ps/base.h"
namespace ps {

class Customer;

class Postoffice {
 public:
  /**
   * \brief return the singleton object
   */
  static Postoffice* Get() {
    static Postoffice e; return &e;
  }

  /**
   * \brief add an customer to the system. threadsafe
   */
  void AddCustomer(int id, Customer* customer);

  /**
   * \brief remove a customer by given it's id. threasafe
   */
  void RemoveCustomer(int id);

  /**
   * \brief get the customer by id, threadsafe
   * \param id the customer id
   * \param timeout timeout in sec
   * \return return nullptr if doesn't exist and timeout
   */
  Customer* GetCustomer(int id, int timeout = 0);




 private:
  Postoffice();
  ~Postoffice() {

  }
  DISALLOW_COPY_AND_ASSIGN(Postoffice);

};
}  // namespace ps
