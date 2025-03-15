#pragma once
#include <yaml-cpp/yaml.h>
#include <string>
#include <unordered_map>
#include <vector>
// ��ܶ�ȡ�����ļ���
class MprpcConfig {
   public:
    //  ���ӽ������������ļ�
    void LoadConfigFile(std::string config_path);
    // ��ѯ��������Ϣ
    std::string Load(const std::string& key);

   private:
    std::unordered_map<std::string, std::string> m_configMap;
    std::vector<std::string> key_array = {"rpcserverip", "rpcserverport",
                                          "zookeeperip", "zookeeperport"};
};