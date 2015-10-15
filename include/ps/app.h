#pragma once

namespace ps {

/**
 * \brief the base class of an application in ps
 */
class App {
 public:
  explicit App(int app_id) {

  }
  virtual ~App() { }

  inline int Request(int node_id, const Message& msg) {

  }

  inline void Response(int node_id, int timestamp,
                       const Message& msg = Message()) {

  }

  inline void Wait(int timestamp) {

  }


 protected:

  /**
   * @brief Slices a message into multiple parts.
   *
   * It will be called by Submit.
   *
   * @param request the request message
   * @param krs a list of key ranges. It has length 1 if the receiver is single
   * node, otherwise, it will contain all the key ranges in this node group, and
   * they are ordered.
   * @param msgs output a list of n messages such as the msgs[i] is sent to the i-th
   * node. Each msgs[i] is already initilized by msgs[i]->task = request.task
   */
  virtual void Slice(const Message& request,
                     const std::vector<Range>& krs,
                     std::vector<Message>* msgs) {
    msgs->resize(krs.size(), request);
  }


  /**
   * @brief A user-defined function, which processes a request message received
   * from "request->sender"
   *
   * It which will be called by the executor's processing thread once the time
   * dependencies specified in "request->task" have been satisfied.
   *
   * @param request the received request
   */
  virtual bool ProcessRequest(const Message& request) {
    return true;
  }

  /**
   * @brief A user-defined function, which processes a response message received
   * from "response->sender"
   *
   * It be called by the executor's processing thread.
   *
   * @param response the received response
   */
  virtual void ProcessResponse(Message* response) { }

 private:
  int id_;
  DISALLOW_COPY_AND_ASSIGN(App);
};

}  // namespace ps
