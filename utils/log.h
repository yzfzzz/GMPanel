#pragma once

#include <glog/logging.h>
#include <unistd.h>
#include <iostream>
#include <string>
namespace monitor {

static void SetupLogging(const std::string& log_path) {
    // ������־����
    FLAGS_stderrthreshold = google::INFO;
    FLAGS_log_dir = log_path;

    // ������־�ļ���СΪ10MB
    FLAGS_max_log_size = 10 * 1024;          // 10MB
    FLAGS_stop_logging_if_full_disk = true;  // ������ʱֹͣ��¼

    // ������־�ļ����еĻ�������׺
    FLAGS_alsologtostderr = true;
    FLAGS_logtostderr = false;

    FLAGS_logtostderr = true;      // ������־�����stderr
    FLAGS_alsologtostderr = true;  // ͬʱ������ļ���stderr

    google::SetLogDestination(google::GLOG_INFO, "./logs/INFO_");
    google::SetLogDestination(google::GLOG_ERROR, "./logs/ERROR_");

    // �Զ��Ƴ�����־ day ��apt�ɰ汾û�У�
    // google::EnableLogCleaner(3);
}
}  // namespace monitor

/*
int main(int argc, char *argv[])
{
    // ������־Ŀ¼
    std::string logDir = "./logs";

    // ��ʼ����־
    SetupLogging(logDir);

    // ��ʼ�� Glog
    google::InitGoogleLogging(argv[0]);

    // ʾ�������ͬ�������־
    LOG(INFO) << "This is an informational message.";
    LOG(WARNING) << "This is a warning message.";
    LOG(ERROR) << "This is an error message.";
    // LOG(FATAL) << "This is a fatal error message.";

    return 0;
}
*/