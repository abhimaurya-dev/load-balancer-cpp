#include "backend_server.h"
#include <iostream>

/**
 * @brief Constructs a new BackendServer object with the specified IP address and port.
 * 
 * Initializes the server with the given IP address and port number. 
 * Sets the initial number of active connections to zero and marks the server as available.
 * 
 * @param ipAddress The IP address of the backend server.
 * @param port The port number on which the backend server listens.
 */
BackendServer::BackendServer(const std::string& ipAddress, int port) 
  : ipAddress(ipAddress), port(port), activeConnections(0), isAvailable(true) {};

// Getters

/**
 * @brief Retrieves the IP address of the backend server.
 * 
 * @return A string representing the IP address of the backend server.
 */
std::string BackendServer::getIpAddress() const {
  return ipAddress;
}

/**
 * @brief Retrieves the number of active connections to the backend server.
 * 
 * @return The current number of active connections.
 */
int BackendServer::getActiveConnections() const {
  return activeConnections;
}

/**
 * @brief Retrieves the port number of the backend server.
 * 
 * @return The port number as an integer.
 */
int BackendServer::getPort() const {
  return port;
}

/**
 * @brief Checks the availability status of the backend server.
 * 
 * @return true if the backend server is available, false otherwise.
 */
bool BackendServer::getAvailability() const {
  return isAvailable;
}

// Setters

/**
 * @brief Sets the availability status of the backend server.
 * 
 * This function updates the availability status of the backend server.
 * 
 * @param availability A boolean value indicating the desired availability status.
 *                      - true: The server is available.
 *                      - false: The server is not available.
 */
void BackendServer::setAvailability(bool availability){
  isAvailable = availability;
}

/**
 * @brief Increments the count of active connections to the backend server.
 *
 * This function increases the number of active connections by one.
 * It should be called whenever a new connection is established.
 */
void BackendServer::incrementConnections() {
  activeConnections += 1;
}

/**
 * @brief Decrements the count of active connections to the backend server.
 *
 * This function decreases the number of active connections by one.
 * It should be called whenever a connection is removed.
 * If there are no active connections, the count remains unchanged.
 */
void BackendServer::decrementConnections() {
  if(activeConnections > 0){
    activeConnections -= 1;
  }
}

/**
 * @brief Prints the details of the backend server.
 * 
 * This function outputs the server's IP address, port number, 
 * number of active connections, and availability status to the standard output.
 */
void BackendServer::printDetails() const {
    std::cout << "Backend Server Details:" << std::endl;
    std::cout << "IP Address: " << ipAddress << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Active Connections: " << activeConnections << std::endl;
    std::cout << "Availability: " << (isAvailable ? "Available" : "Unavailable") << std::endl;
}