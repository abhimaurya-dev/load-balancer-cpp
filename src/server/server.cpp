#include "server.h"
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

Server::Server(int port) :  server_socket(INVALID_SOCKET) ,port(port) {}

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
Server& Server::getInstance(int port) {
  static Server instance(port);
  return instance;
}


/**
 * @brief Sets up a server socket for listening to incoming connections.
 * 
 * This function initializes the Winsock library, creates a TCP socket,
 * binds it to a specified port, and sets it to listen for incoming connections.
 * 
 * @throws std::runtime_error if Winsock initialization fails, socket creation fails,
 *         binding fails, or setting the socket to listen fails. The error message
 *         will contain the specific error code.
 */
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


/**
 * @brief Accepts incoming client connections on the server socket.
 *
 * This function enters an infinite loop, continuously listening for and accepting
 * new client connections. For each accepted connection, it retrieves the client's
 * IP address and port number, and logs the connection details to the console.
 * If an error occurs during the acceptance of a connection, an exception is thrown
 * with the error message.
 *
 * @throws std::runtime_error if accepting a connection fails.
 */
void Server::acceptConnections() {
  std::cout << "Server listening on port " << port << "..." << std::endl;
  while (true) {
    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
      std::string errMsg = "Accept connection failed(): " + std::to_string(WSAGetLastError());
      throw std::runtime_error(errMsg);
      continue;
    }
#ifdef _WIN32 
    wchar_t client_ip[INET_ADDRSTRLEN];
    DWORD dwSize = sizeof(client_ip) * sizeof(wchar_t);
    if (WSAAddressToStringW((LPSOCKADDR)&client_addr, sizeof(client_addr), NULL, client_ip, &dwSize) == 0) {
      std::cerr << "Failed to convert client IP address to string." << std::endl;
      closesocket(client_socket);
      continue;
    }
#endif
    
    unsigned short client_port = ntohs(client_addr.sin_port);
    std::cout << "New Client Connected: " << client_ip << ":" << client_port << std::endl;
    closesocket(client_socket);
  }
}

/**
 * @brief Starts the server by setting up the socket and accepting connections.
 * 
 * This function initializes the server by setting up the necessary socket
 * configurations and begins accepting incoming client connections. Once the
 * server is stopped, it performs cleanup operations for Winsock.
 */
void Server::start() {
  setupSocket();
  acceptConnections();

  // Cleanup Winsock
  closesocket(server_socket);
  WSACleanup();
}

