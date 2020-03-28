// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pulse/cdm/bind/Inhaler.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_pulse_2fcdm_2fbind_2fInhaler_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_pulse_2fcdm_2fbind_2fInhaler_2eproto

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
#include "pulse/cdm/bind/Enums.pb.h"
#include "pulse/cdm/bind/Properties.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_pulse_2fcdm_2fbind_2fInhaler_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_pulse_2fcdm_2fbind_2fInhaler_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_pulse_2fcdm_2fbind_2fInhaler_2eproto;
namespace pulse {
namespace cdm {
namespace bind {
class InhalerData;
class InhalerDataDefaultTypeInternal;
extern InhalerDataDefaultTypeInternal _InhalerData_default_instance_;
}  // namespace bind
}  // namespace cdm
}  // namespace pulse
PROTOBUF_NAMESPACE_OPEN
template<> ::pulse::cdm::bind::InhalerData* Arena::CreateMaybeMessage<::pulse::cdm::bind::InhalerData>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace pulse {
namespace cdm {
namespace bind {

// ===================================================================

class InhalerData :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:pulse.cdm.bind.InhalerData) */ {
 public:
  InhalerData();
  virtual ~InhalerData();

  InhalerData(const InhalerData& from);
  InhalerData(InhalerData&& from) noexcept
    : InhalerData() {
    *this = ::std::move(from);
  }

  inline InhalerData& operator=(const InhalerData& from) {
    CopyFrom(from);
    return *this;
  }
  inline InhalerData& operator=(InhalerData&& from) noexcept {
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
  static const InhalerData& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const InhalerData* internal_default_instance() {
    return reinterpret_cast<const InhalerData*>(
               &_InhalerData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(InhalerData& a, InhalerData& b) {
    a.Swap(&b);
  }
  inline void Swap(InhalerData* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline InhalerData* New() const final {
    return CreateMaybeMessage<InhalerData>(nullptr);
  }

  InhalerData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<InhalerData>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const InhalerData& from);
  void MergeFrom(const InhalerData& from);
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
  void InternalSwap(InhalerData* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "pulse.cdm.bind.InhalerData";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_pulse_2fcdm_2fbind_2fInhaler_2eproto);
    return ::descriptor_table_pulse_2fcdm_2fbind_2fInhaler_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSubstanceFieldNumber = 2,
    kMeteredDoseFieldNumber = 3,
    kNozzleLossFieldNumber = 4,
    kSpacerVolumeFieldNumber = 5,
    kStateFieldNumber = 1,
  };
  // string Substance = 2;
  void clear_substance();
  const std::string& substance() const;
  void set_substance(const std::string& value);
  void set_substance(std::string&& value);
  void set_substance(const char* value);
  void set_substance(const char* value, size_t size);
  std::string* mutable_substance();
  std::string* release_substance();
  void set_allocated_substance(std::string* substance);
  private:
  const std::string& _internal_substance() const;
  void _internal_set_substance(const std::string& value);
  std::string* _internal_mutable_substance();
  public:

  // .pulse.cdm.bind.ScalarMassData MeteredDose = 3;
  bool has_metereddose() const;
  private:
  bool _internal_has_metereddose() const;
  public:
  void clear_metereddose();
  const ::pulse::cdm::bind::ScalarMassData& metereddose() const;
  ::pulse::cdm::bind::ScalarMassData* release_metereddose();
  ::pulse::cdm::bind::ScalarMassData* mutable_metereddose();
  void set_allocated_metereddose(::pulse::cdm::bind::ScalarMassData* metereddose);
  private:
  const ::pulse::cdm::bind::ScalarMassData& _internal_metereddose() const;
  ::pulse::cdm::bind::ScalarMassData* _internal_mutable_metereddose();
  public:

  // .pulse.cdm.bind.Scalar0To1Data NozzleLoss = 4;
  bool has_nozzleloss() const;
  private:
  bool _internal_has_nozzleloss() const;
  public:
  void clear_nozzleloss();
  const ::pulse::cdm::bind::Scalar0To1Data& nozzleloss() const;
  ::pulse::cdm::bind::Scalar0To1Data* release_nozzleloss();
  ::pulse::cdm::bind::Scalar0To1Data* mutable_nozzleloss();
  void set_allocated_nozzleloss(::pulse::cdm::bind::Scalar0To1Data* nozzleloss);
  private:
  const ::pulse::cdm::bind::Scalar0To1Data& _internal_nozzleloss() const;
  ::pulse::cdm::bind::Scalar0To1Data* _internal_mutable_nozzleloss();
  public:

  // .pulse.cdm.bind.ScalarVolumeData SpacerVolume = 5;
  bool has_spacervolume() const;
  private:
  bool _internal_has_spacervolume() const;
  public:
  void clear_spacervolume();
  const ::pulse::cdm::bind::ScalarVolumeData& spacervolume() const;
  ::pulse::cdm::bind::ScalarVolumeData* release_spacervolume();
  ::pulse::cdm::bind::ScalarVolumeData* mutable_spacervolume();
  void set_allocated_spacervolume(::pulse::cdm::bind::ScalarVolumeData* spacervolume);
  private:
  const ::pulse::cdm::bind::ScalarVolumeData& _internal_spacervolume() const;
  ::pulse::cdm::bind::ScalarVolumeData* _internal_mutable_spacervolume();
  public:

  // .pulse.cdm.bind.eSwitch State = 1;
  void clear_state();
  ::pulse::cdm::bind::eSwitch state() const;
  void set_state(::pulse::cdm::bind::eSwitch value);
  private:
  ::pulse::cdm::bind::eSwitch _internal_state() const;
  void _internal_set_state(::pulse::cdm::bind::eSwitch value);
  public:

  // @@protoc_insertion_point(class_scope:pulse.cdm.bind.InhalerData)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr substance_;
  ::pulse::cdm::bind::ScalarMassData* metereddose_;
  ::pulse::cdm::bind::Scalar0To1Data* nozzleloss_;
  ::pulse::cdm::bind::ScalarVolumeData* spacervolume_;
  int state_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_pulse_2fcdm_2fbind_2fInhaler_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// InhalerData

// .pulse.cdm.bind.eSwitch State = 1;
inline void InhalerData::clear_state() {
  state_ = 0;
}
inline ::pulse::cdm::bind::eSwitch InhalerData::_internal_state() const {
  return static_cast< ::pulse::cdm::bind::eSwitch >(state_);
}
inline ::pulse::cdm::bind::eSwitch InhalerData::state() const {
  // @@protoc_insertion_point(field_get:pulse.cdm.bind.InhalerData.State)
  return _internal_state();
}
inline void InhalerData::_internal_set_state(::pulse::cdm::bind::eSwitch value) {
  
  state_ = value;
}
inline void InhalerData::set_state(::pulse::cdm::bind::eSwitch value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:pulse.cdm.bind.InhalerData.State)
}

// string Substance = 2;
inline void InhalerData::clear_substance() {
  substance_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& InhalerData::substance() const {
  // @@protoc_insertion_point(field_get:pulse.cdm.bind.InhalerData.Substance)
  return _internal_substance();
}
inline void InhalerData::set_substance(const std::string& value) {
  _internal_set_substance(value);
  // @@protoc_insertion_point(field_set:pulse.cdm.bind.InhalerData.Substance)
}
inline std::string* InhalerData::mutable_substance() {
  // @@protoc_insertion_point(field_mutable:pulse.cdm.bind.InhalerData.Substance)
  return _internal_mutable_substance();
}
inline const std::string& InhalerData::_internal_substance() const {
  return substance_.GetNoArena();
}
inline void InhalerData::_internal_set_substance(const std::string& value) {
  
  substance_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void InhalerData::set_substance(std::string&& value) {
  
  substance_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:pulse.cdm.bind.InhalerData.Substance)
}
inline void InhalerData::set_substance(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  substance_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:pulse.cdm.bind.InhalerData.Substance)
}
inline void InhalerData::set_substance(const char* value, size_t size) {
  
  substance_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:pulse.cdm.bind.InhalerData.Substance)
}
inline std::string* InhalerData::_internal_mutable_substance() {
  
  return substance_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* InhalerData::release_substance() {
  // @@protoc_insertion_point(field_release:pulse.cdm.bind.InhalerData.Substance)
  
  return substance_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void InhalerData::set_allocated_substance(std::string* substance) {
  if (substance != nullptr) {
    
  } else {
    
  }
  substance_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), substance);
  // @@protoc_insertion_point(field_set_allocated:pulse.cdm.bind.InhalerData.Substance)
}

// .pulse.cdm.bind.ScalarMassData MeteredDose = 3;
inline bool InhalerData::_internal_has_metereddose() const {
  return this != internal_default_instance() && metereddose_ != nullptr;
}
inline bool InhalerData::has_metereddose() const {
  return _internal_has_metereddose();
}
inline const ::pulse::cdm::bind::ScalarMassData& InhalerData::_internal_metereddose() const {
  const ::pulse::cdm::bind::ScalarMassData* p = metereddose_;
  return p != nullptr ? *p : *reinterpret_cast<const ::pulse::cdm::bind::ScalarMassData*>(
      &::pulse::cdm::bind::_ScalarMassData_default_instance_);
}
inline const ::pulse::cdm::bind::ScalarMassData& InhalerData::metereddose() const {
  // @@protoc_insertion_point(field_get:pulse.cdm.bind.InhalerData.MeteredDose)
  return _internal_metereddose();
}
inline ::pulse::cdm::bind::ScalarMassData* InhalerData::release_metereddose() {
  // @@protoc_insertion_point(field_release:pulse.cdm.bind.InhalerData.MeteredDose)
  
  ::pulse::cdm::bind::ScalarMassData* temp = metereddose_;
  metereddose_ = nullptr;
  return temp;
}
inline ::pulse::cdm::bind::ScalarMassData* InhalerData::_internal_mutable_metereddose() {
  
  if (metereddose_ == nullptr) {
    auto* p = CreateMaybeMessage<::pulse::cdm::bind::ScalarMassData>(GetArenaNoVirtual());
    metereddose_ = p;
  }
  return metereddose_;
}
inline ::pulse::cdm::bind::ScalarMassData* InhalerData::mutable_metereddose() {
  // @@protoc_insertion_point(field_mutable:pulse.cdm.bind.InhalerData.MeteredDose)
  return _internal_mutable_metereddose();
}
inline void InhalerData::set_allocated_metereddose(::pulse::cdm::bind::ScalarMassData* metereddose) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(metereddose_);
  }
  if (metereddose) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      metereddose = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, metereddose, submessage_arena);
    }
    
  } else {
    
  }
  metereddose_ = metereddose;
  // @@protoc_insertion_point(field_set_allocated:pulse.cdm.bind.InhalerData.MeteredDose)
}

