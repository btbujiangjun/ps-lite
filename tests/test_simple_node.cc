#include "ps.h"

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
  ps::Start();
  ps::SimpleApp app(0, Handle);

  int n = 100;
  if (node.IsScheduler()) {
    std::vector<int> ts;
    for (int i = 0; i < n; ++i) {
      ts.push_back(app.Request(
          1, "test", kScheduler & kServerGroup & kWorkerGroup));
    }

    for (int t : ts) {
      app.Wait(t);
    }
  }
  ps::Finalize();

  CHECK_EQ(num, 100);
  return 0;
}
