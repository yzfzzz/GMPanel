#-- coding:GBK --
from pynvml import *
nvmlInit()

print("Driver Version:", nvmlSystemGetDriverVersion())
deviceCount = nvmlDeviceGetCount()
for i in range(deviceCount):
    handle = nvmlDeviceGetHandleByIndex(i)
    print(type(nvmlDeviceGetName(handle)))
    print("Device", i, ":", nvmlDeviceGetName(handle))
    # Use handle to get device stats
    memory_info = nvmlDeviceGetMemoryInfo(handle)
    utilization = nvmlDeviceGetUtilizationRates(handle)
    # Report device stats
    #�鿴�Դ桢�¶ȡ����ȡ���Դ
    print(type(memory_info.total))
    print("Memory Total: ",memory_info.total)
    print("Memory Free: ",memory_info.free)
    print("Memory Used: ",memory_info.used)
    print("GPU Utilization:", utilization.gpu)
    print("Memory Utilization:", utilization.memory)
    print(type(nvmlDeviceGetTemperature(handle,NVML_TEMPERATURE_GPU)))
    print("Temperature is: ",nvmlDeviceGetTemperature(handle,NVML_TEMPERATURE_GPU))
    print(type(nvmlDeviceGetFanSpeed(handle)))
    print("Fan speed is: ", nvmlDeviceGetFanSpeed(handle))
    print("Power ststus: ",nvmlDeviceGetPowerState(handle))

#���Ҫ�رչ�����
nvmlShutdown()