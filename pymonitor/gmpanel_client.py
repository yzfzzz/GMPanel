import site
new_path = './pyproto'
site.addsitedir(new_path)
import sys
import monitor_info_pb2
import login_register_pb2
from google.protobuf import empty_pb2
import yaml
import monitor
import channel
import logging
import time

class RpcClient:
    def __init__(self, config_data):
        self.monitor = monitor.Monitor()
        self.channel = channel.MprpcChannel(config_data)
        self.config_data = config_data

    def get_monitor_info(self, sys_info):

        monitor_info = monitor_info_pb2.MonitorInfo(
            hard_disk=monitor_info_pb2.hard__disk__pb2.HardDisk(total=sys_info['disk_info']['total_disk'], used=sys_info['disk_info']['used_disk'],
                                                used_percent = sys_info['disk_info']['disk_percent']),
            cpu_load=monitor_info_pb2.cpu__load__pb2.CpuLoad(load_avg_1=0.5, load_avg_3=0.4, load_avg_15=0.3),
            cpu_stat=monitor_info_pb2.cpu__stat__pb2.CpuStat(cpu_percent=sys_info['cpu_info']['cpu_percent'], cpu_logic_num=sys_info['cpu_info']['cpu_count'],
                                              each_core_utilization_rate=sys_info['cpu_info']['cpu_each_core_uesd']),
            mem_info=monitor_info_pb2.mem__info__pb2.MemInfo(total=sys_info['memory_info']['total_memory'], avail=sys_info['memory_info']['available_memory']),
            net_info=monitor_info_pb2.net__info__pb2.NetInfo(send_rate=sys_info['network_info']['bytes_sent'], rcv_rate=sys_info['network_info']['bytes_recv']),
            accountnum=self.config_data['accountnum'],
            time=monitor_info_pb2.time__pb2.Time(timeymd=sys_info['time']['timeymd'] , timehms = sys_info['time']['timehms']),
            os=monitor_info_pb2.os__pb2.OS(os_name=sys_info['os_info']['system'], os_startup_time=sys_info['os_info']['boot_time']),
            machine_name=self.config_data['machine_name']
        )
        for gpu_info in sys_info['gpu_info']:
            gpu = monitor_info_pb2.gpu__info__pb2.GpuInfo(id = gpu_info['gpu_id'],gpu_name=gpu_info['gpu_name'], gpu_mem_total=int(gpu_info['total_memory']),
                                                gpu_mem_used = int(gpu_info['used_memory']),temperture = gpu_info['temperature'],
                                                gpu_mem_utilize = gpu_info['memory_utilization'] )
            monitor_info.gpu_info.append(gpu)

        return monitor_info

    def SetMonitorInfo(self,service_name, method_name):
        request = self.get_monitor_info(self.monitor.get_system_info())
        response_data = self.channel.callMethod(service_name, method_name, request)

if __name__ == '__main__':
    # 配置日志
    yaml_file_path = 'config.yaml'
    with open(yaml_file_path, 'r', encoding='utf-8') as file:
        # 解析 YAML 文件内容并保存到字典中
        config_data = yaml.safe_load(file)
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
        filename= "gmpanel_{}_{}.log".format(config_data['accountnum'], config_data['machine_name']),
        encoding='utf-8'
    )
    logging.info("This is python client of gmpanel")
    rpc_client = RpcClient(config_data)
    sleep_time = config_data['sleep_time']
    method_name = b"SetMonitorInfo"
    service_name = b"MonitorManager"
    while True:
        rpc_client.SetMonitorInfo(service_name, method_name)
        time.sleep(sleep_time)



