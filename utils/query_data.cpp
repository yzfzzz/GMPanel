#include "query_data.h"
#include <google/protobuf/util/json_util.h>
#include <algorithm>
#include <iostream>
#include "get_time.h"

using google::protobuf::util::JsonStringToMessage;

bool proto_to_json(const google::protobuf::Message& message,
                   std::string& json) {
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;
    return MessageToJsonString(message, &json, options).ok();
}

namespace monitor {
bool QueryData::queryDataInfo(std::string account_num,
                              std::string machine_name) {
    this->account_num = account_num;
    this->machine_name = machine_name;
    GetCurTime cur_time;

    ::monitor::proto::QueryMessage query_message;
    query_message.set_timeymd(cur_time.get_year_mon_day());
    query_message.set_machine_name(machine_name);
    query_message.set_accountnum(account_num);

    ::monitor::proto::QueryResults query_results;
    rpc_client.GetMonitorInfo(query_message, query_results);

    // TODO: 远端函数调用可能会失败，需要处理
    if (!proto_to_json(query_results, json_string)) {
        std::cout << "protobuf convert json failed!" << std::endl;
        return false;
    }
    return true;
}

bool UserManage::login(std::string account_num, std::string pwd) {
    monitor::proto::UserMessage request;
    monitor::proto::UserResponseMessage response;
    request.set_account_num(account_num);
    request.set_pwd(pwd);
    rpc_client.LoginRegister(request, response);
    if (!proto_to_json(response, json_string)) {
        std::cout << "protobuf convert json failed!" << std::endl;
        return false;
    }
    return true;
}

bool UserManage::signup(std::string pwd) {
    std::string account_num;
    monitor::proto::UserMessage request;
    monitor::proto::UserResponseMessage response;
    request.set_account_num(account_num);
    request.set_pwd(pwd);
    rpc_client.LoginRegister(request, response);
    if (!proto_to_json(response, json_string)) {
        std::cout << "protobuf convert json failed!" << std::endl;
        return false;
    }
    return true;
}

}  // namespace monitor