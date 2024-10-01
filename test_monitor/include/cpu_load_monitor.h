#pragma once

#include <string>
#include "monitor_inter.h"

namespace monitor{

// ��̬
class CpuLoadMonitor: public MonitorInter{
public:
    CpuLoadMonitor(){}
    // ��д�麯��
    void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
    // override������ C++11 �����һ���ؼ��֣�������ȷָ�������Ա�����ǻ�����ĳ���麯������д
    void Stop() override {}


private:
    float m_load_avg_1;
    float m_load_avg_3;
    float m_load_avg_15;
};

};