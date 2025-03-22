#include "loadbalancer.h"
std::string LoadBalancer::operator()(std::vector<std::string> host_array) {
    switch (load_pattern_) {
        case ROUND_ROBIN:
            return roundRobinBalance(host_array);
        case RANDOM:
            return randomBalance(host_array);
        case HASH:
            return hashBalance(host_array);
    }
}

std::string LoadBalancer::roundRobinBalance(
    std::vector<std::string> host_array) {
    static int index = 0;
    return host_array[index++ % host_array.size()];
}

std::string LoadBalancer::randomBalance(std::vector<std::string> host_array) {
    return host_array[rand() % host_array.size()];
}

std::string LoadBalancer::hashBalance(std::vector<std::string> host_array) {
    // 获取本机IP
    std::unordered_map<std::string, std::string> ip_map = queryHostIP();
    std::string local_ip = ip_map.at("eth0");
    std::size_t hash_value = std::hash<std::string>{}(local_ip);
    return host_array[hash_value % host_array.size()];
}

std::unordered_map<std::string, std::string> LoadBalancer::queryHostIP() {
    std::unordered_map<std::string, std::string> ip_map;
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    // 获取网络接口列表
    if (getifaddrs(&ifaddr) == -1) {
        std::cerr << "query network ip error\n";
        return ip_map;
    }
    // 遍历网络接口
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) {
            continue;
        }

        // 只处理 IPv4 地址
        if (ifa->ifa_addr->sa_family == AF_INET) {
            // 将地址转换为字符串
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host,
                        NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
            ip_map[ifa->ifa_name] = host;
        }
    }
    // 释放接口列表
    freeifaddrs(ifaddr);
    return ip_map;
}
