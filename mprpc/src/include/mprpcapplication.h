#pragma once
#include "mprpcchannel.h"
#include "mprpcconfig.h"
#include "mprpccontroller.h"
class MprpcApplication {
   public:
    static void Init(std::string config_path = "rpc_server.yaml");
    static MprpcApplication& GetInstance() {
        static MprpcApplication app;
        return app;
    }
    static MprpcConfig& GetConfig();

   private:
    static MprpcConfig m_config;
    MprpcApplication() {}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};