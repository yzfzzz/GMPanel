// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gpu_info.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_gpu_5finfo_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_gpu_5finfo_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3014000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3014000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_gpu_5finfo_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_gpu_5finfo_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_gpu_5finfo_2eproto;
namespace monitor {
namespace proto {
class GpuInfo;
class GpuInfoDefaultTypeInternal;
extern GpuInfoDefaultTypeInternal _GpuInfo_default_instance_;
}  // namespace proto
}  // namespace monitor
PROTOBUF_NAMESPACE_OPEN
template<> ::monitor::proto::GpuInfo* Arena::CreateMaybeMessage<::monitor::proto::GpuInfo>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace monitor {
namespace proto {

// ===================================================================

class GpuInfo PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:monitor.proto.GpuInfo) */ {
 public:
  inline GpuInfo() : GpuInfo(nullptr) {}
  virtual ~GpuInfo();

  GpuInfo(const GpuInfo& from);
  GpuInfo(GpuInfo&& from) noexcept
    : GpuInfo() {
    *this = ::std::move(from);
  }

  inline GpuInfo& operator=(const GpuInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline GpuInfo& operator=(GpuInfo&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const GpuInfo& default_instance();

  static inline const GpuInfo* internal_default_instance() {
    return reinterpret_cast<const GpuInfo*>(
               &_GpuInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(GpuInfo& a, GpuInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(GpuInfo* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(GpuInfo* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline GpuInfo* New() const final {
    return CreateMaybeMessage<GpuInfo>(nullptr);
  }

  GpuInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<GpuInfo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const GpuInfo& from);
  void MergeFrom(const GpuInfo& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(GpuInfo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "monitor.proto.GpuInfo";
  }
  protected:
  explicit GpuInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_gpu_5finfo_2eproto);
    return ::descriptor_table_gpu_5finfo_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGpuNameFieldNumber = 1,
    kIdFieldNumber = 10,
    kGpuMemTotalFieldNumber = 2,
    kGpuMemFreeFieldNumber = 3,
    kGpuMemUsedFieldNumber = 4,
    kGpuMemUtilizeFieldNumber = 5,
    kGpuUtilizeFieldNumber = 6,
    kTempertureFieldNumber = 7,
    kFanSpeedFieldNumber = 8,
    kPowerStatFieldNumber = 9,
  };
  // string gpu_name = 1;
  void clear_gpu_name();
  const std::string& gpu_name() const;
  void set_gpu_name(const std::string& value);
  void set_gpu_name(std::string&& value);
  void set_gpu_name(const char* value);
  void set_gpu_name(const char* value, size_t size);
  std::string* mutable_gpu_name();
  std::string* release_gpu_name();
  void set_allocated_gpu_name(std::string* gpu_name);
  private:
  const std::string& _internal_gpu_name() const;
  void _internal_set_gpu_name(const std::string& value);
  std::string* _internal_mutable_gpu_name();
  public:

  // string id = 10;
  void clear_id();
  const std::string& id() const;
  void set_id(const std::string& value);
  void set_id(std::string&& value);
  void set_id(const char* value);
  void set_id(const char* value, size_t size);
  std::string* mutable_id();
  std::string* release_id();
  void set_allocated_id(std::string* id);
  private:
  const std::string& _internal_id() const;
  void _internal_set_id(const std::string& value);
  std::string* _internal_mutable_id();
  public:

  // int32 gpu_mem_total = 2;
  void clear_gpu_mem_total();
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_total() const;
  void set_gpu_mem_total(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_gpu_mem_total() const;
  void _internal_set_gpu_mem_total(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 gpu_mem_free = 3;
  void clear_gpu_mem_free();
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_free() const;
  void set_gpu_mem_free(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_gpu_mem_free() const;
  void _internal_set_gpu_mem_free(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 gpu_mem_used = 4;
  void clear_gpu_mem_used();
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_used() const;
  void set_gpu_mem_used(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_gpu_mem_used() const;
  void _internal_set_gpu_mem_used(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 gpu_mem_utilize = 5;
  void clear_gpu_mem_utilize();
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_utilize() const;
  void set_gpu_mem_utilize(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_gpu_mem_utilize() const;
  void _internal_set_gpu_mem_utilize(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 gpu_utilize = 6;
  void clear_gpu_utilize();
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_utilize() const;
  void set_gpu_utilize(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_gpu_utilize() const;
  void _internal_set_gpu_utilize(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 temperture = 7;
  void clear_temperture();
  ::PROTOBUF_NAMESPACE_ID::int32 temperture() const;
  void set_temperture(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_temperture() const;
  void _internal_set_temperture(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 fan_speed = 8;
  void clear_fan_speed();
  ::PROTOBUF_NAMESPACE_ID::int32 fan_speed() const;
  void set_fan_speed(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_fan_speed() const;
  void _internal_set_fan_speed(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 power_stat = 9;
  void clear_power_stat();
  ::PROTOBUF_NAMESPACE_ID::int32 power_stat() const;
  void set_power_stat(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_power_stat() const;
  void _internal_set_power_stat(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:monitor.proto.GpuInfo)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr gpu_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr id_;
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_total_;
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_free_;
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_used_;
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_mem_utilize_;
  ::PROTOBUF_NAMESPACE_ID::int32 gpu_utilize_;
  ::PROTOBUF_NAMESPACE_ID::int32 temperture_;
  ::PROTOBUF_NAMESPACE_ID::int32 fan_speed_;
  ::PROTOBUF_NAMESPACE_ID::int32 power_stat_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_gpu_5finfo_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GpuInfo

// string id = 10;
inline void GpuInfo::clear_id() {
  id_.ClearToEmpty();
}
inline const std::string& GpuInfo::id() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.id)
  return _internal_id();
}
inline void GpuInfo::set_id(const std::string& value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.id)
}
inline std::string* GpuInfo::mutable_id() {
  // @@protoc_insertion_point(field_mutable:monitor.proto.GpuInfo.id)
  return _internal_mutable_id();
}
inline const std::string& GpuInfo::_internal_id() const {
  return id_.Get();
}
inline void GpuInfo::_internal_set_id(const std::string& value) {
  
  id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void GpuInfo::set_id(std::string&& value) {
  
  id_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:monitor.proto.GpuInfo.id)
}
inline void GpuInfo::set_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:monitor.proto.GpuInfo.id)
}
inline void GpuInfo::set_id(const char* value,
    size_t size) {
  
  id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:monitor.proto.GpuInfo.id)
}
inline std::string* GpuInfo::_internal_mutable_id() {
  
  return id_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* GpuInfo::release_id() {
  // @@protoc_insertion_point(field_release:monitor.proto.GpuInfo.id)
  return id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void GpuInfo::set_allocated_id(std::string* id) {
  if (id != nullptr) {
    
  } else {
    
  }
  id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:monitor.proto.GpuInfo.id)
}

// string gpu_name = 1;
inline void GpuInfo::clear_gpu_name() {
  gpu_name_.ClearToEmpty();
}
inline const std::string& GpuInfo::gpu_name() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.gpu_name)
  return _internal_gpu_name();
}
inline void GpuInfo::set_gpu_name(const std::string& value) {
  _internal_set_gpu_name(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.gpu_name)
}
inline std::string* GpuInfo::mutable_gpu_name() {
  // @@protoc_insertion_point(field_mutable:monitor.proto.GpuInfo.gpu_name)
  return _internal_mutable_gpu_name();
}
inline const std::string& GpuInfo::_internal_gpu_name() const {
  return gpu_name_.Get();
}
inline void GpuInfo::_internal_set_gpu_name(const std::string& value) {
  
  gpu_name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void GpuInfo::set_gpu_name(std::string&& value) {
  
  gpu_name_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:monitor.proto.GpuInfo.gpu_name)
}
inline void GpuInfo::set_gpu_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  gpu_name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:monitor.proto.GpuInfo.gpu_name)
}
inline void GpuInfo::set_gpu_name(const char* value,
    size_t size) {
  
  gpu_name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:monitor.proto.GpuInfo.gpu_name)
}
inline std::string* GpuInfo::_internal_mutable_gpu_name() {
  
  return gpu_name_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* GpuInfo::release_gpu_name() {
  // @@protoc_insertion_point(field_release:monitor.proto.GpuInfo.gpu_name)
  return gpu_name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void GpuInfo::set_allocated_gpu_name(std::string* gpu_name) {
  if (gpu_name != nullptr) {
    
  } else {
    
  }
  gpu_name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), gpu_name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:monitor.proto.GpuInfo.gpu_name)
}

// int32 gpu_mem_total = 2;
inline void GpuInfo::clear_gpu_mem_total() {
  gpu_mem_total_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_gpu_mem_total() const {
  return gpu_mem_total_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::gpu_mem_total() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.gpu_mem_total)
  return _internal_gpu_mem_total();
}
inline void GpuInfo::_internal_set_gpu_mem_total(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  gpu_mem_total_ = value;
}
inline void GpuInfo::set_gpu_mem_total(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_gpu_mem_total(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.gpu_mem_total)
}

// int32 gpu_mem_free = 3;
inline void GpuInfo::clear_gpu_mem_free() {
  gpu_mem_free_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_gpu_mem_free() const {
  return gpu_mem_free_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::gpu_mem_free() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.gpu_mem_free)
  return _internal_gpu_mem_free();
}
inline void GpuInfo::_internal_set_gpu_mem_free(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  gpu_mem_free_ = value;
}
inline void GpuInfo::set_gpu_mem_free(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_gpu_mem_free(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.gpu_mem_free)
}

// int32 gpu_mem_used = 4;
inline void GpuInfo::clear_gpu_mem_used() {
  gpu_mem_used_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_gpu_mem_used() const {
  return gpu_mem_used_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::gpu_mem_used() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.gpu_mem_used)
  return _internal_gpu_mem_used();
}
inline void GpuInfo::_internal_set_gpu_mem_used(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  gpu_mem_used_ = value;
}
inline void GpuInfo::set_gpu_mem_used(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_gpu_mem_used(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.gpu_mem_used)
}

// int32 gpu_mem_utilize = 5;
inline void GpuInfo::clear_gpu_mem_utilize() {
  gpu_mem_utilize_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_gpu_mem_utilize() const {
  return gpu_mem_utilize_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::gpu_mem_utilize() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.gpu_mem_utilize)
  return _internal_gpu_mem_utilize();
}
inline void GpuInfo::_internal_set_gpu_mem_utilize(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  gpu_mem_utilize_ = value;
}
inline void GpuInfo::set_gpu_mem_utilize(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_gpu_mem_utilize(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.gpu_mem_utilize)
}

// int32 gpu_utilize = 6;
inline void GpuInfo::clear_gpu_utilize() {
  gpu_utilize_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_gpu_utilize() const {
  return gpu_utilize_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::gpu_utilize() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.gpu_utilize)
  return _internal_gpu_utilize();
}
inline void GpuInfo::_internal_set_gpu_utilize(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  gpu_utilize_ = value;
}
inline void GpuInfo::set_gpu_utilize(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_gpu_utilize(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.gpu_utilize)
}

// int32 temperture = 7;
inline void GpuInfo::clear_temperture() {
  temperture_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_temperture() const {
  return temperture_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::temperture() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.temperture)
  return _internal_temperture();
}
inline void GpuInfo::_internal_set_temperture(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  temperture_ = value;
}
inline void GpuInfo::set_temperture(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_temperture(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.temperture)
}

// int32 fan_speed = 8;
inline void GpuInfo::clear_fan_speed() {
  fan_speed_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_fan_speed() const {
  return fan_speed_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::fan_speed() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.fan_speed)
  return _internal_fan_speed();
}
inline void GpuInfo::_internal_set_fan_speed(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  fan_speed_ = value;
}
inline void GpuInfo::set_fan_speed(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_fan_speed(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.fan_speed)
}

// int32 power_stat = 9;
inline void GpuInfo::clear_power_stat() {
  power_stat_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::_internal_power_stat() const {
  return power_stat_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GpuInfo::power_stat() const {
  // @@protoc_insertion_point(field_get:monitor.proto.GpuInfo.power_stat)
  return _internal_power_stat();
}
inline void GpuInfo::_internal_set_power_stat(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  power_stat_ = value;
}
inline void GpuInfo::set_power_stat(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_power_stat(value);
  // @@protoc_insertion_point(field_set:monitor.proto.GpuInfo.power_stat)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace monitor

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_gpu_5finfo_2eproto