// .pulse.cdm.bind.Scalar0To1Data NozzleLoss = 4;
inline bool InhalerData::_internal_has_nozzleloss() const {
  return this != internal_default_instance() && nozzleloss_ != nullptr;
}
inline bool InhalerData::has_nozzleloss() const {
  return _internal_has_nozzleloss();
}
inline const ::pulse::cdm::bind::Scalar0To1Data& InhalerData::_internal_nozzleloss() const {
  const ::pulse::cdm::bind::Scalar0To1Data* p = nozzleloss_;
  return p != nullptr ? *p : *reinterpret_cast<const ::pulse::cdm::bind::Scalar0To1Data*>(
      &::pulse::cdm::bind::_Scalar0To1Data_default_instance_);
}
inline const ::pulse::cdm::bind::Scalar0To1Data& InhalerData::nozzleloss() const {
  // @@protoc_insertion_point(field_get:pulse.cdm.bind.InhalerData.NozzleLoss)
  return _internal_nozzleloss();
}
inline ::pulse::cdm::bind::Scalar0To1Data* InhalerData::release_nozzleloss() {
  // @@protoc_insertion_point(field_release:pulse.cdm.bind.InhalerData.NozzleLoss)
  
  ::pulse::cdm::bind::Scalar0To1Data* temp = nozzleloss_;
  nozzleloss_ = nullptr;
  return temp;
}
inline ::pulse::cdm::bind::Scalar0To1Data* InhalerData::_internal_mutable_nozzleloss() {
  
  if (nozzleloss_ == nullptr) {
    auto* p = CreateMaybeMessage<::pulse::cdm::bind::Scalar0To1Data>(GetArenaNoVirtual());
    nozzleloss_ = p;
  }
  return nozzleloss_;
}
inline ::pulse::cdm::bind::Scalar0To1Data* InhalerData::mutable_nozzleloss() {
  // @@protoc_insertion_point(field_mutable:pulse.cdm.bind.InhalerData.NozzleLoss)
  return _internal_mutable_nozzleloss();
}
inline void InhalerData::set_allocated_nozzleloss(::pulse::cdm::bind::Scalar0To1Data* nozzleloss) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(nozzleloss_);
  }
  if (nozzleloss) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      nozzleloss = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, nozzleloss, submessage_arena);
    }
    
  } else {
    
  }
  nozzleloss_ = nozzleloss;
  // @@protoc_insertion_point(field_set_allocated:pulse.cdm.bind.InhalerData.NozzleLoss)
}

