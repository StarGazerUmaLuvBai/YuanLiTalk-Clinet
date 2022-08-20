#include <string>
#include <sqlite3.h>
#ifndef LYC_SQLITE_OO
#define LYC_SQLITE_OO
class SQLiteDB;
class PreparedStatement;
class SQLiteDB {
private:
  sqlite3* db;
  bool db_opened;
public:
  // SQLiteDB();
  SQLiteDB(const std::string& db_name);
  ~SQLiteDB();
  // void open(const std::string& db_name);
  // void close();
  friend PreparedStatement;
};

class PreparedStatement {
private:
  sqlite3_stmt* stmt;
  bool stmt_set;
  int last_exec_code;
public:
  PreparedStatement();
  PreparedStatement(const SQLiteDB& db, const std::string& sql);
  ~PreparedStatement();
  void bind_value(int index, const std::string& value);
  void bind_value(int index, const int& value);
  void bind_value(int index, const double& value);

  int step();
  int reset();
  std::string get_result_string(int index);
  int get_result_int(int index);
  double get_result_double(int index);


};

// SQLiteDB 实现
// SQLiteDB::SQLiteDB() {
//   db_opened = false;
// }
SQLiteDB::SQLiteDB(const std::string& db_name) {
  int res = sqlite3_open(db_name.c_str(), &db);
  if (res != SQLITE_OK) {
    throw "can't open file with error: " + std::string(sqlite3_errmsg(db));
  }
  db_opened = res == SQLITE_OK;
}
SQLiteDB::~SQLiteDB() {
  if (db_opened) {
    sqlite3_close(db);
    db_opened = false;
  }
}
// void SQLiteDB::open(const std::string& db_name) {
//   close();
//   int res = sqlite3_open(db_name.c_str(), &db);
//   if (res != SQLITE_OK) {
//     throw "can't open file with error: " + std::string(sqlite3_errmsg(db));
//   }
//   db_opened = res == SQLITE_OK;
// }
// void SQLiteDB::close() {
//   if (db_opened) {
//     sqlite3_close(db);
//   }
// }

// PreparedStatement 实现
PreparedStatement::PreparedStatement() {
  stmt_set = false;
}
PreparedStatement::PreparedStatement(const SQLiteDB& db, const std::string& sql) {
  sqlite3_prepare_v2(db.db, sql.c_str(), -1, &stmt, NULL);
  stmt_set = true;
}
PreparedStatement::~PreparedStatement() {
  if (stmt_set) {
    sqlite3_finalize(stmt);
  }
}
void PreparedStatement::bind_value(int index, const std::string& value) {
  sqlite3_bind_text(stmt, index, value.c_str(), -1, NULL);
}
void PreparedStatement::bind_value(int index, const int& value) {
  sqlite3_bind_int(stmt, index, value);
}
void PreparedStatement::bind_value(int index, const double& value) {
  sqlite3_bind_int(stmt, index, value);
}
int PreparedStatement::step() {
  return last_exec_code = sqlite3_step(stmt);
}
int PreparedStatement::reset() {
  return last_exec_code = sqlite3_reset(stmt);
}
std::string PreparedStatement::get_result_string(int index) {
  if (last_exec_code != SQLITE_ROW) {
    throw "db returned no data";
  }
  return std::string((const char*)sqlite3_column_text(stmt, index));
}

int PreparedStatement::get_result_int(int index) {
  if (last_exec_code != SQLITE_ROW) {
    throw "db returned no data";
  }
  return sqlite3_column_int(stmt, index);
}
double PreparedStatement::get_result_double(int index) {
  if (last_exec_code != SQLITE_ROW) {
    throw "db returned no data";
  }
  return sqlite3_column_double(stmt, index);
}
#endif