#include "ps/ps.h"
using namespace ps;

void StartServer() {
  if (!IsServer()) return;
  using Server = KVServer<float>;
  auto handle = [](const KVMeta& req_meta,
                   const KVPairs<float>& req_data,
                   Server* server) {
    LOG(ERROR) << req_data.keys;
  };
  auto server = new Server(0);
  server->set_request_handle(handle);
  RegisterExitCallback([server](){ delete server; });
}

void RunWorker() {
  if (!IsWorker()) return;

  KVWorker<float> kv(0);

  std::vector<Key> keys = {1, 3, 5};
  std::vector<float> vals = {1, 2, 3};
  std::vector<float> rets;

  kv.Wait(kv.Push(keys, vals));
  kv.Wait(kv.Pull(keys, &rets));

  LOG(ERROR) << SArray<float>(rets);
}

int main(int argc, char *argv[]) {
  // setup server nodes
  StartServer();
  // start system
  Start();
  // run worker nodes
  RunWorker();
  // stop system
  Finalize();
  return 0;
}