// .pulse.cdm.bind.ScalarVolumeData SpacerVolume = 5;
inline bool InhalerData::_internal_has_spacervolume() const {
  return this != internal_default_instance() && spacervolume_ != nullptr;
}
inline bool InhalerData::has_spacervolume() const {
  return _internal_has_spacervolume();
}
inline const ::pulse::cdm::bind::ScalarVolumeData& InhalerData::_internal_spacervolume() const {
  const ::pulse::cdm::bind::ScalarVolumeData* p = spacervolume_;
  return p != nullptr ? *p : *reinterpret_cast<const ::pulse::cdm::bind::ScalarVolumeData*>(
      &::pulse::cdm::bind::_ScalarVolumeData_default_instance_);
}
inline const ::pulse::cdm::bind::ScalarVolumeData& InhalerData::spacervolume() const {
  // @@protoc_insertion_point(field_get:pulse.cdm.bind.InhalerData.SpacerVolume)
  return _internal_spacervolume();
}
inline ::pulse::cdm::bind::ScalarVolumeData* InhalerData::release_spacervolume() {
  // @@protoc_insertion_point(field_release:pulse.cdm.bind.InhalerData.SpacerVolume)
  
  ::pulse::cdm::bind::ScalarVolumeData* temp = spacervolume_;
  spacervolume_ = nullptr;
  return temp;
}
inline ::pulse::cdm::bind::ScalarVolumeData* InhalerData::_internal_mutable_spacervolume() {
  
  if (spacervolume_ == nullptr) {
    auto* p = CreateMaybeMessage<::pulse::cdm::bind::ScalarVolumeData>(GetArenaNoVirtual());
    spacervolume_ = p;
  }
  return spacervolume_;
}
inline ::pulse::cdm::bind::ScalarVolumeData* InhalerData::mutable_spacervolume() {
  // @@protoc_insertion_point(field_mutable:pulse.cdm.bind.InhalerData.SpacerVolume)
  return _internal_mutable_spacervolume();
}
inline void InhalerData::set_allocated_spacervolume(::pulse::cdm::bind::ScalarVolumeData* spacervolume) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(spacervolume_);
  }
  if (spacervolume) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      spacervolume = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, spacervolume, submessage_arena);
    }
    
  } else {
    
  }
  spacervolume_ = spacervolume;
  // @@protoc_insertion_point(field_set_allocated:pulse.cdm.bind.InhalerData.SpacerVolume)
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
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_pulse_2fcdm_2fbind_2fInhaler_2eproto
