#include "mprpcapplication.h"
#include "mprpclog.h"
#include "rpcheader.pb.h"
#include "rpcprovider.h"
#include "zookeeperutil.h"
/*
service_name => service描述
                        ==> service* 记录服务对象
                        method_name => method方法对象
*/
// 存储 UserService类、对应的函数, 方便随时调用
void RpcProvider::NotifyService(google::protobuf::Service* service) {
    //  service实际上就是monitor::proto::MonitorManager类
    ServiceInfo service_info;
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pserviceDesc =
        service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = pserviceDesc->name();  // UserService
    // 获取服务对象service的方法数量
    int methodCnt = pserviceDesc->method_count();  // 2

    std::cout << "service_name: " << service_name << std::endl;

    for (int i = 0; i < methodCnt; i++) {
        // 获取了服务对象指定下标的服务方法的描述(抽象描述)
        const google::protobuf::MethodDescriptor* pmethodDesc =
            pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        std::cout << "method_name: " << method_name << std::endl;
    }

    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

void RpcProvider::Run() {
    std::string private_network_ip =
        MprpcApplication::GetInstance().GetConfig().Load("private_network_ip");
    uint16_t port = atoi(MprpcApplication::GetInstance()
                             .GetConfig()
                             .Load("rpcserverport")
                             .c_str());
    muduo::net::InetAddress address(private_network_ip, port);

    // ------------------------muduo------------------------------------
    // 启动Tcp Server
    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    // 绑定连接回调和消息回调方法, 分离了网络代码和业务代码
    // 将RpcProvider::OnConnection(conn)中的参数设置为this
    server.setConnectionCallback(
        std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));

    server.setMessageCallback(
        std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));
    // 设置muduo库的线程数量
    server.setThreadNum(4);

    // 把当前rpc节点上要发布的服务全部注册到zk上面, 让rpc
    // client可以从zk上发现服务
    std::string public_network_ip =
        MprpcApplication::GetInstance().GetConfig().Load("public_network_ip");
    ZkClient zkCli;
    zkCli.Start();
    // service_name为永久性节点, method为临时性节点
    for (auto& sp : m_serviceMap) {
        // /service_name    /UserServiceRpc
        std::string service_path = "/" + sp.first;
        zkCli.Create(service_path.c_str(), nullptr, 0);
        for (auto& mp : sp.second.m_methodMap) {
            // /service_name/method_name    /UserServiceRpc/Login
            std::string method_path = service_path + "/" + mp.first;
            zkCli.Create(method_path.c_str(), nullptr, 0);
            char host_path[128] = {0};
            // 存储当前这个rpc主机的ip和port, 作为一个子节点
            // /UserServiceRpc/Login/182.168.1.1:80
            sprintf(host_path, "%s:%d", public_network_ip.c_str(), port);
            std::string node_path = method_path + "/" + host_path;
            zkCli.Create(node_path.c_str(), nullptr, 0,
                         ZOO_EPHEMERAL);  // ZOO_EPHEMERAL表示临时性节点
        }
    }

    // rpc服务端准备启动, 打印信息
    std::cout << "RpcProvider start service at ip:" << public_network_ip
              << " port:" << port << std::endl;

    // 启动网络服务
    server.start();
    m_eventLoop.loop();
}

/*
在框架内部, RpcProvider和RpcConsumer协商好之间通信用的protobuf数据类型
service_name method_name args 定义proto的message类型，进行数据的序列化和反序列化
数据格式: header_size + header_str + args_str
16UserServiceLoginzhang san123456

*/

// 新的socket连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
    if (!conn->connected()) {
        // 和rpc client的连接断开了
        conn->shutdown();
    }
}

// 已建立连接用户的读写事件回调, 如果远程有一个rpc服务的调用请求,
// 那么OnMessage方法会响应 ==> 黄色部分
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer, muduo::Timestamp) {
    // 网络上接收的远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString();

    // 从字符流中读取前4个字节的内容, 将数据拷贝到header_size的地址处
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    // 根据header_size读取数据头的原始字符流
    std::string rpc_header_str = recv_buf.substr(4, header_size);

    // 反序列化数据, 得到rpc请求的详细信息
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if (rpcHeader.ParseFromString(rpc_header_str)) {
        // 数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        // 数据头反序列化失败
        std::cout << "rpc_head_str: " << rpc_header_str << "parse error!"
                  << std::endl;
        return;
    }
    // 获取rpc方法参数的字节流数据
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // 打印调试信息
    // std::cout << "=========================" << std::endl;
    // std::cout << "header_size: " << header_size << std::endl;
    // std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    // std::cout << "service_name: " << service_name << std::endl;
    // std::cout << "method_name: " << method_name << std::endl;
    // std::cout << "args_str: " << args_str << std::endl;

    // 获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end()) {
        std::cout << service_name << "is not exist!" << std::endl;
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end()) {
        std::cout << service_name << ": " << method_name << "is not exist!"
                  << std::endl;
        return;
    }

    google::protobuf::Service* service =
        it->second.m_service;  // 获取service对象 new UserService
    const google::protobuf::MethodDescriptor* method =
        mit->second;  // 获取method对象 Login

    // 生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message* request =
        service->GetRequestPrototype(method).New();
    // 反序列化
    if (!request->ParseFromString(args_str)) {
        std::cout << "request  parse error! content: " << args_str << std::endl;
        return;
    }

    google::protobuf::Message* response =
        service->GetResponsePrototype(method).New();

    // 给下面的method方法的调用, 绑定一个Closure的回调函数
    // 调用的是callback.h文件的482行的函数
    // inline Closure* NewCallback(Class* object,
    //                              void (Class::*method)(Arg1, Arg2),Arg1 arg1,
    //                              Arg2 arg2)
    google::protobuf::Closure* done =
        google::protobuf::NewCallback<RpcProvider,
                                      const muduo::net::TcpConnectionPtr&,
                                      google::protobuf::Message*>(
            this, &RpcProvider::SendRpcResponse, conn, response);

    // 在框架上根据远端rpc请求, 调用当前rpc节点上发布的方法
    // new UserService().Login(controller, request, response, done);
    service->CallMethod(method, nullptr, request, response, done);
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,
                                  google::protobuf::Message* response) {
    std::string response_str;
    if (response->SerializeToString(&response_str)) {
        // 序列化成功后, 通过网络把rpc方法执行的结果发送回rpc的调用方
        conn->send(response_str);
    } else {
        std::cout << "serialize response_str error" << std::endl;
    }
    conn->shutdown();  // 模拟http的短连接服务, 由rpcprovider主动断开连接
}