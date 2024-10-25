#pragma once
#include <string>
#include "monitor_info.pb.h"
#include "monitor_inter.h"

namespace monitor {
/*  ������
    - �޷�ʵ��������
    - ���������д�������еĴ��麯��������Ҳ���ڳ�����
*/
class MonitorInter {
   public:
    MonitorInter() {}
    virtual ~MonitorInter() {}
    // ���麯��
    virtual void UpdateOnce(monitor::proto::MonitorInfo* monitor_info) = 0;
    virtual void Stop() = 0;
};

};  // namespace monitor