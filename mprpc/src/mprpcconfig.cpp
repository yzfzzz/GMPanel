#include "mprpcconfig.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <unordered_map>
//  复杂解析加载配置文件
void MprpcConfig::LoadConfigFile(std::string config_path) {

    YAML::Node config = YAML::LoadFile(config_path);
    for (auto key : key_array) {
        std::string value = config[key].as<std::string>();

        // 域名解析
        if (key == "rpc_server_ip" || key == "zookeeper_ip") {
            std::unordered_map<std::string, std::string> ip_map;
            struct addrinfo hints, *result, *rp;
            int status = -1;
            std::string hostname = value;  // 要解析的域名

            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_family = AF_UNSPEC;      // 允许IPv4和IPv6
            hints.ai_socktype = SOCK_STREAM;  // 用于TCP连接

            status = getaddrinfo(hostname.c_str(), NULL, &hints, &result);
            if (status != 0) {
                std::cerr << "getaddrinfo error: " << gai_strerror(status)
                          << std::endl;
                return;
            }

            for (rp = result; rp != NULL; rp = rp->ai_next) {
                char ipstr[INET6_ADDRSTRLEN];
                void *addr;
                if (rp->ai_family == AF_INET) {
                    // IPv4地址
                    struct sockaddr_in *ipv4 =
                        (struct sockaddr_in *)rp->ai_addr;
                    addr = &(ipv4->sin_addr);
                    inet_ntop(rp->ai_family, addr, ipstr, sizeof(ipstr));
                    ip_map["IPV4"] = ipstr;
                } else {
                    // IPv6地址
                    struct sockaddr_in6 *ipv6 =
                        (struct sockaddr_in6 *)rp->ai_addr;
                    addr = &(ipv6->sin6_addr);
                    inet_ntop(rp->ai_family, addr, ipstr, sizeof(ipstr));
                    ip_map["IPV6"] = ipstr;
                }
            }
            freeaddrinfo(result);
            value = ip_map["IPV4"];
        }

        m_configMap.insert(std::pair<std::string, std::string>(key, value));
    }
}

// 查询配置项信息
std::string MprpcConfig::Load(const std::string &key) {
    auto it = m_configMap.find(key);
    if (it == m_configMap.end()) {
        return "";
    }
    return it->second;
}
