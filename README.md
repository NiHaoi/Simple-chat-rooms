# 简易聊天室

## 简介

学了select，写了个项目练练手，打算日后多添一点功能。项目只使用了 C++ 作为开发语言。

## 注意事项

- 服务端无法通信，只能接收消息。
- 客户端可以相互通信。

## 使用指南

### 1. 克隆

使用以下命令克隆项目：

```bash
git clone https://github.com/NiHaoi/Simple-chat-rooms.git

```

### 2. 编译

使用 g++ 或 Visual Studio 编译源代码：

```bash
g++ server.cpp -o server.exe -lws232
g++ client.cpp -o client.exe -lws232
```

### 3. 运行

运行服务端和客户端：

```bash
./server.exe
./client.exe
```
