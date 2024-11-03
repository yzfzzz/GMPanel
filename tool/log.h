#pragma once

#include <glog/logging.h>
#include <iostream>
#include <string>
#include <unistd.h>
namespace monitor {

static void SetupLogging(const std::string& log_path) {
    // 设置日志级别
    FLAGS_stderrthreshold = google::INFO;
    FLAGS_log_dir = log_path;

    // 设置日志文件大小为10MB
    FLAGS_max_log_size = 10 * 1024;  // 10MB

    // 禁用日志文件名中的机器名后缀
    FLAGS_alsologtostderr = true;
    FLAGS_logtostderr = false;

    // 自动移除旧日志 day （apt旧版本没有）
    // google::EnableLogCleaner(3);
}
}  // namespace monitor

/*
int main(int argc, char *argv[])
{
    // 设置日志目录
    std::string logDir = "./logs";

    // 初始化日志
    SetupLogging(logDir);

    // 初始化 Glog
    google::InitGoogleLogging(argv[0]);

    // 示例输出不同级别的日志
    LOG(INFO) << "This is an informational message.";
    LOG(WARNING) << "This is a warning message.";
    LOG(ERROR) << "This is an error message.";
    // LOG(FATAL) << "This is a fatal error message.";

    return 0;
}
*/