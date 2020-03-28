// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pulse/cdm/bind/Conditions.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_pulse_2fcdm_2fbind_2fConditions_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_pulse_2fcdm_2fbind_2fConditions_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010001 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_pulse_2fcdm_2fbind_2fConditions_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_pulse_2fcdm_2fbind_2fConditions_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_pulse_2fcdm_2fbind_2fConditions_2eproto;
namespace pulse {
namespace cdm {
namespace bind {
class ConditionData;
class ConditionDataDefaultTypeInternal;
extern ConditionDataDefaultTypeInternal _ConditionData_default_instance_;
}  // namespace bind
}  // namespace cdm
}  // namespace pulse
PROTOBUF_NAMESPACE_OPEN
template<> ::pulse::cdm::bind::ConditionData* Arena::CreateMaybeMessage<::pulse::cdm::bind::ConditionData>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace pulse {
namespace cdm {
namespace bind {

// ===================================================================

class ConditionData :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:pulse.cdm.bind.ConditionData) */ {
 public:
  ConditionData();
  virtual ~ConditionData();

  ConditionData(const ConditionData& from);
  ConditionData(ConditionData&& from) noexcept
    : ConditionData() {
    *this = ::std::move(from);
  }

  inline ConditionData& operator=(const ConditionData& from) {
    CopyFrom(from);
    return *this;
  }
  inline ConditionData& operator=(ConditionData&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
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
  static const ConditionData& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ConditionData* internal_default_instance() {
    return reinterpret_cast<const ConditionData*>(
               &_ConditionData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ConditionData& a, ConditionData& b) {
    a.Swap(&b);
  }
  inline void Swap(ConditionData* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ConditionData* New() const final {
    return CreateMaybeMessage<ConditionData>(nullptr);
  }

  ConditionData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ConditionData>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ConditionData& from);
  void MergeFrom(const ConditionData& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ConditionData* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "pulse.cdm.bind.ConditionData";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_pulse_2fcdm_2fbind_2fConditions_2eproto);
    return ::descriptor_table_pulse_2fcdm_2fbind_2fConditions_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCommentFieldNumber = 1,
  };
  // string Comment = 1;
  void clear_comment();
  const std::string& comment() const;
  void set_comment(const std::string& value);
  void set_comment(std::string&& value);
  void set_comment(const char* value);
  void set_comment(const char* value, size_t size);
  std::string* mutable_comment();
  std::string* release_comment();
  void set_allocated_comment(std::string* comment);
  private:
  const std::string& _internal_comment() const;
  void _internal_set_comment(const std::string& value);
  std::string* _internal_mutable_comment();
  public:

  // @@protoc_insertion_point(class_scope:pulse.cdm.bind.ConditionData)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr comment_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_pulse_2fcdm_2fbind_2fConditions_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ConditionData

// string Comment = 1;
inline void ConditionData::clear_comment() {
  comment_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& ConditionData::comment() const {
  // @@protoc_insertion_point(field_get:pulse.cdm.bind.ConditionData.Comment)
  return _internal_comment();
}
inline void ConditionData::set_comment(const std::string& value) {
  _internal_set_comment(value);
  // @@protoc_insertion_point(field_set:pulse.cdm.bind.ConditionData.Comment)
}
inline std::string* ConditionData::mutable_comment() {
  // @@protoc_insertion_point(field_mutable:pulse.cdm.bind.ConditionData.Comment)
  return _internal_mutable_comment();
}
inline const std::string& ConditionData::_internal_comment() const {
  return comment_.GetNoArena();
}
inline void ConditionData::_internal_set_comment(const std::string& value) {
  
  comment_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void ConditionData::set_comment(std::string&& value) {
  
  comment_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:pulse.cdm.bind.ConditionData.Comment)
}
inline void ConditionData::set_comment(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  comment_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:pulse.cdm.bind.ConditionData.Comment)
}
inline void ConditionData::set_comment(const char* value, size_t size) {
  
  comment_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:pulse.cdm.bind.ConditionData.Comment)
}
inline std::string* ConditionData::_internal_mutable_comment() {
  
  return comment_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* ConditionData::release_comment() {
  // @@protoc_insertion_point(field_release:pulse.cdm.bind.ConditionData.Comment)
  
  return comment_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void ConditionData::set_allocated_comment(std::string* comment) {
  if (comment != nullptr) {
    
  } else {
    
  }
  comment_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), comment);
  // @@protoc_insertion_point(field_set_allocated:pulse.cdm.bind.ConditionData.Comment)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace bind
}  // namespace cdm
}  // namespace pulse

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_pulse_2fcdm_2fbind_2fConditions_2eproto
