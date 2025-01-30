#ifndef SERVER_H
#define SERVER_H

#include<winsock2.h>
#include<ws2tcpip.h>
#include <string>
#include <memory>

#pragma comment(lib, "Ws2_32.lib"); 

class Server {
  public:
    /**
     * @brief Get the singleton instance of the Server class.
     * 
     * @param port The port number on which the server will listen.
     * @return Server& Reference to the singleton instance of the Server.
     */
    static Server& getInstance(int port);

    /**
     * @brief Start the server to begin accepting connections.
     */
    void start();
  
  private:
    SOCKET server_socket; ///< The socket used by the server to listen for connections.
    int port; ///< The port number on which the server listens.

    /**
     * @brief Construct a new Server object.
     * 
     * @param port The port number on which the server will listen.
     * 
     * @note This constructor is private to enforce the singleton pattern.
     */
    Server(int port);

    // Disable copy and assignment constructor
    Server(const Server&) = delete; ///< Deleted copy constructor to prevent copying.
    Server& operator=(const Server&) = delete; ///< Deleted assignment operator to prevent assignment.

    /**
     * @brief Set up the server socket for listening.
     */
    void setupSocket();

    /**
     * @brief Accept incoming connections from clients.
     */
    void acceptConnections();
};

#endif