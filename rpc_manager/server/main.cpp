#include <iostream>
#include <string>
#include <vector>
#include "rpc_manager.h"

int main(int argv, char** argc) {
    // ���ÿ�ܵĳ�ʼ������
    MprpcApplication::Init(argv, argc);

    RpcProvider provider;
    provider.NotifyService(new monitor::ServerManagerImpl());

    // ����һ��rpc���񷢲��ڵ�, Run�Ժ���̽�������״̬���ȴ�Զ�̵�rpc��������
    provider.Run();

    return 0;
}