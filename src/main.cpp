#include "load_balancer.h"
#include "server.h"
#include "round_robin.h"
#include "backend_server.h"
#include <iostream>
#include "json.hpp"
#include <fstream>


void printUsage() {
    std::cout << "Usage: load_balancer -c <config.json> -p <port> -s <strategy>\n";
}

int main(int argc, char* argv[]) {
    std::string configFile;
    int loadBalancerPort = 8080; // Default port
    std::string strategyType = "round_robin";

    // Parse CLI arguments
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-c" && i + 1 < argc) {
            configFile = argv[++i];
        } else if (std::string(argv[i]) == "-p" && i + 1 < argc) {
            loadBalancerPort = std::stoi(argv[++i]);
        } else if (std::string(argv[i]) == "-s" && i + 1 < argc) {
            strategyType = argv[++i];
        } else {
            printUsage();
            return 1;
        }
    }

    if (configFile.empty()) {
        std::cerr << "Error: Config file is required.\n";
        printUsage();
        return 1;
    }

    // Load backend servers from config file
    std::ifstream file(configFile);
    if (!file) {
        std::cerr << "Error: Could not open config file: " << configFile << std::endl;
        return 1;
    }

    nlohmann::json config;
    file >> config;

    std::vector<BackendServer> servers;
    for (const auto& server : config["servers"]) {
        servers.emplace_back(server["ip"], server["port"]);
    }

    // Initialize strategy
    std::unique_ptr<LoadBalancingStrategy> strategy;
    if (strategyType == "round_robin") {
        strategy = std::make_unique<RoundRobinStrategy>();
    } else {
        std::cerr << "Error: Unknown strategy type: " << strategyType << std::endl;
        return 1;
    }

    // Create and start load balancer
    LoadBalancer loadBalancer(std::move(strategy));
    for (const auto& server : servers) {
        loadBalancer.addServer(server);
    }

  std::cout << "Initial backend server details:" << std::endl;
  loadBalancer.printServerDetails();

//   // Simulate distributing requests
//   std::cout << "\nSimulating request distribution:" << std::endl;
//   for (int i = 0; i < 5; ++i) {
//     std::cout<<"hello";
//       loadBalancer.distributeRequest();
//   }
    Server& loadBalancerServer = Server::getInstance(loadBalancerPort, loadBalancer);
    loadBalancerServer.start();

  // Debug: Print updated server details
  std::cout << "\nUpdated backend server details after requests:" << std::endl;
  loadBalancer.printServerDetails();

  return 0;
}