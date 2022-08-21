#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include <shared_mutex>
#include <map>
#include "json.hpp"
#include "md5.hpp"
#include "sqliteoo.hpp"
#ifndef LYC_SOCKET_OO
#define LYC_SOCKET_OO

#define YUANLITALK_SUCCESS              0
#define YUANLITALK_FAILURE              1
#define YUANLITALK_SYSTEM_ERROR         2
#define YUANLITALK_WRONG_TOKEN          3

using json = nlohmann::json;
using websocketpp::md5::md5_hash_hex;
using namespace std;
class SocketObject;
class ConnectionObject;

class SocketObject {
private:
  int fd;
  sockaddr_in saddr;
public:
  SocketObject(int port, int family = AF_INET, in_addr_t address = INADDR_ANY);
  ~SocketObject();
  ConnectionObject accept();
};

class ConnectionObject {
private:
  int cfd;
  sockaddr_in caddr;
  string ip;
  int port;
public:
  ConnectionObject(int cfd, sockaddr_in caddr, const string& ip = "", int port = 0);
  ConnectionObject(const ConnectionObject&) = delete;
  ConnectionObject(ConnectionObject&& a);
  ~ConnectionObject();
  string receive();
  void send(const string& buff);
  friend class Client;
};

class Client {
private:
  int uid;
  ConnectionObject main_connection;
  string uuid;
public:
  static map<int, Client*> user_record; // 键值为uid
  static map<int, Client*> connection_record; // 键值为cfd
  static shared_mutex sh_mutex;
  void set_uid(int uid);
  string get_uuid() const;
  void set_uuid(const string& s);
  Client(ConnectionObject&& conn);
  ~Client();

  json commu_func(const json& user_request);
  void receive();
  void send(const json& s);
  static void client_thread(Client* cli);
};

map<int, Client*> Client::user_record;
map<int, Client*> Client::connection_record;
shared_mutex Client::sh_mutex;
namespace UserOperation {

  SQLiteDB db("project.db");
  shared_mutex db_mut;
  template <class T>
  T randint(T l, T r = 0) {
    static mt19937 eng(time(NULL));
    if (l > r)
      swap(l, r);
    uniform_int_distribution<T> dis(l, r);
    return dis(eng);
  }


  string random_str(int n = 50) {
    char random_string_source[95] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    string s = "";
    for (int i = 0;i < n;++i) {
      s += random_string_source[randint(0, 93)];
    }
    return s;
  }

