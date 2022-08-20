# 消息码规定

```c++
#define YUANLITALK_SUCCESS              0
#define YUANLITALK_FAILURE              1
#define YUANLITALK_SYSTEM_ERROR         2
#define YUANLITALK_REGISTER_USER_EXIST  3

```

# 客户端

## register

```json
{
  "operation":"register",
  "username":"张三",
  "password":"123456"
}
```

# 服务端

## register_result

```json
{
  "status":0,
  "uid":123456
}
```