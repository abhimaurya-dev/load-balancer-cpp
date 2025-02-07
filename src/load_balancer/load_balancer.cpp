#include "load_balancer.h"
#include <iostream>

LoadBalancer::LoadBalancer(std::unique_ptr<LoadBalancingStrategy> strategy) : strategy(std::move(strategy)) {}

void LoadBalancer::addServer(const BackendServer& server){
  servers.push_back(server);
}

BackendServer& LoadBalancer::distributeRequest(){
  if(strategy == nullptr){
    std::cout<<"Strategy not initialized";
    exit(1);
  }
  int serverIndex = strategy->selectServer(servers);
  if(serverIndex == -1){
    std::cout<<"No available server to handle the request"<<std::endl;
    static BackendServer invalidServer = BackendServer("0.0.0.0", 0000);
    return invalidServer;
  }

  BackendServer& selectedServer = servers[serverIndex];

  std::cout<<"Request routed to server: "<< selectedServer.getIpAddress() << ":" << selectedServer.getPort() << std::endl;

  selectedServer.incrementConnections();
  return selectedServer;
}

void LoadBalancer::printServerDetails() const {
  for(const auto& server: servers){
    server.printDetails();
  }
}