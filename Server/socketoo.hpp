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
#define YUANLITALK_NO_PERMISSION        4
#define YUANLITALK_ALREADY_EXIST        5
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
  int fixed_len_send(const char* s, int size) {
    int left = size;
    const char* p = s;
    while (left > 0) {
      int len = ::send(cfd, p, left, 0);
      if (len < 0) {
        return -1;
      }
      else if (len == 0) {
        continue;
      }
      else {
        p += len;
        left -= len;
      }
    }
    return size;
  }
  int fixed_len_receive(char* s, int size) {
    int left = size;
    char* p = s;
    while (left > 0) {
      int len = ::recv(cfd, p, left, 0);
      printf("fixed_len_receive len:%d,size:%d\n", len, size);
      if (len < 0) {
        return -1;
      }
      else if (len == 0) {
        continue;
      }
      else {
        p += len;
        left -= len;
      }
    }
    return size;
  }
public:
  ConnectionObject(int cfd, sockaddr_in caddr, const string& ip = "", int port = 0);
  ConnectionObject(const ConnectionObject&) = delete;
  ConnectionObject(ConnectionObject&& a);
  ~ConnectionObject();
  vector<char> receive();
  void send(const vector<char>& buff);
  void send(const char* buff, int len);
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
  template <class T>
  T randint(T l, T r = 0) {
    static mt19937 eng(time(NULL));
    if (l > r)
      swap(l, r);
    uniform_int_distribution<T> dis(l, r);
    return dis(eng);
  }


  string random_str(int n = 50) {
    static char random_string_source[95] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    string s = "";
    for (int i = 0;i < n;++i) {
      s += random_string_source[randint(0, 93)];
    }
    return s;
  }
  string get_username(int uid) {
    PreparedStatement stmt(db, "SELECT username from user");
    stmt.bind_value(1, uid);
    if (stmt.step() == SQLITE_ROW) {
      return stmt.get_result_string(0);
    }
    return "";
  }
  bool token_check(int uid, const string& uuid, const string& token) {
    PreparedStatement check_legal_stmt(db, "SELECT * from user_device WHERE uid=? AND uuid=? AND token=?;");
    check_legal_stmt.bind_value(1, uid);
    check_legal_stmt.bind_value(2, uuid);
    check_legal_stmt.bind_value(3, token);
    int res;
    res = check_legal_stmt.step();
    return res == SQLITE_ROW;
  }
  void update_active_time(int uid, const string& uuid, long long new_time) {
    PreparedStatement upd_stmt(db, "UPDATE user_device SET last_active_time = ? WHERE uid= ? and uuid = ?;");
    upd_stmt.bind_value(1, new_time);
    upd_stmt.bind_value(2, uid);
    upd_stmt.bind_value(3, uuid);
    upd_stmt.step();
  }
  void send_message(int senderUid, int receiverUid, long long sendingTime, const string& message, const string& file_type) {
    PreparedStatement get_id_stmt(db, "select max(id) from message;");
    int res = get_id_stmt.step();
    if (res != SQLITE_ROW && res != SQLITE_DONE) {
      throw "system error when sending message";
    }
    int message_id = (res == SQLITE_ROW ? get_id_stmt.get_result_int(0) : 0) + 1;
    PreparedStatement insert_stmt(db, "INSERT INTO message(id, message, sender_uid,receiver_uid,sending_time,type) VALUES(?,?,?,?,?,?);");
    insert_stmt.bind_value(1, message_id);
    insert_stmt.bind_value(2, message);
    insert_stmt.bind_value(3, senderUid);
    insert_stmt.bind_value(4, receiverUid);
    insert_stmt.bind_value(5, sendingTime);
    insert_stmt.bind_value(6, file_type);
    res = insert_stmt.step();

    if (Client::user_record.count(receiverUid)) {
      // 如果在线直接发消息
      Client::user_record[receiverUid]->send(json({ {"signal","getMessage"},{"senderUid",senderUid},{"sendingTime",sendingTime},{"message",message},{"type",file_type} }));
      update_active_time(receiverUid, Client::user_record[receiverUid]->get_uuid(), sendingTime);
    }
    if (senderUid) { // senderUid=0为系统消息，无需更新
      update_active_time(senderUid, Client::user_record[senderUid]->get_uuid(), sendingTime);
    }
  }
  json u_register(const json& user_request) {
    SQLTransaction transaction(db);
    const string username = user_request["username"];
    const string password = md5_hash_hex(user_request["password"]);
    PreparedStatement stmt(db, "select max(uid) from user;");

    int res;
    res = stmt.step();
    if (res != SQLITE_ROW && res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR },{ "signal","registerResult" } });
    }
    int uid = (res == SQLITE_ROW ? stmt.get_result_int(0) : 100000) + 1;
    printf("uid:%d\n", uid);
    PreparedStatement insert_user_stmt(db, "INSERT INTO user (uid,username, password) VALUES(?,?,?);");
    insert_user_stmt.bind_value(1, uid);
    insert_user_stmt.bind_value(2, username);
    insert_user_stmt.bind_value(3, password);

    res = insert_user_stmt.step();
    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } ,{ "signal","registerResult" } });
    }

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS },{"uid",uid},{ "signal","registerResult"} });
  }
  json u_login(const json& user_request) {


    SQLTransaction transaction(db);
    // 有两种登录方式，一种是用密码登录，一种是用token登录（记住密码）
    const int uid = user_request["uid"];
    const string password = user_request.count("password") ? md5_hash_hex(user_request["password"]) : "";
    const string token = user_request.count("token") ? user_request["token"] : "";
    const string uuid = user_request["uuid"];
    int res;
    if (password != "") {
      // 密码登录
      PreparedStatement query_stmt(db, "SELECT * FROM user where uid = ? and password = ?;");
      query_stmt.bind_value(1, uid);
      query_stmt.bind_value(2, password);
      res = query_stmt.step();
      if (res != SQLITE_ROW) {
        return json({ { "status",YUANLITALK_FAILURE },{ "signal","loginResult" } });
      }
    }
    else {
      // token登录
      PreparedStatement query_stmt(db, "SELECT * FROM user_device where uid = ? AND uuid = ? AND token = ?;");
      printf("%d %s %s\n", uid, uuid.c_str(), token.c_str());
      query_stmt.bind_value(1, uid);
      query_stmt.bind_value(2, uuid);
      query_stmt.bind_value(3, token);
      res = query_stmt.step();
      printf("%d\n", res);
      if (res != SQLITE_ROW) {
        return json({ { "status",YUANLITALK_FAILURE } ,{ "signal","loginResult" } });
      }
    }
    PreparedStatement get_last_active_time_stmt(db, "SELECT last_active_time FROM user_device WHERE uid = ? AND uuid = ?;");
    get_last_active_time_stmt.bind_value(1, uid);
    get_last_active_time_stmt.bind_value(2, uuid);

    res = get_last_active_time_stmt.step();
    long long last_active_time = res == SQLITE_ROW ? get_last_active_time_stmt.get_result_long_long(0) : 0;
    json j;
    if (token == "") {
      string new_token = random_str();
      PreparedStatement precheck_stmt(db, "SELECT * FROM user_device WHERE uid = ? AND uuid = ?;");
      precheck_stmt.bind_value(1, uid);
      precheck_stmt.bind_value(2, uuid);
      res = precheck_stmt.step();
      if (res == SQLITE_ROW) {
        PreparedStatement update_stmt(db, "UPDATE user_device SET token = ?, last_active_time=strftime('%s','now') WHERE uid = ? AND uuid = ?;");

        update_stmt.bind_value(1, new_token);
        update_stmt.bind_value(2, uid);
        update_stmt.bind_value(3, uuid);
        res = update_stmt.step();
      }
      else {
        PreparedStatement update_stmt(db, "INSERT INTO user_device(token,uid,uuid,last_active_time) values(?,?,?,strftime('%s','now'));");

        update_stmt.bind_value(1, new_token);
        update_stmt.bind_value(2, uid);
        update_stmt.bind_value(3, uuid);
        res = update_stmt.step();
      }

      if (res != SQLITE_DONE) {
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } ,{ "signal","loginResult" } });
      }

      j = json({ { "status",YUANLITALK_SUCCESS },{"token",new_token},{ "signal","loginResult" },{"type","passwordLogin"} });
    }
    else {

      PreparedStatement update_stmt(db, "UPDATE user_device SET last_active_time=strftime('%s','now') WHERE uid = ? AND uuid = ?;");

      update_stmt.bind_value(1, uid);
      update_stmt.bind_value(2, uuid);

      res = update_stmt.step();
      if (res != SQLITE_DONE) {
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } ,{ "signal","loginResult" } });
      }

      j = json({ { "status",YUANLITALK_SUCCESS } ,{ "signal","loginResult" },{"type","tokenLogin"} });
    }
    PreparedStatement get_offline_message(db, "SELECT message,sender_uid,receiver_uid,sending_time,type FROM message WHERE sending_time > ? AND (receiver_uid=? OR sender_uid=?);");
    get_offline_message.bind_value(1, last_active_time);
    get_offline_message.bind_value(2, uid);
    json offline_message = json::object({});
    while (get_offline_message.step() == SQLITE_ROW) {
      json temp;
      temp["message"] = get_offline_message.get_result_string(0);
      temp["senderUid"] = get_offline_message.get_result_int(1);
      temp["receiverUid"] = get_offline_message.get_result_int(2);
      temp["sendingTime"] = get_offline_message.get_result_int(3);
      temp["type"] = get_offline_message.get_result_string(4);
      string friend_id = to_string(temp["senderUid"] == uid ? temp["receiverUid"] : temp["senderUid"]);
      if (!offline_message.count(friend_id)) {
        offline_message[friend_id] = json::array();
      }
      offline_message[friend_id].push_back(temp);
    }
    PreparedStatement get_offline_group_message(db, "SELECT message,sender_uid,group_message.gid,sending_time,type FROM group_message, group_user WHERE sending_time > ? AND group_message.gid=group_user.gid AND group_user.uid=?");
    get_offline_group_message.bind_value(1, last_active_time);
    get_offline_group_message.bind_value(2, uid);

    json offline_group_message = json::object({});

    while (get_offline_group_message.step() == SQLITE_ROW) {
      json temp;
      temp["message"] = get_offline_message.get_result_string(0);
      temp["senderUid"] = get_offline_message.get_result_int(1);
      temp["gid"] = get_offline_message.get_result_int(2);
      temp["sendingTime"] = get_offline_message.get_result_int(3);
      temp["type"] = get_offline_message.get_result_string(4);

      string gid_str = to_string(temp["gid"]);
      if (!offline_group_message.count(gid_str)) {
        offline_message[gid_str] = json::array();
      }
      offline_group_message[gid_str].push_back(temp);
    }

    json friend_list = json::array({ json::object({{"uid","0"},{"username","系统消息"}}) });
    PreparedStatement get_friend_list(db, "SELECT uid2, username FROM friend,user WHERE uid1=? and uid2=user.uid;");
    get_friend_list.bind_value(1, uid);
    while (get_friend_list.step() == SQLITE_ROW) {
      json temp;
      temp["uid"] = get_friend_list.get_result_int(0);
      temp["username"] = get_friend_list.get_result_string(1);
      friend_list.push_back(temp);
    }

    json group_list = json::array({});
    PreparedStatement get_group_list(db, "SELECT 'group'.gid,'group'.groupname FROM group_user,'group' WHERE uid=? AND 'group'.gid=group_user.gid;");
    get_group_list.bind_value(1, uid);
    int t;
    while (get_group_list.step() == SQLITE_ROW) {
      json temp;
      temp["gid"] = get_group_list.get_result_int(0);
      temp["groupname"] = get_group_list.get_result_string(1);
      group_list.push_back(temp);
    }
    j["username"] = get_username(uid);
    transaction.commit();
    j["offlineMessage"] = offline_message;
    j["offlineGroupMessage"] = offline_group_message;
    j["friendList"] = friend_list;
    j["groupList"] = group_list;

    return j;
  }
  json u_sendMessage(const json& user_request) {

    int senderUid = user_request["senderUid"];
    int receiverUid = user_request["receiverUid"];
    const string message = user_request["message"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const string file_type = user_request["type"];
    long long sendingTime = (long long)time(NULL);
    SQLTransaction transaction(db);


    int res;

    if (!token_check(senderUid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }

    shared_lock<shared_mutex> lock(Client::sh_mutex);
    try {
      send_message(senderUid, receiverUid, sendingTime, message, file_type);
    }
    catch (const char* s) {
      printf("%s\n", s);
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  }
  json u_createGroup(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const string groupName = user_request["groupName"];
    int res;
    SQLTransaction transaction(db);


    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }

    PreparedStatement get_id_stmt(db, "select max(gid) from 'group';");
    res = get_id_stmt.step();
    if (res != SQLITE_ROW && res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    int gid = (res == SQLITE_ROW ? get_id_stmt.get_result_int(0) : 100000) + 1;
    PreparedStatement insert_stmt(db, "INSERT INTO 'group'(gid, groupname) VALUES(?,?);");
    insert_stmt.bind_value(1, gid);
    insert_stmt.bind_value(2, groupName);
    res = insert_stmt.step();


    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }

    insert_stmt.set_new(db, "INSERT INTO 'group_user'(gid, uid, user_role) VALUES(?,?,0);");
    insert_stmt.bind_value(1, gid);
    insert_stmt.bind_value(2, uid);

    printf("%d %d\n", gid, uid);
    res = insert_stmt.step();
    printf("%d\n", res);
    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }

    if (user_request.count("memberUid")) {
      json users = user_request["memberUid"];

      for (json user : users) {

        printf("%d\n", user.get<int>());
        insert_stmt.set_new(db, "INSERT INTO 'group_user'(gid, uid, user_role) VALUES(?,?,2);");
        insert_stmt.bind_value(1, gid);
        insert_stmt.bind_value(2, user.get<int>());
        res = insert_stmt.step();

        if (res != SQLITE_DONE) {
          return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
        }
      }
    }

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS },{"gid",gid} });
  }
  json u_inviteUser(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    int res;
    int invitedUid = user_request["invitedUid"];
    int gid = user_request["gid"];

    SQLTransaction transaction(db);
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }
    PreparedStatement is_in_group_stmt(db, "SELECT * FROM 'group_user' WHERE uid=? AND gid=?;");
    is_in_group_stmt.bind_value(1, uid);
    is_in_group_stmt.bind_value(2, gid);
    res = is_in_group_stmt.step();
    if (res != SQLITE_ROW) {
      return json({ { "status",YUANLITALK_NO_PERMISSION } });
    }
    PreparedStatement insert_stmt(db, "INSERT INTO 'group_user'(gid, uid, user_role) VALUES(?,?,2);");
    insert_stmt.bind_value(1, gid);
    insert_stmt.bind_value(2, invitedUid);
    res = insert_stmt.step();

    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  };
  json u_sendGroupMessage(const json& user_request) {
    int senderUid = user_request["senderUid"];
    int gid = user_request["gid"];
    const string message = user_request["message"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const string file_type = user_request["type"];
    long long sendingTime = (long long)time(NULL);

    SQLTransaction transaction(db);

    int res;

    if (!token_check(senderUid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }

    PreparedStatement get_id_stmt(db, "select max(id) from group_message;");
    res = get_id_stmt.step();
    if (res != SQLITE_ROW && res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    int message_id = (res == SQLITE_ROW ? get_id_stmt.get_result_int(0) : 0) + 1;
    PreparedStatement insert_stmt(db, "INSERT INTO group_message(id, message, sender_uid,gid,sending_time,type) VALUES(?,?,?,?,?,?);");
    insert_stmt.bind_value(1, message_id);
    insert_stmt.bind_value(2, message);
    insert_stmt.bind_value(3, senderUid);
    insert_stmt.bind_value(4, gid);
    insert_stmt.bind_value(5, sendingTime);
    insert_stmt.bind_value(6, file_type);
    res = insert_stmt.step();

    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
    }
    shared_lock<shared_mutex> lock(Client::sh_mutex);
    PreparedStatement get_group_users_stmt(db, "SELECT uid FROM 'group_user' WHERE gid=?;");
    get_group_users_stmt.bind_value(1, gid);
    while (get_group_users_stmt.step() == SQLITE_ROW) {
      int receiverUid = get_group_users_stmt.get_result_int(0);
      if (receiverUid == senderUid) continue;

      printf("receiverUid :%d\n", receiverUid);
      if (Client::user_record.count(receiverUid)) {
        // 如果在线直接发消息
        Client::user_record[receiverUid]->send(json({ {"signal","getGroupMessage"},{"senderUid",senderUid},{"gid",gid},{"sendingTime",sendingTime},{"message",message},{"type",file_type} }));

        update_active_time(receiverUid, Client::user_record[receiverUid]->get_uuid(), sendingTime);

      }
    }
    update_active_time(senderUid, Client::user_record[senderUid]->get_uuid(), sendingTime);
    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  }
  json u_searchUser(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const string key = user_request["uuid"];
    int res;
    SQLTransaction transaction(db);
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }
    vector<int> user_list;
    PreparedStatement serach_stmt(db, "SELECT uid FROM 'user' WHERE uid=? OR username LIKE ?;");
    serach_stmt.bind_value(1, key);
    serach_stmt.bind_value(2, "%" + key + "%");

    while (serach_stmt.step() == SQLITE_ROW) {
      user_list.push_back(serach_stmt.get_result_int(0));
    }

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } ,{"uid",user_list} });
  }
  json u_searchGroup(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const string key = user_request["key"];
    int res;

    SQLTransaction transaction(db);
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }
    vector<int> group_list;
    PreparedStatement serach_stmt(db, "SELECT gid FROM 'group' WHERE gid = ? OR groupname LIKE ?;");
    serach_stmt.bind_value(1, key);
    serach_stmt.bind_value(2, "%" + key + "%");

    while (serach_stmt.step() == SQLITE_ROW) {
      group_list.push_back(serach_stmt.get_result_int(0));
    }

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } ,{"uid",group_list} });
  }
  json u_addUser(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const int addUid = user_request["addUid"];
    long long sendingTime = (long long)time(NULL);
    SQLTransaction transaction(db);
    int res;
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }


    PreparedStatement set_new_pending_friend_stmt(db, "INSERT INTO pending_friend(uid1, uid2, sending_time) VALUES(?,?,?)");
    set_new_pending_friend_stmt.bind_value(1, uid);
    set_new_pending_friend_stmt.bind_value(2, addUid);
    set_new_pending_friend_stmt.bind_value(3, sendingTime);
    res = set_new_pending_friend_stmt.step();
    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_ALREADY_EXIST } });
    }
    shared_lock<shared_mutex> lock(Client::sh_mutex);
    if (Client::user_record.count(addUid)) {
      // 如果在线直接发消息
      Client::user_record[addUid]->send(json({ {"operation","friendRequest"},{"fromWho",uid},{"sendingTime",sendingTime} }));

      update_active_time(addUid, Client::user_record[addUid]->get_uuid(), sendingTime);
    }
    update_active_time(uid, Client::user_record[uid]->get_uuid(), sendingTime);

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  }
  json u_addGroup(const json& user_request) {

    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    const int addGid = user_request["addGid"];
    long long sendingTime = (long long)time(NULL);
    SQLTransaction transaction(db);
    int res;
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }

    PreparedStatement set_new_pending_friend_stmt(db, "INSERT INTO pending_group_member(uid, gid, sending_time) VALUES(?,?,?)");
    set_new_pending_friend_stmt.bind_value(1, uid);
    set_new_pending_friend_stmt.bind_value(2, addGid);
    set_new_pending_friend_stmt.bind_value(3, sendingTime);
    res = set_new_pending_friend_stmt.step();
    if (res != SQLITE_DONE) {
      return json({ { "status",YUANLITALK_ALREADY_EXIST } });
    }

    shared_lock<shared_mutex> lock(Client::sh_mutex);

    PreparedStatement get_group_users_stmt(db, "SELECT uid FROM 'group_user' WHERE gid=? and user_role<2;");
    get_group_users_stmt.bind_value(1, addGid);
    while (get_group_users_stmt.step() == SQLITE_ROW) {
      int admin = get_group_users_stmt.get_result_int(0);
      if (Client::user_record.count(admin)) {
        // 如果在线直接发消息
        Client::user_record[admin]->send(json({ {"operation","addGroupRequest"},{"fromWho",uid},{"sendingTime",sendingTime} }));
        update_active_time(admin, Client::user_record[admin]->get_uuid(), sendingTime);
      }
    }
    update_active_time(uid, Client::user_record[uid]->get_uuid(), sendingTime);

    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  }

  json u_accepetAddUser(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    int fromWho = user_request["fromWho"];
    bool accept = user_request["accept"];
    SQLTransaction transaction(db);
    int res;
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }
    PreparedStatement delete_request_stmt(db, "DELETE FROM pending_friend WHERE uid1=? AND uid2=?;");
    delete_request_stmt.bind_value(1, fromWho);
    delete_request_stmt.bind_value(2, uid);
    delete_request_stmt.step();
    shared_lock<shared_mutex> lock(Client::sh_mutex);

    if (accept) {
      try {
        PreparedStatement new_friend_stmt(db, "INSERT INTO friend(uid1,uid2) VALUES(?,?);");
        new_friend_stmt.bind_value(1, fromWho);
        new_friend_stmt.bind_value(2, uid);
        new_friend_stmt.step();
        new_friend_stmt.reset();
        new_friend_stmt.bind_value(1, uid);
        new_friend_stmt.bind_value(2, fromWho);
        new_friend_stmt.step();
        send_message(0, fromWho, time(NULL), to_string(uid) + "同意了你的好友申请", "text");
      }
      catch (const char* s) {
        printf("%s\n", s);
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
      }
    }
    else {
      try {
        send_message(0, fromWho, time(NULL), to_string(uid) + "拒绝了你的好友申请", "text");
      }
      catch (const char* s) {
        printf("%s\n", s);
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
      }
    }
    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  }
  json u_accepetAddGroup(const json& user_request) {
    int uid = user_request["uid"];
    const string token = user_request["token"];
    const string uuid = user_request["uuid"];
    int gid = user_request["gid"];
    int fromWho = user_request["fromWho"];
    bool accept = user_request["accept"];
    SQLTransaction transaction(db);
    int res;
    if (!token_check(uid, uuid, token)) {
      return json({ { "status",YUANLITALK_WRONG_TOKEN } });
    }
    PreparedStatement check_is_admin(db, "SELECT uid FROM 'group_user' WHERE gid=? AND uid=? AND user_role<2;");
    check_is_admin.bind_value(1, gid);
    check_is_admin.bind_value(2, uid);
    res = check_is_admin.step();
    if (res != SQLITE_ROW) {
      return json({ { "status",YUANLITALK_NO_PERMISSION } });
    }

    PreparedStatement delete_request_stmt(db, "DELETE FROM pending_group_member WHERE uid=? AND gid=?;");
    delete_request_stmt.bind_value(1, fromWho);
    delete_request_stmt.bind_value(2, gid);
    delete_request_stmt.step();
    shared_lock<shared_mutex> lock(Client::sh_mutex);

    if (accept) {
      try {
        PreparedStatement new_member_stmt(db, "INSERT INTO group_user(gid,uid) VALUES(?,?);");
        new_member_stmt.bind_value(1, gid);
        new_member_stmt.bind_value(2, fromWho);
        new_member_stmt.step();
        send_message(0, fromWho, time(NULL), "你已进入群聊" + to_string(gid), "text");
      }
      catch (const char* s) {
        printf("%s\n", s);
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
      }
    }
    else {
      try {
        send_message(0, fromWho, time(NULL), to_string(uid) + "群" + to_string(gid) + "拒绝了你", "text");
      }
      catch (const char* s) {
        printf("%s\n", s);
        return json({ { "status",YUANLITALK_SYSTEM_ERROR } });
      }
    }
    transaction.commit();
    return json({ { "status",YUANLITALK_SUCCESS } });
  }
}




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
    }
    return res;
  }
  else if (operation == "sendMessage") {
    json res = UserOperation::u_sendMessage(user_request);
    return res;
  }
  else if (operation == "createGroup") {
    json res = UserOperation::u_createGroup(user_request);
    return res;
  }
  else if (operation == "inviteUser") {
    json res = UserOperation::u_inviteUser(user_request);
    return res;
  }
  else if (operation == "sendGroupMessage") {
    json res = UserOperation::u_sendGroupMessage(user_request);
    return res;
  }
  else if (operation == "searchUser") {
    json res = UserOperation::u_searchUser(user_request);
    return res;
  }
  else if (operation == "searchGroup") {
    json res = UserOperation::u_searchGroup(user_request);
    return res;
  }
  else if (operation == "addUser") {
    json res = UserOperation::u_addUser(user_request);
    return res;
  }
  else if (operation == "accepetAddUser") {
    json res = UserOperation::u_accepetAddUser(user_request);
    return res;
  }
  else if (operation == "accepetAddGroup") {
    json res = UserOperation::u_accepetAddGroup(user_request);
    return res;
  }

  return json({});
}
void Client::receive() {
  vector<char> recv_data;
  string s;
  try {
    recv_data = main_connection.receive();

    s = string(recv_data.data(), recv_data.size());
  }
  catch (const string& e) {
    printf("%s\n", e.c_str());
  }

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

vector<char> ConnectionObject::receive() {
  // char buff[1024];
  unsigned len = 0;
  fixed_len_receive((char*)&len, sizeof(int));
  len = ntohl(len);
  printf("ntohl(len):%d\n", len);
  vector<char> buff;
  buff.resize(len);
  int res = fixed_len_receive(buff.data(), len);
  printf("datalen: %d\n", len);
  if (res > 0) {
    buff.resize(len + 1);
    buff[len] = '\0';
    printf("%d %s\n", len, buff.data());
    buff.resize(len);
  }
  else if (len == 0) {
    string e = "客户端" + ip + ":" + to_string(port) + "断开连接";
    // printf("客户端%s:%d断开连接\n", ip.c_str(), port);
    throw e;
  }
  else {
    string e = "接收" + ip + ":" + to_string(port) + "的数据失败";
    throw e;
  }
  return buff;
}
void ConnectionObject::send(const char* s, int len) {
  unsigned len_1 = htonl(len);
  char* buff = new char[len + 4];
  printf("No I am sending %d\n", len);
  memcpy(buff, &len_1, 4);
  memcpy(buff + 4, s, len);
  printf("%s\n", buff + 4);
  int res = fixed_len_send(buff, len + 4);
  delete buff;
  if (res < 0) {
    throw "发送失败";
  }
}
void ConnectionObject::send(const vector<char>& buff) {
  send(buff.data(), buff.size());
}

void ConnectionObject::send(const string& s) {
  send(s.c_str(), s.size());
}

#endif