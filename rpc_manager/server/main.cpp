#include <iostream>
#include <string>
#include <vector>
#include "rpc_manager.h"
#include "log.h"

int main(int argv, char** argc) {
    std::string log_path = "./logs";
    monitor::SetupLogging(log_path);
    // ���ÿ�ܵĳ�ʼ������
    MprpcApplication::Init(argv, argc);

    RpcProvider provider;
    provider.NotifyService(new monitor::ServerManagerImpl());

    // ����һ��rpc���񷢲��ڵ�, Run�Ժ���̽�������״̬���ȴ�Զ�̵�rpc��������
    provider.Run();

    return 0;
}