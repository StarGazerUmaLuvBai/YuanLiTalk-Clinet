#include <cstdio>
#include <thread>
#include <mutex>
#include <sqlite3.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <random>
#include "json.hpp"
#include "md5.hpp"
#include "sqliteoo.hpp"
#include "socketoo.hpp"

using namespace std;
using json = nlohmann::json;
using websocketpp::md5::md5_hash_hex;


void globalListener() {
  try {
    // 监听客户端连接，每个客户连接开一个线程
    SocketObject globalSocket(10086);
    while (true) {
      Client* temp = new Client(globalSocket.accept());
    }
  }
  catch (char const* e) {
    printf("%s\n", e);
  }
}
int main() {
  PreparedStatement activete_fk(UserOperation::db, "PRAGMA foreign_keys = ON;");
  activete_fk.step();
  thread gbListener(globalListener);
  gbListener.detach();
  pthread_exit(NULL);

}