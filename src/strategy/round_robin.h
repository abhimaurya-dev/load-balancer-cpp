#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "load_balancer.h"

class RoundRobinStrategy: public LoadBalancingStrategy{
  private: int currentIndex;

  public: RoundRobinStrategy() : currentIndex(-1) {}

  public: int selectServer(const std::vector<BackendServer>& servers) override;
};

#endif