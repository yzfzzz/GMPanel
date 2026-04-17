FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

SHELL ["/bin/bash", "-c"]

# 1. 换源
COPY apt/sources.list /etc/apt/

# 2. 合并所有 apt 安装层，并在同一层末尾清理缓存
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    # --- 系统工具与基础 ---
    apt-utils \
    tzdata \
    curl \
    git \
    vim \
    htop \
    zip \
    net-tools \
    openssh-server \
    # --- 编译工具链 ---
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    gdb \
    # --- 核心依赖库 ---
    libssl-dev \
    libc-ares-dev \
    libgoogle-glog-dev \
    libboost-all-dev \
    libfmt-dev \
    libyaml-cpp-dev \
    libmysqlclient-dev \
    # --- gRPC 与 Protobuf (22.04源自带较新版本) ---
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    # --- ZooKeeper ---
    libzookeeper-mt-dev \
    # --- Qt5 相关 ---
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5charts5 \
    libqt5charts5-dev \
    # --- UI/X11 依赖 ---
    libx11-xcb1 \
    libfreetype6 \
    libdbus-1-3 \
    libfontconfig1 \
    libxkbcommon0 \
    libxkbcommon-x11-0 \
    # --- Java ---
    openjdk-11-jdk \
    # --- Python3 ---
    python3-dev \
    python3-pip \
    python3-all-dev \
    stress && \
    # 建立 python/pip 软链接
    ln -sf /usr/bin/python3 /usr/bin/python && \
    ln -sf /usr/bin/pip3 /usr/bin/pip && \
    # 清理 apt 缓存，减小镜像体积
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

COPY install/muduo /tmp/install/muduo
RUN /tmp/install/muduo/install_muduo.sh

RUN apt-get update && apt-get install -y \
    pkg-config \
    clangd \
    clang-format


CMD ["tail", "-f", "/dev/null"]



