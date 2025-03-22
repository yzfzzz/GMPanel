#pragma once
#include <yaml-cpp/yaml.h>
#include <string>
#include <unordered_map>
#include <vector>
// 框架读取配置文件类
class MprpcConfig {
   public:
    //  复杂解析加载配置文件
    void LoadConfigFile(std::string config_path);
    // 查询配置项信息
    std::string Load(const std::string& key);

   private:
    std::unordered_map<std::string, std::string> m_configMap;
    std::vector<std::string> key_array = {"public_network_ip", "private_network_ip","rpcserverport",
                                          "zookeeperip", "zookeeperport"};
};