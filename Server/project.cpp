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


void clientListener(int cfd, sockaddr_in caddr) {
  // 管理与每个客户端的连接

  char ip[32];
  int port = ntohs(caddr.sin_port);
  inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip));
  printf("客户端的IP:%s:%d已成功连接\n", ip, port);
  char buff[1024];
  while (true) {

    int len = recv(cfd, buff, sizeof(buff), 0);
    if (len > 0) {

      buff[len] = '\0';
      printf("client %s say: %s\n", ip, buff);
      printf("before parse %s\n", buff);
      json user_request;
      try {
        user_request = json::parse(buff);
      }
      catch (json::parse_error& e) {
        // output exception information
        printf("message: %s\n", e.what());
        printf("exception id: %d\n", e.id);
        printf("byte position of error:  %d\n", (int)e.byte);
        continue;
      }

      printf("after parse\n");
      string res;
      if (user_request["operation"].get<string>() == "register") {
        res = UserOperation::u_register(user_request).dump();
      }
      else if (user_request["operation"].get<string>() == "login") {
        res = UserOperation::u_login(user_request).dump();
      }
      send(cfd, res.c_str(), res.length(), 0);
    }
    else if (len == 0) {
      printf("客户端%s:%d断开连接\n", ip, port);
      break;
    }
    else {
      printf("接受%s:%d的数据失败\n", ip, port);
      break;
    }
  }
}
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

  thread gbListener(globalListener);
  gbListener.detach();
  pthread_exit(NULL);

}