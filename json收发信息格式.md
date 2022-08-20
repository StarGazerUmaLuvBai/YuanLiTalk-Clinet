# 消息码规定

```c++
#define YUANLITALK_SUCCESS 0
#define YUANLITALK_ERROR   1
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