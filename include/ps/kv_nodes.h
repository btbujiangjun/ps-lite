// #include "./app.h"
namespace ps {

/**
 * \brief Communicate server nodes with key-value pairs
 *
 * This class provides \ref Push and \ref Pull with several variants for worker
 * nodes. See the \ref OnlineServer of server APIs
 *
 * \tparam Val the type of value, which should be primitive types such as
 * int32_t and float
 */
template<typename Val>
class KVWorker {
 public:
  /**
   * \brief Creates with an unique identity
   *
   * A worker node can have multiple \ref KVWorker. They are distinguished with
   * each other via the identities. Furthermore, it communicates with the
   * object (such as \ref OnlineServer) on a remote node with the same
   * identity. If such object does not exist on the receiver node, the system
   * will produce a fatal message.
   *
   * \param app_id the unique identity, negative IDs are preserved by system.
   */
  explicit KVWorker(int app_id) {}

  ~KVWorker() { }

  /**
   * \brief callback function
   *
   */
  using Callaback = std::function<void()>;

  /**
   * \brief Pushes a list of key-value pairs to all server nodes.
   *
   * This function pushes a KV list specified by \a keys and \a vals to all
   * server nodes. The length of a value,  which is
   * determined by `k=vals.size()/keys.size()`, can be more than one but should
   * be a constant. The \a i-th KV pair is
   *
   * \verbatim {keys[i], (vals[i*k], ..., vals[(i+1)*k-1])} \endverbatim
   *
   * This list may be sliced according to the key ranges server nodes
   * handling and be pushed to multiple servers
   *
   * This function is a non-blocking call, which returns immediately once the
   * message is queued in the system's sending buffer. The actual pushing is
   * finished only if \ref Wait returns or the callback specified in \ref
   * opts is called.
   *
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
   * @brief Extends \ref Push to dynamic length values
   *
   * This function is similar to \ref Push except that there is additional \a
   * vals_size where `vals_size[i]` stores the value length of the \a i-th KV
   * pair. In other words, assume `n = vals_size[0] + .. + vals_size[i-1]`,
   * then the \a i-th KV pair is presented as
   *
   * \verbatim {keys[i], (vals[n], ..., vals[vals_size[i]+n-1])} \endverbatim
   *
   * @param keys a list of keys, which must be sorted
   * @param vals the according values
   * @param val_lens val_lens[i] stores the value length of the \a i-th KV pair
   * @param keys a list of keys, which must be sorted
   * @param vals the according values
   * @param opts push options
   * @return the timestamp of this request
   *
   * \note Both keys and values will be copied into a system buffer, the
   * zero-copy version \ref ZPush might be more efficient
   * \note Use \ref VPush to push dynamic length values
   */
  int Push(const std::vector<Key>& keys,
           const std::vector<Val>& vals,
           const std::vector<int>& val_lens = {},
           const Callback& cb = nullptr) {
  }


  /**
   * \brief Pulls the values associated with the keys from the server nodes
   *
   * This function pulls the values of the keys specified in \a keys from the
   * server nodes.
   *
   * It's a non-blocking call, which returns immediately once the message is
   * queued in the system's sending buffer. The actual pulling is finished,
   * namely \a vals is filled with pulled values, only
   * if \ref Wait returns or the callback specified in opts is called.
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
   * @brief Extends \ref Push to dynamic length values
   *
   * This function is similar to \ref Push except that there is additional \a
   * vals_size where `vals_size[i]` stores the value length of the \a i-th KV
   * pair. In other words, assume `n = vals_size[0] + .. + vals_size[i-1]`,
   * then the \a i-th KV pair is presented as
   *
   * \verbatim {keys[i], (vals[n], ..., vals[vals_size[i]+n-1])} \endverbatim
   *
   * @param keys a list of keys, which must be sorted
   * @param vals the according values
   * @param val_lens val_lens[i] stores the value length of the \a i-th KV pair
   *
   * @param keys a list of keys, which must be sorted
   * @param vals the buffer for the pulled values. It can be empty.
   * @param opts pull options
   * @return the timestamp of this request
   *
   * \note Both keys and values will be copied into a system buffer, the
   * zero-copy version \ref ZPull might be more efficient
   * \note Use \ref VPull to pull dynamic length values
   */
  int Pull(const std::vector<Key>& keys,
           std::vector<Val>* vals,
           std::vector<int>* val_lens = nullptr,
           const Callback& cb = nullptr) {
  }


  void Wait(int timestamp) {
  }
};

/**
 *
 *
 */
template <typename Val>
class KVServer {
 public:
  KVServer(int app_id, const Handle& handle) { }

  void Response(int node_id, int timestamp,
                const KVPairs ret = KVPairs()) {
  }

  /**
   * \brief handle a push/pull request from a worker node
   *
   *
   * @param push true if this is a push request, false for pull
   * @param recved the received kv pairs
   * @param server this pointer
   */
  using Handle = std::function<void(bool push,
                                    const KVParis& recved,
                                    KVServer<Val>* server)>;


};

}  // namespace ps
