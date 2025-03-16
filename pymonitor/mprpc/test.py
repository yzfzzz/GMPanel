import struct

# 假设 header_size 是一个整数
header_size = 35

# 将 header_size 转换为 4 字节的二进制数据（小端字节序）
header_size_bytes = struct.pack('<I', header_size)  # '<I' 表示小端无符号 32 位整型

# 假设 rpc_header_str 和 args_str 是字节类型
rpc_header_str = b"rpc_header"  # 示例数据
args_str = b"args"  # 示例数据

# 构建 send_rpc_str
send_rpc_str = bytearray()
send_rpc_str.extend(header_size_bytes)  # 插入 header_size 的二进制表示
send_rpc_str.extend(rpc_header_str)  # 追加 rpc_header_str
send_rpc_str.extend(args_str)  # 追加 args_str

# 如果需要将 send_rpc_str 转换为 bytes
send_rpc_str = bytes(send_rpc_str)

print(send_rpc_str)