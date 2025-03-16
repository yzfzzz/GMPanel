#include "mprpcapplication.h"
#include <unistd.h>
#include <iostream>
#include <string>
MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp() {
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(std::string config_path) {
    // 开始加载配置文件了
    /* config
    rpcserver_ip
    rpcserver_port
    zookeeper_ip
    zookeeper_port
    */
    m_config.LoadConfigFile(config_path);
    std::cout << "rpcserverip = " << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport = " << m_config.Load("rpcserverport")
              << std::endl;
    std::cout << "zookeeperip = " << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport = " << m_config.Load("zookeeperport")
              << std::endl;
}

MprpcConfig& MprpcApplication::GetConfig() { return m_config; }
