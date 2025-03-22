#pragma once
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum LoadPattern {
    ROUND_ROBIN = 0,
    RANDOM = 1,
    HASH = 2,
};

class LoadBalancer {
   public:
    LoadBalancer(int load_pattern = ROUND_ROBIN)
        : load_pattern_(load_pattern) {}

    std::string operator()(std::vector<std::string> host_array);

   private:
    std::string roundRobinBalance(std::vector<std::string> host_array);
    std::string randomBalance(std::vector<std::string> host_array);
    std::string hashBalance(std::vector<std::string> host_array);
    std::unordered_map<std::string, std::string> queryHostIP();
    int load_pattern_;
};
