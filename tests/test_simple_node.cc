#include "ps/ps.h"
using namespace ps;

int num = 0;

void Handle(const SimpleApp::RecvData& data, SimpleApp* app) {
  if (data.request) {
    CHECK_EQ(data.head, 1);
    CHECK_EQ(data.body, "test");
    app->Response(data);
    ++ num;
  }
}

int main(int argc, char *argv[]) {
  int n = 100;
  Start();
  SimpleApp app(0, Handle);

  if (MyNode::IsScheduler()) {
    std::vector<int> ts;
    for (int i = 0; i < n; ++i) {
      int recver = kScheduler + kServerGroup + kWorkerGroup;
      ts.push_back(app.Request(1, "test", recver));
    }

    for (int t : ts) {
      app.Wait(t);
    }
  }

  Finalize();

  CHECK_EQ(num, n);
  return 0;
}
