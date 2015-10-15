#pragma once

namespace ps {


/**
 * \brief a simple app communicating <int, string> pair with others.
 */

class SimpleApp {

 public:
  explicit SimpleApp(int app_id);
  virtual ~SimpleApp() { }

  /**
   * \brief send a request to a remote node
   *
   * @param req_head request head
   * @param req_body request body
   * @param node_id remote node id
   *
   * @return the timestamp of this request
   */
  int Request(int req_head, const std::string& req_body, int node_id);

  /**
   * \brief wait until a request is finished
   *
   * @param timestamp
   */
  void Wait(int timestamp);

  /**
   * \brief the received data, either a request or a response
   */
  struct RecvData {
    int head;

    std::string body;
    /**
     * \brief whether or not this is a request
     */
    bool request;

    /**
     * \brief sender id
     */
    int sender;

    int timestamp;

  }

  /**
   * \brief send back a response for a request
   *
   */

  int Response(const RecvData& req, const std::string& res_body = "");


  /**
   * \brief handle a request or a response from a worker node
   *
   *
   * @param push true if this is a push request, false for pull
   * @param recved the received request/response
   * @param node this pointer
   */
  using Handle = std::function<void(const RecvData& recved, SimpleApp* node)>


};
}  // namespace ps
