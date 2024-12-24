#!/usr/bin/env bash
#�?动docker容器的bash脚本�?
MONITOR_HOME_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )"

display=""
if [ -z ${DISPLAY} ];then
    display=":1"
else
    display="${DISPLAY}"
fi

local_host="$(hostname)"
user="${USER}"
uid="$(id -u)"
group="$(id -g -n)"
gid="$(id -g)"


echo "stop and rm docker" 
docker stop server_monitor > /dev/null
docker rm -v -f server_monitor > /dev/null

echo "start docker"
docker run -it -d \
--name server_monitor \
-e DISPLAY=$display \
--privileged=true \
-e DOCKER_USER="${user}" \
-e USER="${user}" \
-e DOCKER_USER_ID="${uid}" \
-e DOCKER_GRP="${group}" \
-e DOCKER_GRP_ID="${gid}" \
-e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
-e LANG="C.UTF-8" \
-v ${MONITOR_HOME_DIR}:/home/work \
-v ${XDG_RUNTIME_DIR}:${XDG_RUNTIME_DIR} \
--gpus all \
--net host \
--add-host=host.docker.internal:host-gateway  \
linux:mprpc_monitor
