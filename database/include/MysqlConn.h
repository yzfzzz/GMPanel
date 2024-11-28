#pragma once
#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <chrono>
class MysqlConn {
   public:
    // ��ʼ�����ݿ�����
    MysqlConn();
    // �ͷ����ݿ�����
    ~MysqlConn();
    // �������ݿ�
    bool connect(std::string user, std::string passwd, std::string dbName,
                 std::string ip, unsigned short port = 3306);
    // �������ݿ�, insert/update/delete
    bool update(std::string sql);
    // ��ѯ���ݿ�
    bool query(std::string sql);
    // ������ѯ�õ������
    bool next();
    // �õ�������е��ֶ�ֵ
    std::string value(int index);
    // �����������(�ֶ��ύ)
    bool transaction();
    // �ύ����
    bool commit();
    // ����ع�
    bool rollback();
    // ˢ����һ�λʱ��
    void refreshAliveTime();
    // ������е���ʱ��
    long long getAliveTime();

   private:
    // �ͷŽ�����ռ�
    void freeResult();
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row = nullptr;
    std::chrono::steady_clock::time_point m_alivetime;

};