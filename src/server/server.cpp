#include "server.h"
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS




Server::Server(int port, LoadBalancer& lb) : port(port), server_socket(INVALID_SOCKET) , loadBalancer(lb) {}

// Helper function to read HTTP requests
std::string Server::readHttpRequest(SOCKET client_socket) {
    char buffer[4096];
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    return "";
}

// Helper function to send HTTP response
void Server::sendHttpResponse(SOCKET client_socket, const std::string& response) {
    send(client_socket, response.c_str(), response.length(), 0);
}

/**
 * @brief Retrieves the singleton instance of the Server.
 * 
 * This function returns a reference to the single instance of the Server class,
 * ensuring that only one instance is created and used throughout the application.
 * The instance is initialized with the specified port number on its first call.
 * 
 * @param port The port number to initialize the Server instance with.
 * @return Server& A reference to the singleton Server instance.
 */
Server& Server::getInstance(int port, LoadBalancer& lb) {
  static Server instance(port, lb);
  return instance;
}

void Server::setupSocket(){
  WSADATA wsaData;
  int wsaerr;
  WORD wVersionRequested = MAKEWORD(2,2);
  wsaerr = WSAStartup(wVersionRequested, &wsaData);
  if(wsaerr != 0){
    throw std::runtime_error("Winsock dll not found");
  }
  server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(server_socket == INVALID_SOCKET){
    WSACleanup();
    std::string errMsg = "Socket Error(): " + std::to_string(WSAGetLastError());
    throw std::runtime_error(errMsg);
  }

  sockaddr_in server_add{};
  server_add.sin_family = AF_INET;
  server_add.sin_addr.s_addr = INADDR_ANY;
  server_add.sin_port = htons(port);
  
  if(bind(server_socket, reinterpret_cast<sockaddr*>(&server_add), sizeof(server_add)) == SOCKET_ERROR){
    closesocket(server_socket);
    WSACleanup();
    std::string errMsg = "Socket bind failed(): " + std::to_string(WSAGetLastError());
    throw std::runtime_error(errMsg); 
  }

  if(listen(server_socket, SOMAXCONN) == SOCKET_ERROR){
    closesocket(server_socket);
    WSACleanup();
    std::string errMsg = "Socket listen failed(): " + std::to_string(WSAGetLastError());
    throw std::runtime_error(errMsg);
  }
}

// void Server::acceptConnections() {
//   std::cout << "Server listening on port " << port << "..." << std::endl;
//   while (true) {
//     sockaddr_in client_addr;
//     int client_addr_len = sizeof(client_addr);
//     SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
//     if (client_socket == INVALID_SOCKET) {
//       std::string errMsg = "Accept connection failed(): " + std::to_string(WSAGetLastError());
//       throw std::runtime_error(errMsg);
//       continue;
//     }
// #ifdef _WIN32 
//     wchar_t client_ip[INET_ADDRSTRLEN];
//     DWORD dwSize = sizeof(client_ip) * sizeof(wchar_t);
//     if (WSAAddressToStringW((LPSOCKADDR)&client_addr, sizeof(client_addr), NULL, client_ip, &dwSize) == 0) {
//       std::cerr << "Failed to convert client IP address to string." << std::endl;
//       closesocket(client_socket);
//       continue;
//     }
// #endif
    
//     unsigned short client_port = ntohs(client_addr.sin_port);
//     std::cout << "New Client Connected: " << client_ip << ":" << client_port << std::endl;
//     closesocket(client_socket);
//   }
// }

void Server::acceptConnections() {
    std::cout << "Server listening on port " << port << "..." << std::endl;
    while (true) {
        sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept connection failed(): " << WSAGetLastError() << std::endl;
            continue;
        }

        std::string clientRequest = readHttpRequest(client_socket);

        // Use LoadBalancer to get a backend server
        try {
            BackendServer selectedServer = loadBalancer.distributeRequest(); // Assuming you have a LoadBalancer instance

            // Forward the HTTP request to the backend server
            SOCKET backendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            sockaddr_in backendAddr{};
            backendAddr.sin_family = AF_INET;
            backendAddr.sin_port = htons(selectedServer.getPort());

            // Use WSAStringToAddressA to resolve the IP address
            int addrSize = sizeof(sockaddr_in);
            if (WSAStringToAddressA(
                    (LPSTR)selectedServer.getIpAddress().c_str(),
                    AF_INET,
                    nullptr,
                    (LPSOCKADDR)&backendAddr,
                    &addrSize) != 0) {
                std::cerr << "WSAStringToAddress failed(): " << WSAGetLastError() << std::endl;
                closesocket(client_socket);
                closesocket(backendSocket);
                continue;
            }
            
char ipStr[INET_ADDRSTRLEN]; // Buffer to hold the IP address string
DWORD dwSize = INET_ADDRSTRLEN;

// Assuming backendAddr is a sockaddr_in*
sockaddr_in addr = (sockaddr_in)backendAddr; // Cast to sockaddr_in

// Use WSAAddressToString to convert the address to a string
int result = WSAAddressToStringA(
    (LPSOCKADDR)&addr,            // Cast to LPSOCKADDR (sockaddr*)
    sizeof(SOCKADDR_IN),         // Size of sockaddr_in for IPv4
    NULL,                        // No filter
    ipStr,                       // Buffer to store the string
    &dwSize                      // Length of the buffer
);

if (result == 0) {
    std::cout << "IP Address: " << ipStr << std::endl;
} else {
    std::cerr << "WSAAddressToString failed with error: " << WSAGetLastError() << std::endl;
}
            // Connect to the backend server
            // std::cout<<backendAddr.sin_addr.S_un.S_addr<<std::endl;
            if (connect(backendSocket, (sockaddr*)&backendAddr, sizeof(backendAddr)) == SOCKET_ERROR) {
                std::cerr << "Failed to connect to backend server." << std::endl;
                closesocket(client_socket);
                closesocket(backendSocket);
                continue;
            }

            // Send the request to the backend server
            send(backendSocket, clientRequest.c_str(), clientRequest.length(), 0);

            // Receive the response from the backend server
            std::string backendResponse = readHttpRequest(backendSocket);
            closesocket(backendSocket);

            // Send the backend response back to the client
            sendHttpResponse(client_socket, backendResponse);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        closesocket(client_socket);
    }
}


void Server::start() {
    try {
        setupSocket();
        acceptConnections();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    // Cleanup Winsock
    if (server_socket != INVALID_SOCKET) {
        closesocket(server_socket);
    }
    WSACleanup();
}


