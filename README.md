# Load Balancer in C++ (windows)

## Overview

This project implements a **custom load balancer** in C++ that distributes incoming requests across multiple backend servers. It supports configurable server management and uses the **Strategy Design Pattern** to allow flexible selection of load balancing algorithms.

## Features

- Implements **Round Robin scheduling** for balanced request distribution.
- Uses **Strategy Design Pattern** to allow easy extension with new load balancing algorithms.
- Supports **configurable server management** via `server_config.json`.
- Modular backend structure for efficient request processing.

## Configuration

The load balancer reads its configuration from `server_config.json`, which defines the backend servers and settings.

Example `server_config.json`:

```json
{
  "servers": [
    { "ip": "192.168.1.1", "port": 8080 },
    { "ip": "192.168.1.2", "port": 8080 }
  ]
}
```

## How to Run

1. **Build the Project**

   ```sh
   g++ -std=c++17 -Wall -Wextra -O2 -I src -I src/backend -I src/load_balancer -I src/server -I src/strategy src/main.cpp src/load_balancer/load_balancer.cpp src/backend/backend_server.cpp src/server/server.cpp src/strategy/round_robin.cpp -o load_balancer.exe -lws2_32

   ```

2. **Run the Load Balancer**
   ```sh
   ./load_balancer.exe -c ./server_config.json -p 8000 -s round_robin
   ```

## Future Enhancements

- Add support for **Least Connections** and **IP Hashing** load balancing algorithms.
- Implement **multi-threaded request handling** for better performance.
- Integrate **health checks** to remove unresponsive servers dynamically.
