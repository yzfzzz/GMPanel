import site
# 添加一个新的搜索路径
new_path = './pyproto'
site.addsitedir(new_path)
# 查看搜索路径，确认新路径已添加
import sys
print(sys.path)

import grpc
import monitor_info_pb2
import monitor_info_pb2_grpc
import login_register_pb2
import login_register_pb2_grpc
from google.protobuf import empty_pb2

import psutil
import os
import platform
from pynvml import *
import time

class Monitor:
    def __init__(self):
        print("monitor init called")

    def convert_bytes(self, size, unit):
        """将字节转换为指定单位"""
        units = {
            "KB": 1024,
            "MB": 1024 ** 2,
            "GB": 1024 ** 3,
            "TB": 1024 ** 4
        }
        return size / units[unit]

    def get_cpu_info(self):
        """获取 CPU 信息"""
        cpu_percent = psutil.cpu_percent(interval=1)  # CPU 使用率
        cpu_count = psutil.cpu_count(logical=True)  # CPU 逻辑核心数
        cpu_freq = psutil.cpu_freq().current  # CPU 当前频率
        cpu_each_core_uesd = psutil.cpu_percent(interval=1, percpu=True)
        return {
            "cpu_percent": cpu_percent,
            "cpu_count": cpu_count,
            "cpu_freq": cpu_freq,
            "cpu_each_core_uesd": cpu_each_core_uesd
        }

    def get_memory_info(self):
        """获取内存信息"""
        mem = psutil.virtual_memory()
        return {
            "total_memory": round(self.convert_bytes(mem.total, "GB"), 2),  # 转换为 GB
            "available_memory": round(self.convert_bytes(mem.available, "GB"), 2),  # 转换为 GB
            "used_memory": round(self.convert_bytes(mem.used, "GB"), 2),  # 转换为 GB
            "memory_percent": mem.percent
        }

    def get_disk_info(self):
        """获取磁盘信息"""
        disk = psutil.disk_usage('/')
        return {
            "total_disk": round(self.convert_bytes(disk.total, "GB"), 2),  # 转换为 GB
            "used_disk": round(self.convert_bytes(disk.used, "GB"), 2),  # 转换为 GB
            "disk_percent": disk.percent
        }

    def get_network_info(self):
        """获取网络流量信息"""
        net_io = psutil.net_io_counters()
        return {
            "bytes_sent": round(self.convert_bytes(net_io.bytes_sent, "MB"), 2),  # 转换为 MB
            "bytes_recv": round(self.convert_bytes(net_io.bytes_recv, "MB"), 2)  # 转换为 MB
        }

    def get_gpu_info(self):
        """获取 GPU 信息"""
        gpu_info = []
        try:
            nvmlInit()
            device_count = nvmlDeviceGetCount()
            for i in range(device_count):
                handle = nvmlDeviceGetHandleByIndex(i)
                mem_info = nvmlDeviceGetMemoryInfo(handle)
                util = nvmlDeviceGetUtilizationRates(handle)
                gpu_info.append({
                    "gpu_id": i,
                    "gpu_name": nvmlDeviceGetName(handle),
                    "total_memory": round(self.convert_bytes(mem_info.total, "MB"), 2),  # 转换为 MB
                    "used_memory": round(self.convert_bytes(mem_info.used, "MB"), 2),  # 转换为 MB
                    "gpu_utilization": util.gpu,
                    "temperature": nvmlDeviceGetTemperature(handle, NVML_TEMPERATURE_GPU),
                    "memory_utilization": util.memory
                })
            nvmlShutdown()
        except Exception as e:
            print(f"Error fetching GPU info: {e}")
        return gpu_info

    def get_os_info(self):
        """获取操作系统信息"""
        return {
            "system": platform.system(),
            "release": platform.release(),
            "version": platform.version(),
            "machine": platform.machine(),
            "boot_time": time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(psutil.boot_time()))
        }

    def get_time(self):
        return{
            "timeymd": time.strftime("%Y-%m-%d",time.localtime()),
            "timehms": time.strftime("%H:%M:%S",time.localtime()),
        }

    def get_system_info(self):
        """获取完整的系统信息"""
        return {
            "cpu_info": self.get_cpu_info(),
            "memory_info": self.get_memory_info(),
            "disk_info": self.get_disk_info(),
            "network_info": self.get_network_info(),
            "gpu_info": self.get_gpu_info(),
            "os_info": self.get_os_info(),
            "time": self.get_time()
        }


