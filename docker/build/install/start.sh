. /tmp/install/zookeeper/zookeeper-3.4.10/bin/zkServer.sh start
sed -a 'ulimit-c unlimited' /etc/profile
echo /home/work/bin/coredump/core.%e.%p> /proc/sys/kernel/core_pattern
mv /usr/bin/python /usr/bin/python.bak
ln -s /usr/bin/python3.6 /usr/bin/python
cd /tmp
curl https://bootstrap.pypa.io/pip/3.6/get-pip.py -o get-pip.py
python get-pip.py
pip install nvidia-ml-py3
apt-get install libmysqlclient-dev -y
DEBIAN_FRONTEND=noninteractive apt install -y tzdata
export TZ=Asia/Shanghai
cp -r /home/work/docker/build/install/fonts/* /usr/share/fonts/
apt-get -y install fontconfig xfonts-utils
apt-get install libfmt-dev
fc-cache -f -v