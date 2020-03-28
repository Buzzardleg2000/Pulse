// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pulse/engine/bind/Pulse.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_pulse_2fengine_2fbind_2fPulse_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_pulse_2fengine_2fbind_2fPulse_2eproto

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
#include "pulse/cdm/bind/Scenario.pb.h"
#include "pulse/engine/bind/PulseConfiguration.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_pulse_2fengine_2fbind_2fPulse_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_pulse_2fengine_2fbind_2fPulse_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_pulse_2fengine_2fbind_2fPulse_2eproto;
namespace pulse {
namespace engine {
namespace bind {
class ScenarioData;
class ScenarioDataDefaultTypeInternal;
extern ScenarioDataDefaultTypeInternal _ScenarioData_default_instance_;
}  // namespace bind
}  // namespace engine
}  // namespace pulse
PROTOBUF_NAMESPACE_OPEN
template<> ::pulse::engine::bind::ScenarioData* Arena::CreateMaybeMessage<::pulse::engine::bind::ScenarioData>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace pulse {
namespace engine {
namespace bind {

// ===================================================================

class ScenarioData :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:pulse.engine.bind.ScenarioData) */ {
 public:
  ScenarioData();
  virtual ~ScenarioData();

  ScenarioData(const ScenarioData& from);
  ScenarioData(ScenarioData&& from) noexcept
    : ScenarioData() {
    *this = ::std::move(from);
  }

  inline ScenarioData& operator=(const ScenarioData& from) {
    CopyFrom(from);
    return *this;
  }
  inline ScenarioData& operator=(ScenarioData&& from) noexcept {
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
  static const ScenarioData& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ScenarioData* internal_default_instance() {
    return reinterpret_cast<const ScenarioData*>(
               &_ScenarioData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ScenarioData& a, ScenarioData& b) {
    a.Swap(&b);
  }
  inline void Swap(ScenarioData* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ScenarioData* New() const final {
    return CreateMaybeMessage<ScenarioData>(nullptr);
  }

  ScenarioData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ScenarioData>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ScenarioData& from);
  void MergeFrom(const ScenarioData& from);
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
  void InternalSwap(ScenarioData* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "pulse.engine.bind.ScenarioData";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_pulse_2fengine_2fbind_2fPulse_2eproto);
    return ::descriptor_table_pulse_2fengine_2fbind_2fPulse_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kScenarioFieldNumber = 1,
    kConfigurationFieldNumber = 2,
  };
  // .pulse.cdm.bind.ScenarioData Scenario = 1;
  bool has_scenario() const;
  private:
  bool _internal_has_scenario() const;
  public:
  void clear_scenario();
  const ::pulse::cdm::bind::ScenarioData& scenario() const;
  ::pulse::cdm::bind::ScenarioData* release_scenario();
  ::pulse::cdm::bind::ScenarioData* mutable_scenario();
  void set_allocated_scenario(::pulse::cdm::bind::ScenarioData* scenario);
  private:
  const ::pulse::cdm::bind::ScenarioData& _internal_scenario() const;
  ::pulse::cdm::bind::ScenarioData* _internal_mutable_scenario();
  public:

  // .pulse.engine.bind.ConfigurationData Configuration = 2;
  bool has_configuration() const;
  private:
  bool _internal_has_configuration() const;
  public:
  void clear_configuration();
  const ::pulse::engine::bind::ConfigurationData& configuration() const;
  ::pulse::engine::bind::ConfigurationData* release_configuration();
  ::pulse::engine::bind::ConfigurationData* mutable_configuration();
  void set_allocated_configuration(::pulse::engine::bind::ConfigurationData* configuration);
  private:
  const ::pulse::engine::bind::ConfigurationData& _internal_configuration() const;
  ::pulse::engine::bind::ConfigurationData* _internal_mutable_configuration();
  public:

  // @@protoc_insertion_point(class_scope:pulse.engine.bind.ScenarioData)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::pulse::cdm::bind::ScenarioData* scenario_;
  ::pulse::engine::bind::ConfigurationData* configuration_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_pulse_2fengine_2fbind_2fPulse_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ScenarioData

// .pulse.cdm.bind.ScenarioData Scenario = 1;
inline bool ScenarioData::_internal_has_scenario() const {
  return this != internal_default_instance() && scenario_ != nullptr;
}
inline bool ScenarioData::has_scenario() const {
  return _internal_has_scenario();
}
inline const ::pulse::cdm::bind::ScenarioData& ScenarioData::_internal_scenario() const {
  const ::pulse::cdm::bind::ScenarioData* p = scenario_;
  return p != nullptr ? *p : *reinterpret_cast<const ::pulse::cdm::bind::ScenarioData*>(
      &::pulse::cdm::bind::_ScenarioData_default_instance_);
}
inline const ::pulse::cdm::bind::ScenarioData& ScenarioData::scenario() const {
  // @@protoc_insertion_point(field_get:pulse.engine.bind.ScenarioData.Scenario)
  return _internal_scenario();
}
inline ::pulse::cdm::bind::ScenarioData* ScenarioData::release_scenario() {
  // @@protoc_insertion_point(field_release:pulse.engine.bind.ScenarioData.Scenario)
  
  ::pulse::cdm::bind::ScenarioData* temp = scenario_;
  scenario_ = nullptr;
  return temp;
}
inline ::pulse::cdm::bind::ScenarioData* ScenarioData::_internal_mutable_scenario() {
  
  if (scenario_ == nullptr) {
    auto* p = CreateMaybeMessage<::pulse::cdm::bind::ScenarioData>(GetArenaNoVirtual());
    scenario_ = p;
  }
  return scenario_;
}
inline ::pulse::cdm::bind::ScenarioData* ScenarioData::mutable_scenario() {
  // @@protoc_insertion_point(field_mutable:pulse.engine.bind.ScenarioData.Scenario)
  return _internal_mutable_scenario();
}
inline void ScenarioData::set_allocated_scenario(::pulse::cdm::bind::ScenarioData* scenario) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(scenario_);
  }
  if (scenario) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      scenario = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, scenario, submessage_arena);
    }
    
  } else {
    
  }
  scenario_ = scenario;
  // @@protoc_insertion_point(field_set_allocated:pulse.engine.bind.ScenarioData.Scenario)
}

