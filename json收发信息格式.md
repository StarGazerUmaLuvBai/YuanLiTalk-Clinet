# 消息码规定

```c++
#define YUANLITALK_SUCCESS              0
#define YUANLITALK_FAILURE              1
#define YUANLITALK_SYSTEM_ERROR         2
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
接收后向用户返回register_result

## login

```json
{
  "operation":"login",
  "uid":"张三",
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

### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_SYSTEM_ERROR`：系统错误

### uid

用户的唯一身份标识。

## login_result

```json
{
  "status":0,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|"
}
```

### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：用户名或密码错误

### token

一个随机字符串，免密登陆的凭证。

当且仅当status为`YUANLITALK_SUCCESS`时存在。