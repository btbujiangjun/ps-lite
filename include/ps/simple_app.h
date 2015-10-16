#pragma once
#include "ps/internal/message.h"
#include "ps/internal/postoffice.h"
namespace ps {

/**
 * \brief a simple app communicating a pair of int (head) and string (body) with others.
 */
class SimpleApp {
 public:
  /**
   * \brief the received data, can be either a request or a response
   */
  struct RecvData {
    /** \brief the int head */
    int head;
    /** \brief the string body */
    std::string body;
    /** \brief whether or not this is a request */
    bool request;
    /** \brief sender's node id */
    int sender;
    /** \brief the associated timestamp */
    int timestamp;
  };

  /**
   * \brief the handle to proces the received data
   *
   * @param recved the received request or response
   * @param app this pointer
   */
  using RecvHandle = std::function<void(const RecvData& recved, SimpleApp* app)>;

  /**
   * \brief constructor
   *
   * @param app_id the app id, should match with the remote node app with which this app
   * is communicated
   * @param recv_handle the handle for processing the received data
   */
  SimpleApp(int app_id, const RecvHandle& recv_handle) :
      recv_handle_(recv_handle),
      obj_(app_id, [this](const Message& recv){ RunRecvHandle(recv); }) {
    CHECK(recv_handle_) << "invalid recving handle";
  }

  /** \brief deconstructor */
  virtual ~SimpleApp() { }

  /**
   * \brief send a request to a remote node
   *
   * @param req_head request head
   * @param req_body request body
   * @param recv_id remote node id
   *
   * @return the timestamp of this request
   */
  int Request(int req_head, const std::string& req_body, int recv_id) {
    // setup message
    Message msg;
    msg.meta.set_head(req_head);
    if (req_body.size()) msg.meta.set_body(req_body);
    int ts = obj_.NewRequest(recv_id);
    msg.meta.set_timestamp(ts);
    msg.meta.set_request(true);
    msg.meta.set_customer_id(obj_.id());

    // send
    for (int r : Postoffice::Get()->GetNodeIDs(recv_id)) {
      msg.recver = r;
      Postoffice::Get()->van()->Send(msg);
    }
    return ts;
  }

  /**
   * \brief wait until a request is finished
   *
   * @param timestamp
   */
  void Wait(int timestamp) { obj_.WaitRequest(timestamp); }


  /**
   * \brief send back a response for a request
   * \param req the request
   * \param the response body
   */
  void Response(const RecvData& req, const std::string& res_body = "") {
    // setup message
    Message msg;
    msg.meta.set_head(req.head);
    if (res_body.size()) msg.meta.set_body(res_body);
    msg.meta.set_timestamp(req.timestamp);
    msg.meta.set_request(false);
    msg.meta.set_customer_id(obj_.id());
    msg.recver = req.sender;

    // send
    Postoffice::Get()->van()->Send(msg);
  }

 private:
  /**
   * \brief internal handle
   */
  void RunRecvHandle(const Message& msg) {
    RecvData recv;
    recv.sender    = msg.sender;
    recv.head      = msg.meta.head();
    recv.body      = msg.meta.body();
    recv.request   = msg.meta.request();
    recv.timestamp = msg.meta.timestamp();
    recv_handle_(recv, this);
  }

  /** \brief data handle */
  RecvHandle recv_handle_;
  /** \brief ps internal object */
  Customer obj_;
};

}  // namespace ps
