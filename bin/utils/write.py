#-- coding:UTF-8 --
#!/usr/bin/python3
import json
import os
import time
from pynvml import *

pipe_path = ""
nvmlInit()
# 打开JSON文件，使用'with'语句可以确保文件正确关闭，避免资源泄露
with open('./utils/monitor_config.json', 'r') as file:
    monitor_config = json.load(file)
    pipe_path = "./utils/"+monitor_config["account_num"]+"_"+monitor_config["machine_name"]+".fifo"

if not os.path.exists(pipe_path):
    os.mkfifo(pipe_path)

pipe = os.open(pipe_path, os.O_WRONLY)
bytesToMB = 1024*1024
while(True):

    deviceCount = nvmlDeviceGetCount()
    data = []
    for i in range(deviceCount):
        handle = nvmlDeviceGetHandleByIndex(i)
        memory_info = nvmlDeviceGetMemoryInfo(handle)
        utilization = nvmlDeviceGetUtilizationRates(handle)
        # ! 12288 = 12884901888/(1024*1024)
        data.append({
            "ID": "GPU"+str(i),
            "DeviceName": nvmlDeviceGetName(handle).decode("utf-8"),
            "MemoryTotal": memory_info.total/bytesToMB,
            "MemoryFree": memory_info.free/bytesToMB,
            "MemoryUsed": memory_info.used/bytesToMB,
            "MemoryUtilization": utilization.memory,
            "GPUUtilization": utilization.gpu,
            "Temperature": nvmlDeviceGetTemperature(handle,NVML_TEMPERATURE_GPU),
            # "FanSpeed": nvmlDeviceGetFanSpeed(handle),
            "FanSpeed": 100,
            "PowerStstus": nvmlDeviceGetPowerState(handle)
        })

    json_data = json.dumps(data)
    # print(json_data)
    max_attempts = 3
    attempt_count = 0
    while attempt_count < max_attempts:
        try:
            if os.path.getsize(pipe) == 0:
                os.write(pipe, json_data.encode("utf-8"))
            time.sleep(3)
            break
        except BrokenPipeError as e:
            print("Accidentally disconnected!", e)
            attempt_count += 1
            time.sleep(2*attempt_count)
    if attempt_count == max_attempts:
           print("Timeout! Program ending!")
           os._exit(0)




