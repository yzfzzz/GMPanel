#include <utils/read_file.h>
// ifstream -> string -> istringstream
// ��ȡ�ļ��е�һ���ı�������ָ�ɵ��ʣ�������Щ���ʴ洢��arg��
bool monitor::ReadFile::ReadLine(vector<string>* arg)
{
    string line;
    /*
    m_ifs ����ʾһ�������������� cin��
    line ��string���͵����ã������洢�������е�����Ϣ��
    */
    getline(m_ifs, line);
    // eof()�������ж��ļ��Ƿ�Ϊ�ջ����Ƿ�����ļ���β
    if(m_ifs.eof() || line.empty())
    {
        return false;
    }
    // !
    istringstream line_ss(line);
    while(!line_ss.eof())
    {
        string word;
        line_ss >> word;
        arg->push_back(word);
    }
    return true;
}