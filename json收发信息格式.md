# 消息码规定

```c++
#define YUANLITALK_SUCCESS              0
#define YUANLITALK_FAILURE              1
#define YUANLITALK_SYSTEM_ERROR         2
#define YUANLITALK_WRONG_TOKEN          3
#define YUANLITALK_NO_PERMISSION        4
```

# 客户端

## register

### 发送数据
```json
{
  "operation":"register",
  "username":"张三",
  "password":"123456"
}
```

### 返回数据

```json
{
  "status":0,
  "uid":123456
}
```
#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_SYSTEM_ERROR`：系统错误

#### uid

用户的唯一身份标识。

## login

### 发送数据
```json
{
  "operation":"login",
  "uid":100000,
  "password":"123456",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```
或
```json
{
  "operation":"login",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```
#### uuid

登录设备的uuid

### 返回数据

```json
{
  "status":0,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|"
}
```

#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：用户名或密码错误
+ `YUANLITALK_SYSTEM_ERROR`：系统错误

#### token

一个随机字符串，免密登陆的凭证。

当且仅当status为`YUANLITALK_SUCCESS`且使用密码登录时存在。

## sendMessage
### 发送数据
```json
{
  "operation":"sendMessage",
  "senderUid":100000,
  "receiverUid":100001,
  "message":"hello world",
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```

### 返回数据


```json
{
  "status":0
}
```
### status

+ `YUANLITALK_SUCCESS`：发送成功
+ `YUANLITALK_FAILURE`：发送失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误

## createGroup

### 发送数据

```json
{
  "operation":"createGroup",
  "uid":100000,
  "groupName":"猿理talk开发组",
  "memberUid":[100001,100003,100005],
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```
#### memberUid

可选属性，一个加群用户列表，不需要包含创建人（群主）

### 返回数据


```json
{
  "status":0,
  "gid":123456
}
```

## inviteUser

### 发送数据

```json
{
  "operation":"inviteUser",
  "uid":100000,
  "invitedUid":100000,
  "gid":123456,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```

## sendGroupMessage

### 发送数据

```json
{
  "operation":"sendGroupMessage",
  "senderUid":100000,
  "gid":123456,
  "message":"大家好呀~"
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```