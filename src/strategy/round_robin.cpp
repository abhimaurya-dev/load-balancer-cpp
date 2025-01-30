#include "round_robin.h"
#include <iostream>

int RoundRobinStrategy::selectServer(const std::vector<BackendServer>& servers){
  std::cout<<"IN round strategy";
  int numOfServers = servers.size();
  std::cout<<numOfServers;
  if(numOfServers == 0){
    return -1;
  }


  for(int i = 0; i < numOfServers; i++){
    currentIndex = (currentIndex + 1) % numOfServers;
    if(servers[currentIndex].getAvailability()){
      return currentIndex;
    }
  }
  return -1;
}