#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
namespace monitor
{

    class ReadFile
    {

    public:
        // �ù��캯������ʽ�ģ�������ʽ��
        explicit ReadFile(const string &path) : m_ifs(path) {}
        // ��������
        ~ReadFile() { m_ifs.close(); }

        bool ReadLine(vector<string>* arg);

    private:
        // ��ȡ�ļ����ݣ����Դ򿪲���ȡ�ļ��е�����
        ifstream m_ifs;
    };

};