class RpcClient:
    def __init__(self, address):
        self.channel = grpc.insecure_channel(address)
        self.monitor_stub = monitor_info_pb2_grpc.MonitorManagerStub(self.channel)
        self.user_stub = monitor_info_pb2_grpc.UserManagerStub(self.channel)


    def get_monitor_info(self, sys_info):
        print("run")
        monitor_info = monitor_info_pb2.MonitorInfo(
            name="MyMachine",
            hard_disk=monitor_info_pb2.hard__disk__pb2.HardDisk(total=sys_info['disk_info']['total_disk'], used=sys_info['disk_info']['used_disk'],
                                                used_percent = sys_info['disk_info']['disk_percent']),
            cpu_load=monitor_info_pb2.cpu__load__pb2.CpuLoad(load_avg_1=0.5, load_avg_3=0.4, load_avg_15=0.3),
            cpu_stat=monitor_info_pb2.cpu__stat__pb2.CpuStat(cpu_percent=sys_info['cpu_info']['cpu_percent'], cpu_logic_num=sys_info['cpu_info']['cpu_count'],
                                              each_core_utilization_rate=sys_info['cpu_info']['cpu_each_core_uesd']),
            mem_info=monitor_info_pb2.mem__info__pb2.MemInfo(total=sys_info['memory_info']['total_memory'], avail=sys_info['memory_info']['available_memory']),
            net_info=monitor_info_pb2.net__info__pb2.NetInfo(send_rate=sys_info['network_info']['bytes_sent'], rcv_rate=sys_info['network_info']['bytes_recv']),
            accountnum="user123",
            time=monitor_info_pb2.time__pb2.Time(timeymd=sys_info['time']['timeymd'] , timehms = sys_info['time']['timehms']),
            os=monitor_info_pb2.os__pb2.OS(os_name=sys_info['os_info']['system'], os_startup_time=sys_info['os_info']['boot_time']),
            machine_name="MyMachine"
        )
        for gpu_info in sys_info['gpu_info']:
            gpu = monitor_info_pb2.gpu__info__pb2.GpuInfo(gpu_name=gpu_info['gpu_name'], gpu_mem_total=int(gpu_info['total_memory']),
                                                gpu_mem_used = int(gpu_info['used_memory']),temperture = gpu_info['temperature'],
                                                gpu_utilize = gpu_info['memory_utilization'] )
            monitor_info.gpu_info.append(gpu)

        return monitor_info

    def SetMonitorInfo(self):
        m = Monitor()
        self.monitor_stub.SetMonitorInfo(self.get_monitor_info(m.get_system_info()))




# def run():
#     # 创建 gRPC 通道
#     channel = grpc.insecure_channel('124.223.141.236:50051')
#
#     # 创建客户端
#     monitor_stub = monitor_info_pb2_grpc.MonitorManagerStub(channel)
#     user_stub = monitor_info_pb2_grpc.UserManagerStub(channel)
#
#     # 调用 SetMonitorInfo
#     # response = monitor_stub.SetMonitorInfo(monitor_info)
#     # print("SetMonitorInfo response:", response)
#
#     # 调用 GetMonitorInfo
#     # query_message = monitor_info_pb2.QueryMessage(query="Get all info")
#     # query_results = monitor_stub.GetMonitorInfo(query_message)
#     # print("GetMonitorInfo response:", query_results)
#
#     # 调用 UserManager 的 LoginRegister
#     user_message = login_register_pb2.UserMessage(account_num="168925", pwd="123456")
#     user_response = user_stub.LoginRegister(user_message)
#     print("LoginRegister response:", user_response.machine_name_array[1])


if __name__ == '__main__':
    m = Monitor()
    info = m.get_system_info()
    print(info)
    rpc_client = RpcClient("124.223.141.236:50051")
    rpc_client.SetMonitorInfo()

