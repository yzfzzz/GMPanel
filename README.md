# GPU��������Ⱥ���ϵͳ

## ?��Ŀ����

�������������ѧϰ�ģ�����3̨������10�����Կ��������ڹ����ƣ���������ػ��ȡ���ˣ���Ҫһ�׼��ϵͳ��ͬʱ��ض�̨��������cpu/gpu��Ϣ��������λ���⣬����崻�



## ?����

- [x] �ͻ���GUI����
- [x] ֧�ּ��cpu���洢��gpu���������Ϣ
- [x] ֧��ͬʱ��ض�̨������
- [x] ֧����־�������м�¼
- [ ] mysql���ݿⱣ����ʷ����
- [ ] �ͻ��˽�������

.......



## ?��ɫ

1. ����SOA�ܹ�����ضˡ�����ˡ��ͻ��˻���Ӱ��
2. �������Ƶ� [mprpc](https://github.com/yzfzzz/mprpc) ΢����ܹ���������������rpc��ܣ���grpc��brpc�ȣ�
3. dockerһ��������ȥ���ӵĻ�������
4. ֧��ͬʱ��ط�������Ⱥ
5. ��ȫ������¶���ض˿�



## ?�ļ�

| �ļ���                                                       | ˵��              |
| ------------------------------------------------------------ | ----------------- |
| [bin](https://github.com/yzfzzz/linux-monitor/tree/main/bin) | ��ִ���ļ�        |
| [display_monitor](https://github.com/yzfzzz/linux-monitor/tree/main/display_monitor) | �ͻ���            |
| [docker](https://github.com/yzfzzz/linux-monitor/tree/main/docker) | ��������          |
| [mprpc](https://github.com/yzfzzz/linux-monitor/tree/main/mprpc) | ���Ƶ�rpc���     |
| [proto](https://github.com/yzfzzz/linux-monitor/tree/main/proto) | ���ݴ����ʽ      |
| [rpc_manager](https://github.com/yzfzzz/linux-monitor/tree/main/rpc_manager) | rpc�ͻ��ˡ������ |
| [test/python_cpp_pipe](https://github.com/yzfzzz/linux-monitor/tree/main/test/python_cpp_pipe) | ���ܿ�������      |
| [test_monitor](https://github.com/yzfzzz/linux-monitor/tree/main/test_monitor) | ��ض�            |
| [tool](https://github.com/yzfzzz/linux-monitor/tree/main/tool) | һЩ���õ�С����  |
| [doc](https://github.com/yzfzzz/linux-monitor/tree/main/doc) | ��Ŀ�ĵ�/ͼƬ����  |




## ��ô�ã�

?[������]

�����ú�docker���������ɾ��񣬾�����ӦΪ linux:mprpc_monitor

Ȼ��

```shell
./docker/scripts/monitor_docker_run.sh
./docker/scripts/monitor_docker_into.sh
./docker/build/install/start.sh
```

һ�����û���?

Ȼ�����з���˺ͼ�ض�

```shell
cd bin
./server -i address.conf & ./run.sh 
```

������ն����пͻ��ˣ���Ҫͼ�λ����棩

```shell
./display -i address.conf
```