// .pulse.engine.bind.ConfigurationData Configuration = 2;
inline bool ScenarioData::_internal_has_configuration() const {
  return this != internal_default_instance() && configuration_ != nullptr;
}
inline bool ScenarioData::has_configuration() const {
  return _internal_has_configuration();
}
inline const ::pulse::engine::bind::ConfigurationData& ScenarioData::_internal_configuration() const {
  const ::pulse::engine::bind::ConfigurationData* p = configuration_;
  return p != nullptr ? *p : *reinterpret_cast<const ::pulse::engine::bind::ConfigurationData*>(
      &::pulse::engine::bind::_ConfigurationData_default_instance_);
}
inline const ::pulse::engine::bind::ConfigurationData& ScenarioData::configuration() const {
  // @@protoc_insertion_point(field_get:pulse.engine.bind.ScenarioData.Configuration)
  return _internal_configuration();
}
inline ::pulse::engine::bind::ConfigurationData* ScenarioData::release_configuration() {
  // @@protoc_insertion_point(field_release:pulse.engine.bind.ScenarioData.Configuration)
  
  ::pulse::engine::bind::ConfigurationData* temp = configuration_;
  configuration_ = nullptr;
  return temp;
}
inline ::pulse::engine::bind::ConfigurationData* ScenarioData::_internal_mutable_configuration() {
  
  if (configuration_ == nullptr) {
    auto* p = CreateMaybeMessage<::pulse::engine::bind::ConfigurationData>(GetArenaNoVirtual());
    configuration_ = p;
  }
  return configuration_;
}
inline ::pulse::engine::bind::ConfigurationData* ScenarioData::mutable_configuration() {
  // @@protoc_insertion_point(field_mutable:pulse.engine.bind.ScenarioData.Configuration)
  return _internal_mutable_configuration();
}
inline void ScenarioData::set_allocated_configuration(::pulse::engine::bind::ConfigurationData* configuration) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(configuration_);
  }
  if (configuration) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      configuration = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, configuration, submessage_arena);
    }
    
  } else {
    
  }
  configuration_ = configuration;
  // @@protoc_insertion_point(field_set_allocated:pulse.engine.bind.ScenarioData.Configuration)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace bind
}  // namespace engine
}  // namespace pulse

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_pulse_2fengine_2fbind_2fPulse_2eproto
