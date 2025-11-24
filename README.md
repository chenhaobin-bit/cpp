极简用户认证 HTTP 服务
项目介绍
这是一个基于 C++ 开发的轻量级 HTTP 后端服务，核心提供 用户注册、登录、健康检查 三大功能，采用 cpp-httplib 实现 HTTP 通信，nlohmann/json 处理 JSON 数据，支持多线程安全访问，适合作为前端开发测试、小型工具后端或学习 C++ 网络编程的示例项目。
技术栈
核心框架：cpp-httplib（轻量级 HTTP 服务器库）
JSON 解析：nlohmann/json（C++ 常用 JSON 处理库）
开发环境：MinGW-w64（Windows）/ GCC（Linux/macOS）
线程安全：std::mutex + std::lock_guard
线程安全：使用互斥锁保护用户数据，支持多并发请求
数据校验：严格校验请求体 JSON 格式及必填字段
轻量无依赖：仅依赖两个头文件库，编译部署简单
本地运行：默认绑定本地地址 127.0.0.1:11451，安全可控
下载依赖库（放入项目 include 目录）：
cpp-httplib：仅需 httplib.h
nlohmann/json：仅需 json.hpp
编译环境：
Windows：MinGW-w64（推荐 12.2.0 及以上）
D:/cpp1/
│
├── src/
│   └── 3.cpp
│
└── include/
    └── nlohmann/
        └── json.hpp
编译命令 g++ ./src/3.cpp -o server.exe -I include -DCPPHTTPLIB_NO_MMAP -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 -lws2_32
运行服务 ./server.exe
1. 健康检查（/hello）
方式 1：浏览器访问
直接打开浏览器输入：http://127.0.0.1:11451/hello，页面显示 hello 即正常。
方式 2：命令行（PowerShell）
powershell
curl http://127.0.0.1:11451/hello
2. 用户注册（/register）
命令行（PowerShell）
powershell
curl -Method Post -Uri http://127.0.0.1:11451/register `
-Header @{"Content-Type"="application/json"} `
-Body '{"name":"zhangsan","password":"123456","info":"我是张三，喜欢编程"}'
成功响应：Content: ok
失败响应：Content: failed（原因：JSON 格式错误、缺少必填字段）
3. 用户登录（/login）
命令行（PowerShell）
powershell
curl -Method Post -Uri http://127.0.0.1:11451/login `
-Header @{"Content-Type"="application/json"} `
-Body '{"name":"zhangsan","password":"123456"}'
成功响应（JSON）：
json
{"status":"ok","info":"我是张三，喜欢编程"}
失败响应：Content: failed（原因：用户名不存在、密码错误、JSON 格式错误）
