@echo off
echo Building the project...
g++ -std=c++17 -Wall -Wextra -O2 -I src -I src/backend -I src/load_balancer -I src/server -I src/strategy src/main.cpp src/load_balancer/load_balancer.cpp src/backend/backend_server.cpp src/server/server.cpp src/strategy/round_robin.cpp -o load_balancer.exe -lws2_32

if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting...
    exit /b %ERRORLEVEL%
)

echo Running the program...
load_balancer.exe -c ./server_config.json -p 8000 -s round_robin
