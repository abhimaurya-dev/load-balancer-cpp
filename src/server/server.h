#ifndef SERVER_H
#define SERVER_H

#include<winsock2.h>
#include<ws2tcpip.h>
#include <string>
#include <memory>
#include "load_balancer.h"

#pragma comment(lib, "Ws2_32.lib"); 

class Server {
private:
    int port;
    SOCKET server_socket;
    LoadBalancer& loadBalancer;

    void setupSocket();
    void acceptConnections();
    std::string readHttpRequest(SOCKET client_socket);
    void sendHttpResponse(SOCKET client_socket, const std::string& response);

    Server(int port, LoadBalancer& lb);
public:
    static Server& getInstance(int port, LoadBalancer& lb);
    void start();
};
#endif