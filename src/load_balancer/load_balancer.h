#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "backend_server.h"
#include <vector>
#include <memory>

class LoadBalancingStrategy {
  public:
  virtual int selectServer(const std::vector<BackendServer>& servers) = 0;
  virtual ~LoadBalancingStrategy() = default;
};

class LoadBalancer{
  public: LoadBalancer(std::unique_ptr<LoadBalancingStrategy> strategy);
  public: void addServer(const BackendServer& server);
  public: int getServerIndex();
  public: BackendServer& distributeRequest();
  public: void printServerDetails() const;

  private: std::vector<BackendServer> servers;
  private: std::unique_ptr<LoadBalancingStrategy> strategy;
};

#endif