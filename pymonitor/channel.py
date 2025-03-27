import random
import socket
from kazoo.client import KazooClient
from kazoo.recipe.watchers import DataWatch
import rpcheader_pb2
import struct
import logging
from enum import Enum

class LoadPattern(Enum):
    ROUND_ROBIN = 0
    RANDOM = 1
    HASH = 2

# 负载均衡器
class LoadBanlancer:
    def __init__(self, load_pattern):
        self.load_pattern = load_pattern
        self.index = 0

    def work(self, children):
        if(self.load_pattern == LoadPattern.ROUND_ROBIN):
            i = self.index
            self.index = (self.index + 1) % len(children)
            return children[i]
        elif(self.load_pattern == LoadPattern.RANDOM):
            i = random.randint(0, len(children)-1)
            return children[i]

class MprpcChannel:
    def __init__(self, config_data, load_pattern = LoadPattern.ROUND_ROBIN):
        self.zk_host = "124.223.141.236"
        self.zk_port = 2181
        self.zk = KazooClient(hosts=self.zk_host)
        self.zk.start()
        self.load_banlancer = LoadBanlancer(load_pattern)

    def __del__(self):
        self.zk.stop()

    def getService(self):
        SERVICE_PATH = "/MonitorManager/SetMonitorInfo"
        # 检查节点是否存在kill
        if self.zk.exists(SERVICE_PATH):
            # 获取节点数据
            children = self.zk.get_children(SERVICE_PATH)
            # 客户端负载均衡
            if len(children) > 0:
                node = self.load_banlancer.work(children)
                logging.info(f"Children of {SERVICE_PATH}: {children}, select: {node}")
                self.host, port = node.split(":")
                self.port = int(port)

    def callMethod(self ,service_name, method_name, request):

        try:
            self.getService()
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            arg = request.SerializeToString()
            arg_len = len(arg)
            rpcheader = rpcheader_pb2.RpcHeader(service_name=service_name, method_name=method_name, args_size=arg_len)
            rpcheader_str = rpcheader.SerializeToString()
            header_len = len(rpcheader_str)
            header_size_bytes = struct.pack('<I', header_len)

            send_rpc_str = bytearray()
            send_rpc_str.extend(header_size_bytes)
            send_rpc_str.extend(rpcheader_str)
            send_rpc_str.extend(arg)

            self.socket.sendall(send_rpc_str)
            response_data = self.socket.recv(4096)
            logging.info("Successful call function!!!")
            return response_data
        except Exception as e:
            logging.warning(f"Connection failed, find error: {e}!!!")