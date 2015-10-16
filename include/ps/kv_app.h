#include "ps/base.h"
#include "ps/internal/customer.h"
namespace ps {

/**
 * \brief A worker node that can \ref Push (\ref Pull) key-value pairs to (from) server
 * nodes
 *
 * \tparam Val the type of value, which should be primitive types such as
 * int32_t and float
 */
template<typename Val>
class KVWorker {
 public:
  /**
   * \brief constructor
   *
   * \param app_id the app id, should match with \ref KVServer's id
   */
  explicit KVWorker(int app_id) {}

  /**
   * \brief deconstructor
   */
  ~KVWorker() { }

  /**
   * \brief callback function for \ref Push and \ref Pull
   *
   * It is called by the data receiving thread of this instance when the push or
   * pull is actually finished. Namely the kv pairs have already written into
   * servers' data structure or the kv pairs have already pulled back.
   */
  using Callaback = std::function<void()>;

  /**
   * \brief Pushes a list of key-value pairs to all server nodes.
   *
   * This function pushes a KV list specified by \a keys and \a vals to all
   * server nodes. The keys must be unique and sorted in a increasing order.
   * The length of a value can be more than one. If \a val_lens is
   * not specified, then the length of a value is determined by
   * `k=vals.size()/keys.size()`.  The \a i-th KV pair is then
   *
   * \verbatim {keys[i], (vals[i*k], ..., vals[(i+1)*k-1])} \endverbatim
   *
   * Sample usage: the following codes push two KV pairs `{1, (1.1, 1.2)}` and `{3,
   * (3.1,3.2)}` to server nodes, where the value is a length-2 float vector
   * \code
   *   KVWorker<float> w;
   *   std::vector<Key> keys = {1, 3};
   *   std::vector<float> vals = {1.1, 1.2, 3.1, 3.2};
   *   w.Push(keys, vals);
   * \endcode
   *
   * The value length can be various, in that case \a val_lens should be
   * given. And the following format is assumed. Let

   * \verbatim n = val_lens[0] + .. + val_lens[i-1]  \endverbatim
   *
   * then the \a i-th KV pair is presented as
   *
   * \verbatim {keys[i], (vals[n], ..., vals[val_lens[i]+n-1])} \endverbatim
   *
   * The KV list is partitioned and sent based on the key range each server
   * maintaining. This function returns without waiting the data are sent
   * actually. Instead, use either \ref Wait or the callback to know when
   * finished. This function is thread-safe.
   *
   * @param keys a list of keys, must be unique and sorted in increasing order
   * @param vals the according values
   * @param val_lens optional, val_lens[i] stores the value length of the \a
   * i-th KV pair
   * @param cmd an optional command sent to the servers
   * @param cb the callback which is called when the push is finished.
   * @return the timestamp of this request
   */
  int Push(const std::vector<Key>& keys,
           const std::vector<Val>& vals,
           const std::vector<int>& val_lens = {},
           int cmd = 0,
           const Callback& cb = nullptr) {
  }


  /**
   * \brief Pulls the values associated with the keys from the server nodes
   *
   * This function pulls the values of the keys specified in \a keys from the
   * server nodes. The data layout is similar to \ref Push.
   *
   * Sample usage: the following codes pull the values of keys \a 1 and \a 3
   * from the server nodes.
   * \code
   *   KVWorker<float> w;
   *   std::vector<Key> keys = {1, 3};
   *   std::vector<float> vals;
   *   ps.Pull(keys, &vals);
   * \endcode
   *
   * It's a non-blocking call. The actual pulling is finished,
   * namely \a vals (and \a val_lens) is filled with pulled values, only
   * if \ref Wait returns or the callback is called.
   *
   * @param keys a list of keys, must be unique and sorted in increasing order
   * @param vals the buffer for the pulled values. It can be 0 size.
   * @param val_lens optional buffer for the value length. If set, it can be 0 size.
   * @param cmd an optional command sent to the servers
   * @param cb the callback which is called when the pull is finished.
   * @return the timestamp of this request
   */
  int Pull(const std::vector<Key>& keys,
           std::vector<Val>* vals,
           std::vector<int>* val_lens = nullptr,
           int cmd = 0,
           const Callback& cb = nullptr) {
  }


  /**
   * \brief Waits until a push or pull has been finished
   *
   * Sample usage:
   * \code
   *   int ts = w.Pull(keys, &vals);
   *   Wait(ts);
   *   // now vals is ready for use
   * \endcode
   *
   * \param timestamp the timestamp returned by the push or pull
   */
  void Wait(int timestamp) {

  }


  /**
   * \brief zero-copy Push
   *
   * This function is similar to \ref Push except that all data
   * will not be copied into system for better performance. It is the caller's
   * responsibility to keep the content to be not changed before actually
   * finished.
   */
  int ZPush(const SArray<Key>& keys,
           const SArray<Val>& vals,
           const SArray<int>& val_lens = {},
           int cmd = 0,
           const Callback& cb = nullptr) {
  }


  /**
   * \brief zero-copy Pull
   *
   * This function is similar to \ref Pull except that all data
   * will not be copied into system for better performance. It is the caller's
   * responsibility to keep the content to be not changed before actually
   * finished.
   */
  int ZPush(const SArray<Key>& keys,
           const SArray<Val>* vals,
           const SArray<int>* val_lens = nullptr,
           int cmd = 0,
           const Callback& cb = nullptr) {
  }

 private:

};

/**
 * \brief the received key-value pairs, with additional info
 */
struct RecvKVs {
  /** \brief the list of keys */
  SArray<Key> keys;
  /** \brief the according values */
  SArray<Key> vals;
  /** \brief the according value lengths (could be empty) */
  SArray<Key> val_lens;
  /** \brief the int cmd */
  int cmd;
  /** \brief whether or not this is a push request */
  bool push;
  /** \brief sender's node id */
  int sender;
  /** \brief the associated timestamp */
  int timestamp;
};

/**
 * \brief A server node for maintaining key-value pairs
 *
 */
template <typename Val>
class KVServer {
 public:
  /**
   * \brief the handle to process a push/pull request from a worker
   * \param recved one request received from one worker
   * \param server this pointer
   */
  using ReqHandle = std::function<void(const RecvKVs& recved, KVServer* server)>;

  /**
   * \brief constructor
   * \param app_id the app id, should match with \ref KVWorker's id
   * \param req_handle the handle for processing a request
   */
  KVServer(int app_id, const ReqHandle& req_handle) { }

  /**
   * \brief response to the push/pull request
   * \param req the request received from the worker
   * \param res the response that will send back to the worker
   */
  void Response(const RecvKVs req, const RecvKVs res = RecvKVs()) {

  }
 private:

};



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
}  // namespace ps
