#include <semaphore.h>
#include <iostream>
#include "mprpcapplication.h"
#include "zookeeperutil.h"

ZkClient::ZkClient() : m_zhandle(nullptr) {}

ZkClient::~ZkClient() {
    if (m_zhandle != nullptr) {
        // 关闭句柄, 释放资源
        zookeeper_close(m_zhandle);
    }
}

// 全局的watcher观察器  zkserver给zkclient的通知
void global_watcher(zhandle_t* zh, int type, int state, const char* path,
                    void* watcherCtx) {
    // 回调的消息类型是和会话相关的消息类型
    if (type == ZOO_SESSION_EVENT) {
        // zkclient和zkserver连接成功
        if (state == ZOO_CONNECTED_STATE) {
            // !这里执行完zoo_get_context函数，sem可能是一个空指针
            sem_t* sem = nullptr;
            while(sem == nullptr)
            {
                sem = (sem_t*)zoo_get_context(zh);
            }
            sem_post(sem);
        }
    }
}

void ZkClient::Start() {
    std::string host =
        MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port =
        MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    /*
    zookeeper_mt: 多线程版本
    zookeeper的api客户端提供了三个线程
    api调用线程
    网络I/O线程 pthread_create poll
    watcher回调线程
    */
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr,
                               nullptr, 0);
    if (m_zhandle == nullptr) {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 此时创建句柄成功了

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}

void ZkClient::Create(const char* path, const char* data, int datalen,
                      int state) {
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    // 先判断path表示的znode节点是否存在, 如果存在, 就不重复创建了
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    // 节点不存在
    if (ZNONODE == flag) {
        // 创建指定的path的znode节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE,
                          state, path_buffer, bufferlen);
        if (flag == ZOK) {
            std::cout << "znode create success... path:" << path << std::endl;
        } else {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char* path) {
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK) {
        std::cout << "get znode error... path" << path << std::endl;
        return "";
    } else {
        return buffer;
    }
}

std::vector<std::string> ZkClient::GetChildren(const char* path) {
    String_vector children;
    int flag = zoo_get_children(m_zhandle, path, 0, &children);
    if(flag != ZOK)
    {
        std::cout << "get children error... path:" << path << std::endl;
        return std::vector<std::string>();
    }
    std::vector<std::string> v;
    for(int i = 0; i < children.count; ++i)
    {
        v.push_back(children.data[i]);
    }
    return v;
}