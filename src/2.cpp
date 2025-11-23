#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
int main()
{
    WSADATA WSAData;
    //启动winsock服务
    if(WSAStartup(MAKEWORD(2,2),&WSAData) != 0)
    {
        std::cerr << "WRONG" << std::endl;
        return 1;
    }
    std::cout<<"成功了"<< std::endl;
    //创建socket
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket == INVALID_SOCKET)
    {
        std::cerr<<"WRONG"<< std::endl;
        WSACleanup();
        return 1;
    }
    std::cout <<"成立" << std::endl;

    //配置服务器
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr) == SOCKET_ERROR)
    {
        std::cerr << "WRONG" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout <<"成立" << std::endl;
    
}