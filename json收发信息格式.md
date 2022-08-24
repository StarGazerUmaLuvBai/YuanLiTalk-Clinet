# 消息码规定

```c++
#define YUANLITALK_SUCCESS              0
#define YUANLITALK_FAILURE              1
#define YUANLITALK_SYSTEM_ERROR         2
#define YUANLITALK_WRONG_TOKEN          3
#define YUANLITALK_NO_PERMISSION        4
#define YUANLITALK_ALREADY_EXIST        5
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
  "type":"text",
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132"
}
```
type可以是text或picture。
### 返回数据


```json
{
  "status":0
}
```
#### status

+ `YUANLITALK_SUCCESS`：发送成功
+ `YUANLITALK_FAILURE`：发送失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效
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

#### status

+ `YUANLITALK_SUCCESS`：创建成功
+ `YUANLITALK_FAILURE`：创建失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效

#### gid

创建群的id

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

### 返回数据

```json
{
  "status":0
}
```

#### status

+ `YUANLITALK_SUCCESS`：创建成功
+ `YUANLITALK_FAILURE`：创建失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效
+ `YUANLITALK_NO_PERMISSION`：没有权限（不在群内）

## sendGroupMessage

### 发送数据

```json
{
  "operation":"sendGroupMessage",
  "senderUid":100000,
  "gid":123456,
  "message":"大家好呀~",
  "type":"text",
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

#### status

+ `YUANLITALK_SUCCESS`：发送成功
+ `YUANLITALK_FAILURE`：发送失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效


## searchUser

### 发送数据

```json
{
  "operation":"searchUser",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "key":"张三"
}
或
{
  "operation":"searchUser",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "key":"100005"
}
```

### 返回数据

```json
{
  "status":0,
  "uid":[123456]
}
```
#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效

#### uid
是一个列表，返回所有用户名包含key或者uid为key的用户的uid。

## addUser

### 发送数据

```json
{
  "operation":"addUser",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "addUid":100005
}
```

### 返回数据

```json
{
  "status":0
}
```

#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效


## searchGroup

### 发送数据

```json
{
  "operation":"searchGroup",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "key":"张三"
}
或
{
  "operation":"searchUser",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "key":"100005"
}
```

### 返回数据

```json
{
  "status":0,
  "gid":[123456]
}
```
#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效

#### uid
是一个列表，返回所有用户名包含key或者uid为key的用户的uid。

## addGroup

### 发送数据

```json
{
  "operation":"addGroup",
  "uid":100000,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "addGid":100005
}
```

### 返回数据

```json
{
  "status":0
}
```

#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效

## accepetAddUser

### 发送数据

```json
{
  "operation":"accepetAddUser",
  "uid":100005,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "fromWho":100000,
  "accept":true
}
```

#### accept

一个bool值，为真代表接受申请，否则拒绝

### 返回数据

```json
{
  "status":0
}
```

#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效

## accepetAddGroup

### 发送数据

```json
{
  "operation":"accepetAddUser",
  "uid":100005,
  "gid":100001,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uuid":"F6C2CCFB-DD10-11EA-80DE-5405DB150132",
  "fromWho":100000,
  "accept":true
}
```

#### accept

一个bool值，为真代表接受申请，否则拒绝

### 返回数据

```json
{
  "status":0
}
```

#### status

+ `YUANLITALK_SUCCESS`：成功
+ `YUANLITALK_FAILURE`：失败
+ `YUANLITALK_SYSTEM_ERROR`：系统错误
+ `YUANLITALK_WRONG_TOKEN`：token信息无效


## 

# 本地json数据

## rememberedUser

用于存储所有用户的信息

```json
{
  "123456":{
    "rememberPassword":true,
    "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|"
  },
  "100001":{
    "rememberPassword":false
  }
}
```
## lastLogin

```json
{
  "rememberPassword":true,
  "token":"h-%+*592j||q5XL}X,[wPAN0t.XBtS0T:$!JiT}v%^zY(fv,Oj|",
  "uid":100005
}
```

