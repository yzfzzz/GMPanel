# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: query.proto
# Protobuf Python Version: 5.29.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    29,
    0,
    '',
    'query.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0bquery.proto\x12\rmonitor.proto\"\x1b\n\x0cQueryMessage\x12\x0b\n\x03sql\x18\x01 \x01(\t\"\x9d\x02\n\tQueryData\x12\x0f\n\x07gpu_num\x18\x01 \x01(\x05\x12\x10\n\x08gpu_name\x18\x02 \x01(\t\x12\x14\n\x0cgpu_used_mem\x18\x03 \x01(\x05\x12\x15\n\rgpu_total_mem\x18\x04 \x01(\x05\x12\x14\n\x0cgpu_avg_util\x18\x05 \x01(\x05\x12\x16\n\x0e\x63pu_load_avg_1\x18\x06 \x01(\x02\x12\x16\n\x0e\x63pu_load_avg_3\x18\x07 \x01(\x02\x12\x17\n\x0f\x63pu_load_avg_15\x18\x08 \x01(\x02\x12\x10\n\x08mem_used\x18\t \x01(\x02\x12\x11\n\tmem_total\x18\n \x01(\x02\x12\x15\n\rnet_send_rate\x18\x0b \x01(\x02\x12\x14\n\x0cnet_rcv_rate\x18\x0c \x01(\x02\x12\x0f\n\x07timehms\x18\r \x01(\t\"<\n\x0cQueryResults\x12,\n\nquery_data\x18\x01 \x03(\x0b\x32\x18.monitor.proto.QueryDatab\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'query_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_QUERYMESSAGE']._serialized_start=30
  _globals['_QUERYMESSAGE']._serialized_end=57
  _globals['_QUERYDATA']._serialized_start=60
  _globals['_QUERYDATA']._serialized_end=345
  _globals['_QUERYRESULTS']._serialized_start=347
  _globals['_QUERYRESULTS']._serialized_end=407
# @@protoc_insertion_point(module_scope)
