#include "ps/ps.h"
using namespace ps;

int main(int argc, char *argv[]) {

  Start();

  if (IsServer()) {
    using Server = KVServer<float>;
    auto handle = [](const Server::ReqMeta& req_meta,
                     const KVPairs<float>& req_data,
                     Server* server) {
      LOG(ERROR) << req_data.keys;
    };
    auto server = new Server(0, handle);
    RegisterExitCallback([server](){ delete server; });
  } else {
    KVWorker<float> kv(0);

    std::vector<Key> keys = {1, 3, 5};
    std::vector<float> vals = {1, 2, 3};
    std::vector<float> rets;

    kv.Wait(kv.Push(keys, vals));
    kv.Wait(kv.Pull(keys, &rets));

    LOG(ERROR) << SArray<float>(rets);
  }

  Finalize();
  return 0;
}
