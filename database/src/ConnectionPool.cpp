#include "ConnectionPool.h"

ConnectionPool* ConnectionPool::getConnectPool() {
    // һ�δ���, ��������, ֱ����������
    /*
    �洢�ھ�̬�������ı������ڳ���տ�ʼ����ʱ����ɳ�ʼ����
    Ҳ��Ψһ��һ�γ�ʼ����֮���ٴ����е����� static �ؼ��ֵ�
    ��ʼ�����ʱ������ִ�и����
    */
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::parseJsonFile() {
    std::ifstream inFile("dbconf.json");
    nlohmann::json data;
    inFile >> data;
    inFile.close();

    m_ip = data["ip"];
    m_port = data["port"];
    m_user = data["userName"];
    m_passwd = data["password"];
    m_dbName = data["dbName"];
    m_minSize = data["minSize"];
    m_maxSize = data["maxSize"];
    m_maxIdleTime = data["maxIdleTime"];
    m_timeout = data["timeout"];
    return true;
}

ConnectionPool::ConnectionPool() {
    // ���������ļ�
    if (!parseJsonFile()) {
        return;
    }

    for (int i = 0; i < m_minSize; i++) {
        addConnection();
    }

    /* ΪʲôҪ����thisָ��?
    ȷ�������ĸ�(��)����ĳ�Ա����
    */
    std::thread producer(&ConnectionPool::produceConnection, this);
    std::thread recycler(&ConnectionPool::recycleConnection, this);

    producer.detach();
    recycler.detach();
}

void ConnectionPool::addConnection() {
    MysqlConn* conn = new MysqlConn;
    conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
    m_connectionQ.push(conn);
}

void ConnectionPool::produceConnection() {
    while (true) {
        std::unique_lock<std::mutex> locker(m_mutex);

        while (m_connectionQ.size() >= m_minSize) {
            // ��Ҫ����һ��conn,�Ż�������
            m_cond.wait(locker);

            /* ��������, �����ж��Ƿ�ȱconn,
            ȱ����������whileִ�������add���, ������һ�ν������
            */
        }
        addConnection();
        m_cond.notify_all();
    }  // �ܵ����ﻥ�����ͷ���
}

void ConnectionPool::recycleConnection() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> locker(m_mutex);
        while (m_connectionQ.size() > m_minSize ||
               m_connectionQ.size() > m_maxSize) {
            MysqlConn* conn = m_connectionQ.front();
            if (m_connectionQ.size() > m_maxSize) {
                m_connectionQ.pop();
                delete conn;
            } else if (m_connectionQ.size() > m_minSize &&
                       m_connectionQ.size() < m_maxSize &&
                       conn->getAliveTime() >= m_maxIdleTime) {
                m_connectionQ.pop();
                delete conn;
            } else {
                break;
            }
        }
    }
}

std::shared_ptr<MysqlConn> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_connectionQ.empty()) {
        // ��Ҫ m_cond �����Ѳ����������whileѭ��
        if (std::cv_status::timeout ==
            m_cond.wait_for(locker, std::chrono::milliseconds(m_timeout))) {
            if (m_connectionQ.empty()) {
                continue;
            }
        }
    }
    // share_ptr�Ĺ���
    // ���ȵ��������Զ�����ͷŹ���
    std::shared_ptr<MysqlConn> connptr(
        m_connectionQ.front(), [this](MysqlConn* conn) {
            std::lock_guard<std::mutex> locker(m_mutex);
            conn->refreshAliveTime();
            m_connectionQ.push(conn);
        });
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}

ConnectionPool::~ConnectionPool() {
    while (!m_connectionQ.empty()) {
        MysqlConn* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}