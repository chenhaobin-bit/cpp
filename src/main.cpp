#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>

// Windows 网络库头文件
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // 必须链接的网络库

int main() {
    // ================================= 1. 初始化网络环境（容错更强）
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[Error] Initialize Winsock failed! Code: " << WSAGetLastError() << std::endl;
        return 1;
    }
    std::cout << "[Info] Winsock initialized successfully." << std::endl;

    // ================================= 2. 创建 TCP Socket（添加端口复用）
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	std:: cout << server_socket << std::endl;
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "[Error] Create socket failed! Code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "[Info] Socket created successfully." << std::endl;

    // 关键：允许端口复用（解决“端口已被占用”问题）
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) == SOCKET_ERROR) {
        std::cerr << "[Warn] Set port reuse failed! Code: " << WSAGetLastError() << std::endl;
    }

    // ================================= 3. 配置服务器地址（IPv4 + 端口）
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;         // IPv4 协议
    server_addr.sin_port = htons(8888);       // 端口 8888（可修改为 9999 等）
    server_addr.sin_addr.s_addr = INADDR_ANY; // 绑定本机所有 IP（127.0.0.1、局域网 IP 均可）

    // ================================= 4. 绑定 Socket 与地址（详细错误提示）
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "[Error] Bind failed! Code: " << WSAGetLastError() << std::endl;
        std::cerr << "[Tips] Maybe port 8888 is used. Try modify port to 9999 (change htons(8888) to htons(9999))" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "[Info] Bind success! Server: 0.0.0.0:" << ntohs(server_addr.sin_port) << std::endl;

    // ================================= 5. 监听客户端连接（队列大小 5）
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        std::cerr << "[Error] Listen failed! Code: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "[Info] Waiting for client connection... (Press Ctrl+C to exit)" << std::endl;

    // ================================= 6. 循环接收连接 + 保持通信（核心）
    while (true) {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        // 等待客户端连接（阻塞，直到有连接接入）
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (int*)&client_addr_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "[Error] Accept connection failed! Code: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 打印客户端信息（英文避免乱码）
        std::cout << "\n[Info] Client connected!" << std::endl;
        std::cout << "[Client] IP: " << inet_ntoa(client_addr.sin_addr) << std::endl;
        std::cout << "[Client] Port: " << ntohs(client_addr.sin_port) << std::endl;

        // 给客户端发送欢迎消息（ASCII 字符，无乱码）
        const char welcome_msg[] = "Welcome to the server! Type 'exit' to disconnect.\n";
        send(client_socket, welcome_msg, strlen(welcome_msg), 0);

        // 保持与客户端的通信（循环收发数据）
        char recv_buf[1024] = {0};  // 接收缓冲区
        while (true) {
            // 接收客户端消息
            int recv_len = recv(client_socket, recv_buf, sizeof(recv_buf) - 1, 0);
            if (recv_len <= 0) {
                // 客户端断开连接或接收失败
                std::cout << "[Info] Client disconnected." << std::endl;
                break;
            }

            // 打印客户端消息
            recv_buf[recv_len] = '\0';  // 手动添加字符串结束符
            std::cout << "[Client Msg] " << recv_buf << std::endl;

            // 回复客户端（echo 功能：原样返回消息）
            std::string reply = "Server received: " + std::string(recv_buf) + "\n";
            send(client_socket, reply.c_str(), reply.size(), 0);

            // 客户端输入 exit 则断开连接
            if (strcmp(recv_buf, "exit") == 0) {
                std::cout << "[Info] Client requested to disconnect." << std::endl;
                break;
            }

            memset(recv_buf, 0, sizeof(recv_buf));  // 清空缓冲区
        }

        // 关闭当前客户端连接
        closesocket(client_socket);
    }

    // ================================= 7. 释放资源（Ctrl+C 退出时不会执行，仅作规范）
    closesocket(server_socket);
    WSACleanup();
    return 0;
}