#pragma once
#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>
#include <vector>

// 封装的zk客户端类
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // 启动连接
    void Start();
    // 在zkserver上根据path创建znode节点，state=0表示永久性节点
    void Create(const char* path, const char* data, int datalen, int state = 0);
    // 根据参数指定的znode节点路径或者znode节点的值
    std::string GetData(const char* path);
    std::vector<std::string> GetChildren(const char* path);
private:
    // zk客户端的句柄
    zhandle_t *m_zhandle;
};