  json u_register(const json& user_request) {
    lock_guard<shared_mutex> db_lock(db_mut);
    const string username = user_request["username"];
    const string password = md5_hash_hex(user_request["password"]);
    PreparedStatement stmt(db, "select max(uid) from user;");

    int res;
    res = stmt.step();
    if (res != SQLITE_ROW && res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    int uid = (res == SQLITE_ROW ? stmt.get_result_int(0) : 100000) + 1;
    printf("uid:%d\n", uid);
    PreparedStatement insert_user_stmt(db, "INSERT INTO user (uid,username, password) VALUES(?,?,?);");
    insert_user_stmt.bind_value(1, uid);
    insert_user_stmt.bind_value(2, username);
    insert_user_stmt.bind_value(3, password);

    res = insert_user_stmt.step();
    if (res == SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SUCCESS },{"uid",uid} });
    }
    else {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
  }
  json u_login(const json& user_request) {


    lock_guard<shared_mutex> db_lock(db_mut);
    // 有两种登录方式，一种是用密码登录，一种是用token登录（记住密码）
    const int uid = user_request["uid"];
    const string password = user_request.count("password") ? md5_hash_hex(user_request["password"]) : "";
    const string token = user_request.count("token") ? user_request["token"] : "";
    const string uuid = user_request["uuid"];
    if (password != "") {
      // 密码登录
      PreparedStatement query_stmt(db, "SELECT * FROM user where uid = ? and password = ?;");
      query_stmt.bind_value(1, uid);
      query_stmt.bind_value(2, password);
      int res;
      res = query_stmt.step();
      if (res != SQLITE_ROW) {
        return json({ { "status",YUANLITALK_FAILURE } });
      }
    }
    else {
      // token登录
      PreparedStatement query_stmt(db, "SELECT * FROM user_device where uid = ? AND uuid = ? AND token = ?;");
      printf("%d %s %s\n", uid, uuid.c_str(), token.c_str());
      query_stmt.bind_value(1, uid);
      query_stmt.bind_value(2, uuid);
      query_stmt.bind_value(3, token);
      int res;
      res = query_stmt.step();
      printf("%d\n", res);
      if (res != SQLITE_ROW) {
        return json({ { "status",YUANLITALK_FAILURE } });
      }
    }


    if (token == "") {
      string new_token = random_str();
      PreparedStatement precheck_stmt(db, "SELECT * FROM user_device WHERE uid = ? AND uuid = ?;");
      precheck_stmt.bind_value(1, uid);
      precheck_stmt.bind_value(2, uuid);
      int res;
      res = precheck_stmt.step();
      if (res == SQLITE_ROW) {
        PreparedStatement update_stmt(db, "UPDATE user_device SET token = ?, last_login=strftime('%s','now') WHERE uid = ? AND uuid = ?;");

        update_stmt.bind_value(1, new_token);
        update_stmt.bind_value(2, uid);
        update_stmt.bind_value(3, uuid);
        res = update_stmt.step();
      }
      else {
        PreparedStatement update_stmt(db, "INSERT INTO user_device(token,uid,uuid,last_login) values(?,?,?,strftime('%s','now'));");

        update_stmt.bind_value(1, new_token);
        update_stmt.bind_value(2, uid);
        update_stmt.bind_value(3, uuid);
        res = update_stmt.step();
      }
      printf("%d\n", res);

      if (res != SQLITE_DONE) {
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
      }

      return json({ { "status",YUANLITALK_SUCCESS },{"token",new_token} });
    }
    else {

      PreparedStatement update_stmt(db, "UPDATE user_device SET last_login=strftime('%s','now') WHERE uid = ? AND uuid = ?;");

      update_stmt.bind_value(1, uid);
      update_stmt.bind_value(2, uuid);

      int res = update_stmt.step();
      if (res != SQLITE_DONE) {
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
      }
      return json({ { "status",YUANLITALK_SUCCESS } });
    }
  }
  json u_sendMessage(const json& user_request) {

    int senderUid = user_request["senderUid"];
    int receiverUid = user_request["receiverUid"];
    const string message = user_request["message"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    long long sendingTime = (long long)time(NULL);
    lock_guard<shared_mutex> db_lock(db_mut);

    PreparedStatement check_legal_stmt(db, "SELECT * from user_device WHERE uid=? AND uuid=? AND token=?;");
    check_legal_stmt.bind_value(1, senderUid);
    check_legal_stmt.bind_value(2, uuid);
    check_legal_stmt.bind_value(3, token);
    int res;
    res = check_legal_stmt.step();
    if (res != SQLITE_ROW) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }

    PreparedStatement get_id_stmt(db, "select max(id) from message;");
    res = get_id_stmt.step();
    if (res != SQLITE_ROW && res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    int message_id = (res == SQLITE_ROW ? get_id_stmt.get_result_int(0) : 0) + 1;
    PreparedStatement insert_stmt(db, "INSERT INTO message(id, message, sender_uid,receiver_uid,sending_time) VALUES(?,?,?,?,?);");
    insert_stmt.bind_value(1, message_id);
    insert_stmt.bind_value(2, message);
    insert_stmt.bind_value(3, senderUid);
    insert_stmt.bind_value(4, receiverUid);
    insert_stmt.bind_value(5, sendingTime);
    res = insert_stmt.step();

    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    shared_lock<shared_mutex> lock(Client::sh_mutex);
    for (auto item : Client::user_record) {
      printf("item :%d\n", item.first);
    }
    printf("receiverUid :%d\n", receiverUid);
    if (Client::user_record.count(receiverUid)) {
      // 如果在线直接发消息
      Client::user_record[receiverUid]->send(json({ {"operation","getMessage"},{"senderUid",senderUid},{"sendingTime",sendingTime},{"message",message} }));

      printf("111111\n");
      PreparedStatement upd_stmt(db, "UPDATE user_device SET last_login = ? WHERE uid= ? and uuid = ?;");
      upd_stmt.bind_value(1, sendingTime);
      upd_stmt.bind_value(2, receiverUid);
      upd_stmt.bind_value(3, Client::user_record[receiverUid]->get_uuid());
      upd_stmt.step();
    }
    printf("222222\n");
    PreparedStatement upd_stmt(db, "UPDATE user_device SET last_login = ? WHERE uid= ? and uuid = ?;");
    upd_stmt.bind_value(1, sendingTime);
    upd_stmt.bind_value(2, senderUid);
    upd_stmt.bind_value(3, Client::user_record[senderUid]->get_uuid());
    upd_stmt.step();
    printf("444444\n");
    return json({ { "status",YUANLITALK_SUCCESS } });

  }
};





