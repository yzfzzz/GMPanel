import psutil
import os
import platform
from pynvml import *
import logging
import time

class Monitor:
    def __init__(self):
        print("monitor init called")
        try:
            nvmlInit()
        except NVMLError as err:
            print("GPU warning: {}".format(err))
            logging.warning("GPU warning: {}".format(err))

    def __del__(self):
        try:
            nvmlShutdown()
        except NVMLError as err:
            logging.warning("GPU warning: {}".format(err))

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
            "cpu_each_core_uesd": list(map(lambda x: round(x, 2), cpu_each_core_uesd))
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
            device_count = nvmlDeviceGetCount()
            for i in range(device_count):
                handle = nvmlDeviceGetHandleByIndex(i)
                mem_info = nvmlDeviceGetMemoryInfo(handle)
                util = nvmlDeviceGetUtilizationRates(handle)
                gpu_info.append({
                    "gpu_id": str(i),
                    "gpu_name": nvmlDeviceGetName(handle),
                    "total_memory": round(self.convert_bytes(mem_info.total, "MB"), 2),  # 转换为 MB
                    "used_memory": round(self.convert_bytes(mem_info.used, "MB"), 2),  # 转换为 MB
                    "gpu_utilization": util.gpu,
                    "temperature": nvmlDeviceGetTemperature(handle, NVML_TEMPERATURE_GPU),
                    "memory_utilization": util.memory
                })
        except Exception as e:
            logging.warning(f"Error fetching GPU info: {e}")
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
            "timeymd": time.strftime("%Y%m%d",time.localtime()),
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