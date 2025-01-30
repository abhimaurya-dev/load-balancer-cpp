#ifndef BACKEND_SERVER_H
#define BACKEND_SERVER_H

#include<string>

class BackendServer{
  private:
    std::string ipAddress; ///< The IP address of the backend server.
    int port; ///< The port number on which the backend server is running.
    int activeConnections; ///< The number of active connections to the backend server.
    bool isAvailable; ///< The availability status of the backend server.
  
  public:
    /**
     * @brief Constructs a new BackendServer object with the specified IP address and port.
     * 
     * @param ip The IP address of the backend server.
     * @param port The port number on which the backend server is running.
     */
    BackendServer(const std::string& ip, int port);

    // Getters

    /**
     * @brief Retrieves the IP address of the backend server.
     * 
     * @return The IP address as a string.
     */
    std::string getIpAddress() const;

    /**
     * @brief Retrieves the port number of the backend server.
     * 
     * @return The port number as an integer.
     */
    int getPort() const;

    /**
     * @brief Retrieves the number of active connections to the backend server.
     * 
     * @return The number of active connections as an integer.
     */
    int getActiveConnections() const;

    /**
     * @brief Checks the availability status of the backend server.
     * 
     * @return True if the server is available, false otherwise.
     */
    bool getAvailability() const;

    // Setters

    /**
     * @brief Sets the availability status of the backend server.
     * 
     * @param availability The new availability status.
     */
    void setAvailability(bool availability);

    /**
     * @brief Increments the count of active connections by one.
     */
    void incrementConnections();

    /**
     * @brief Decrements the count of active connections by one.
     */
    void decrementConnections();

    /**
     * @brief Prints the details of the backend server, including IP address, port, active connections, and availability.
     */
    void printDetails() const;
};;

#endif
