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
#define YUANLITALK_SUCCESS 0
#define YUANLITALK_ERROR   1

using namespace std;
using json = nlohmann::json;
using websocketpp::md5::md5_hash_hex;
SQLiteDB db("project.db");

char random_string_source[95] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

template <class T>
T randint(T l, T r = 0) {
  static mt19937 eng(time(NULL));
  if (l > r)
    swap(l, r);
  uniform_int_distribution<T> dis(l, r);
  return dis(eng);
}


string random_str(int n = 50) {
  string s = "";
  for (int i = 0;i < n;++i) {
    s += random_string_source[randint(0, 93)];
  }
  return s;
}

namespace UserOperation {
  json u_register(const json& user_request) {
    // sqlite3_stmt* stmt;
    const string username = user_request["username"];
    const string password = md5_hash_hex(user_request["password"]);
    const string nickname = user_request.count("nickname") ? user_request["nickname"] : "";

    PreparedStatement stmt(db, "SELECT * FROM user where username=?;");

    stmt.bind_value(1, username);

    int res;
    res = stmt.step();
    if (res == SQLITE_ROW) {
      return json({ { "status",YUANLITALK_ERROR }, { "massage","用户已存在" } });
    }


    PreparedStatement insert_user_stmt(db, "INSERT INTO user (username, password, nickname) VALUES(?,?,?);");
    insert_user_stmt.bind_value(1, username);
    insert_user_stmt.bind_value(2, password);
    insert_user_stmt.bind_value(3, nickname);

    res = insert_user_stmt.step();
    if (res == SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SUCCESS }, { "massage","创建成功" } });
    }
    else {
      return json({ { "status",YUANLITALK_ERROR }, { "massage","系统错误，创建失败" } });
    }
  }
  json u_login(const json& user_request) {
    const string username = user_request["username"];
    const string password = md5_hash_hex(user_request["password"]);
    PreparedStatement query_stmt(db, "SELECT username, password FROM user where username=?;");
    query_stmt.bind_value(1, username);
    int res;
    res = query_stmt.step();

    if (res != SQLITE_ROW) {
      return json({ { "status",YUANLITALK_ERROR }, { "massage","用户不存在或密码错误" } });
    }
    string password_in_db = query_stmt.get_result_string(1);
    if (password_in_db != password) {
      return json({ { "status",YUANLITALK_ERROR }, { "massage","用户不存在或密码错误" } });
    }
    else {
      string token = random_str();
      PreparedStatement update_stmt(db, "UPDATE user SET token = ? WHERE username = ?;");

      update_stmt.bind_value(1, token);
      update_stmt.bind_value(2, username);

      res = update_stmt.step();
      if (res != SQLITE_DONE) {
        return json({ { "status",YUANLITALK_ERROR }, { "massage","系统错误" } });
      }
      printf("token=%s\n", token.c_str());
      return json({ { "status",YUANLITALK_SUCCESS }, { "massage","登录成功" },{"token",token} });
    }
  }

  json u_sendMessage(const json& user_request) {

  }

};
void clientListener(int cfd, sockaddr_in* caddr) {
  // 管理与每个客户端的连接

  char ip[32];
  int port = ntohs(caddr->sin_port);
  inet_ntop(AF_INET, &caddr->sin_addr.s_addr, ip, sizeof(ip));
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

  // 监听客户端连接，每个客户连接开一个线程

  // int res = sqlite3_open("project.db", &db);
  // if (res != SQLITE_OK) {
  //   printf("数据库连接失败\n");
  //   pthread_exit(NULL);
  // }

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("socket没有成功建立");
    return;
  }
  sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(10086);
  saddr.sin_addr.s_addr = INADDR_ANY;
  int ret = bind(fd, (sockaddr*)&saddr, sizeof(saddr));
  if (ret == -1) {
    perror("fail to bind");
    return;
  }
  ret = listen(fd, 128);
  if (ret == -1) {
    perror("fail to listen");
    return;
  }
  while (true) {
    sockaddr_in* caddr = new sockaddr_in;
    socklen_t addrlen = sizeof(sockaddr_in);
    int cfd = accept(fd, (sockaddr*)caddr, &addrlen);
    if (cfd == -1) {
      printf("fail to accept\n");
      continue;
    }
    thread thr(clientListener, cfd, caddr);
    thr.detach();
  }

  close(fd);
}
int main() {
  thread gbListener(globalListener);
  gbListener.detach();
  pthread_exit(NULL);
}