void Client::set_uid(int uid) {
  this->uid = uid;
  user_record[uid] = this;
}
string Client::get_uuid() const {
  return uuid;
}
void Client::set_uuid(const string& s) {
  uuid = s;
}
Client::Client(ConnectionObject&& conn) :main_connection(move(conn)), uid(-1) {
  connection_record[main_connection.cfd] = this;
  thread trd(client_thread, this);
  trd.detach();
}
Client::~Client() {
  if (uid != -1) {
    user_record.erase(uid);
  }
  connection_record.erase(main_connection.cfd);

}

json Client::commu_func(const json& user_request) {
  string operation = user_request["operation"];
  if (operation == "register") {
    json res = UserOperation::u_register(user_request);
    return res;
  }
  else if (operation == "login") {
    json res = UserOperation::u_login(user_request);
    if (res["status"] == YUANLITALK_SUCCESS) {
      set_uid(user_request["uid"].get<int>());
      set_uuid(user_request["uuid"].get<string>());
      printf("%d\n", (int)Client::user_record.size());
    }
    return res;
  }
  else if (operation == "sendMessage") {
    json res = UserOperation::u_sendMessage(user_request);
    return res;
  }
  return json({});
}
void Client::receive() {
  string s = main_connection.receive();
  json user_request;
  try {
    user_request = json::parse(s);
  }
  catch (json::parse_error& e) {
    // output exception information
    printf("message: %s\n", e.what());
    printf("exception id: %d\n", e.id);
    printf("byte position of error:  %d\n", (int)e.byte);
    throw "断开连接";
    return;
  }
  try {
    json return_data = commu_func(user_request);
    string s = return_data.dump();
    printf("return_data:%s\n", s.c_str());
    main_connection.send(return_data.dump());
  }
  catch (const std::exception& e) {
    throw "格式错误";
    return;
  }


}
void Client::send(const json& s) {

  main_connection.send(s.dump());
}
void  Client::client_thread(Client* cli) {
  while (true) {
    try {
      cli->receive();
    }
    catch (const char* e) {
      printf("%s\n", e);
      break;
    }
  }
  string ip = cli->main_connection.ip;
  int port = cli->main_connection.port;

  printf("%s:%d对应Client已删除\n", ip.c_str(), port);
  lock_guard<shared_mutex> lock(sh_mutex);
  delete cli;
}



SocketObject::SocketObject(int port, int family, in_addr_t address) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("socket没有成功建立");
    return;
  }
  saddr.sin_family = family;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = address;
  int ret = bind(fd, (sockaddr*)&saddr, sizeof(saddr));
  if (ret == -1) {
    throw "fail to bind";
    return;
  }
  ret = listen(fd, 128);
  if (ret == -1) {
    throw "fail to listen";
    return;
  }

}
SocketObject::~SocketObject() {
  close(fd);
}

ConnectionObject SocketObject::accept() {
  sockaddr_in caddr;
  socklen_t addrlen = sizeof(sockaddr_in);
  int cfd = ::accept(fd, (sockaddr*)&caddr, &addrlen);

  char ip[32];
  int port = ntohs(caddr.sin_port);
  inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip));
  printf("客户端的IP:%s:%d已成功连接\n", ip, port);

  if (cfd == -1) {
    throw "fail to accept\n";
  }
  return ConnectionObject(cfd, caddr, ip, port);
}

ConnectionObject::ConnectionObject(int cfd, sockaddr_in caddr, const string& ip, int port) :cfd(cfd), caddr(caddr), ip(ip), port(port) {
  printf("%s %d\n", ip.c_str(), port);
}
ConnectionObject::ConnectionObject(ConnectionObject&& a) {
  cfd = a.cfd;
  caddr = a.caddr;
  ip = a.ip;
  port = a.port;
  a.cfd = -1;
}

ConnectionObject::~ConnectionObject() {
  if (cfd != -1) {
    close(cfd);
  }
}

string ConnectionObject::receive() {
  static char buff[1024];

  int len = recv(cfd, buff, sizeof(buff), 0);
  if (len > 0) {
    buff[len] = '\0';
    printf("%d %s\n", len, buff);
    return string(buff);
  }
  else if (len == 0) {
    printf("客户端%s:%d断开连接\n", ip.c_str(), port);
  }
  else {
    printf("接受%s:%d的数据失败\n", ip.c_str(), port);
  }
  return "";
}
void ConnectionObject::send(const string& buff) {
  int len = ::send(cfd, buff.c_str(), buff.size(), 0);
  if (len < 0) {
    throw "发送失败";
  }
}


#endif