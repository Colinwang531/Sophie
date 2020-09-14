// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Message.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Message_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Message_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3013000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3013000 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "Alarm.pb.h"
#include "Algorithm.pb.h"
#include "Component.pb.h"
#include "Crew.pb.h"
#include "Device.pb.h"
#include "Status.pb.h"
#include "User.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Message_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Message_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Message_2eproto;
namespace msg {
class MSG;
class MSGDefaultTypeInternal;
extern MSGDefaultTypeInternal _MSG_default_instance_;
}  // namespace msg
PROTOBUF_NAMESPACE_OPEN
template<> ::msg::MSG* Arena::CreateMaybeMessage<::msg::MSG>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace msg {

enum MSG_Type : int {
  MSG_Type_NONE = 0,
  MSG_Type_ALARM = 1,
  MSG_Type_ALGORITHM = 2,
  MSG_Type_COMPONENT = 3,
  MSG_Type_CREW = 4,
  MSG_Type_DEVICE = 5,
  MSG_Type_STATUS = 6,
  MSG_Type_USER = 7
};
bool MSG_Type_IsValid(int value);
constexpr MSG_Type MSG_Type_Type_MIN = MSG_Type_NONE;
constexpr MSG_Type MSG_Type_Type_MAX = MSG_Type_USER;
constexpr int MSG_Type_Type_ARRAYSIZE = MSG_Type_Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MSG_Type_descriptor();
template<typename T>
inline const std::string& MSG_Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MSG_Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MSG_Type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MSG_Type_descriptor(), enum_t_value);
}
inline bool MSG_Type_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MSG_Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MSG_Type>(
    MSG_Type_descriptor(), name, value);
}
// ===================================================================

class MSG PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.MSG) */ {
 public:
  inline MSG() : MSG(nullptr) {}
  virtual ~MSG();

  MSG(const MSG& from);
  MSG(MSG&& from) noexcept
    : MSG() {
    *this = ::std::move(from);
  }

  inline MSG& operator=(const MSG& from) {
    CopyFrom(from);
    return *this;
  }
  inline MSG& operator=(MSG&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
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
  static const MSG& default_instance();

  static inline const MSG* internal_default_instance() {
    return reinterpret_cast<const MSG*>(
               &_MSG_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(MSG& a, MSG& b) {
    a.Swap(&b);
  }
  inline void Swap(MSG* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(MSG* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline MSG* New() const final {
    return CreateMaybeMessage<MSG>(nullptr);
  }

  MSG* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<MSG>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const MSG& from);
  void MergeFrom(const MSG& from);
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
  void InternalSwap(MSG* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.MSG";
  }
  protected:
  explicit MSG(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Message_2eproto);
    return ::descriptor_table_Message_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef MSG_Type Type;
  static constexpr Type NONE =
    MSG_Type_NONE;
  static constexpr Type ALARM =
    MSG_Type_ALARM;
  static constexpr Type ALGORITHM =
    MSG_Type_ALGORITHM;
  static constexpr Type COMPONENT =
    MSG_Type_COMPONENT;
  static constexpr Type CREW =
    MSG_Type_CREW;
  static constexpr Type DEVICE =
    MSG_Type_DEVICE;
  static constexpr Type STATUS =
    MSG_Type_STATUS;
  static constexpr Type USER =
    MSG_Type_USER;
  static inline bool Type_IsValid(int value) {
    return MSG_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    MSG_Type_Type_MIN;
  static constexpr Type Type_MAX =
    MSG_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    MSG_Type_Type_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Type_descriptor() {
    return MSG_Type_descriptor();
  }
  template<typename T>
  static inline const std::string& Type_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Type>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Type_Name.");
    return MSG_Type_Name(enum_t_value);
  }
  static inline bool Type_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      Type* value) {
    return MSG_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kAlarmFieldNumber = 4,
    kAlgorithmFieldNumber = 5,
    kComponentFieldNumber = 6,
    kCrewFieldNumber = 7,
    kDeviceFieldNumber = 8,
    kStatusFieldNumber = 9,
    kUserFieldNumber = 10,
    kSequenceFieldNumber = 2,
    kTimestampFieldNumber = 3,
    kTypeFieldNumber = 1,
  };
  // optional .msg.Alarm alarm = 4;
  bool has_alarm() const;
  private:
  bool _internal_has_alarm() const;
  public:
  void clear_alarm();
  const ::msg::Alarm& alarm() const;
  ::msg::Alarm* release_alarm();
  ::msg::Alarm* mutable_alarm();
  void set_allocated_alarm(::msg::Alarm* alarm);
  private:
  const ::msg::Alarm& _internal_alarm() const;
  ::msg::Alarm* _internal_mutable_alarm();
  public:
  void unsafe_arena_set_allocated_alarm(
      ::msg::Alarm* alarm);
  ::msg::Alarm* unsafe_arena_release_alarm();

  // optional .msg.Algorithm algorithm = 5;
  bool has_algorithm() const;
  private:
  bool _internal_has_algorithm() const;
  public:
  void clear_algorithm();
  const ::msg::Algorithm& algorithm() const;
  ::msg::Algorithm* release_algorithm();
  ::msg::Algorithm* mutable_algorithm();
  void set_allocated_algorithm(::msg::Algorithm* algorithm);
  private:
  const ::msg::Algorithm& _internal_algorithm() const;
  ::msg::Algorithm* _internal_mutable_algorithm();
  public:
  void unsafe_arena_set_allocated_algorithm(
      ::msg::Algorithm* algorithm);
  ::msg::Algorithm* unsafe_arena_release_algorithm();

  // optional .msg.Component component = 6;
  bool has_component() const;
  private:
  bool _internal_has_component() const;
  public:
  void clear_component();
  const ::msg::Component& component() const;
  ::msg::Component* release_component();
  ::msg::Component* mutable_component();
  void set_allocated_component(::msg::Component* component);
  private:
  const ::msg::Component& _internal_component() const;
  ::msg::Component* _internal_mutable_component();
  public:
  void unsafe_arena_set_allocated_component(
      ::msg::Component* component);
  ::msg::Component* unsafe_arena_release_component();

  // optional .msg.Crew crew = 7;
  bool has_crew() const;
  private:
  bool _internal_has_crew() const;
  public:
  void clear_crew();
  const ::msg::Crew& crew() const;
  ::msg::Crew* release_crew();
  ::msg::Crew* mutable_crew();
  void set_allocated_crew(::msg::Crew* crew);
  private:
  const ::msg::Crew& _internal_crew() const;
  ::msg::Crew* _internal_mutable_crew();
  public:
  void unsafe_arena_set_allocated_crew(
      ::msg::Crew* crew);
  ::msg::Crew* unsafe_arena_release_crew();

  // optional .msg.Device device = 8;
  bool has_device() const;
  private:
  bool _internal_has_device() const;
  public:
  void clear_device();
  const ::msg::Device& device() const;
  ::msg::Device* release_device();
  ::msg::Device* mutable_device();
  void set_allocated_device(::msg::Device* device);
  private:
  const ::msg::Device& _internal_device() const;
  ::msg::Device* _internal_mutable_device();
  public:
  void unsafe_arena_set_allocated_device(
      ::msg::Device* device);
  ::msg::Device* unsafe_arena_release_device();

  // optional .msg.Status status = 9;
  bool has_status() const;
  private:
  bool _internal_has_status() const;
  public:
  void clear_status();
  const ::msg::Status& status() const;
  ::msg::Status* release_status();
  ::msg::Status* mutable_status();
  void set_allocated_status(::msg::Status* status);
  private:
  const ::msg::Status& _internal_status() const;
  ::msg::Status* _internal_mutable_status();
  public:
  void unsafe_arena_set_allocated_status(
      ::msg::Status* status);
  ::msg::Status* unsafe_arena_release_status();

  // optional .msg.User user = 10;
  bool has_user() const;
  private:
  bool _internal_has_user() const;
  public:
  void clear_user();
  const ::msg::User& user() const;
  ::msg::User* release_user();
  ::msg::User* mutable_user();
  void set_allocated_user(::msg::User* user);
  private:
  const ::msg::User& _internal_user() const;
  ::msg::User* _internal_mutable_user();
  public:
  void unsafe_arena_set_allocated_user(
      ::msg::User* user);
  ::msg::User* unsafe_arena_release_user();

  // required int64 sequence = 2;
  bool has_sequence() const;
  private:
  bool _internal_has_sequence() const;
  public:
  void clear_sequence();
  ::PROTOBUF_NAMESPACE_ID::int64 sequence() const;
  void set_sequence(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_sequence() const;
  void _internal_set_sequence(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // required int64 timestamp = 3;
  bool has_timestamp() const;
  private:
  bool _internal_has_timestamp() const;
  public:
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::int64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // required .msg.MSG.Type type = 1 [default = NONE];
  bool has_type() const;
  private:
  bool _internal_has_type() const;
  public:
  void clear_type();
  ::msg::MSG_Type type() const;
  void set_type(::msg::MSG_Type value);
  private:
  ::msg::MSG_Type _internal_type() const;
  void _internal_set_type(::msg::MSG_Type value);
  public:

  // @@protoc_insertion_point(class_scope:msg.MSG)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::msg::Alarm* alarm_;
  ::msg::Algorithm* algorithm_;
  ::msg::Component* component_;
  ::msg::Crew* crew_;
  ::msg::Device* device_;
  ::msg::Status* status_;
  ::msg::User* user_;
  ::PROTOBUF_NAMESPACE_ID::int64 sequence_;
  ::PROTOBUF_NAMESPACE_ID::int64 timestamp_;
  int type_;
  friend struct ::TableStruct_Message_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// MSG

// required .msg.MSG.Type type = 1 [default = NONE];
inline bool MSG::_internal_has_type() const {
  bool value = (_has_bits_[0] & 0x00000200u) != 0;
  return value;
}
inline bool MSG::has_type() const {
  return _internal_has_type();
}
inline void MSG::clear_type() {
  type_ = 0;
  _has_bits_[0] &= ~0x00000200u;
}
inline ::msg::MSG_Type MSG::_internal_type() const {
  return static_cast< ::msg::MSG_Type >(type_);
}
inline ::msg::MSG_Type MSG::type() const {
  // @@protoc_insertion_point(field_get:msg.MSG.type)
  return _internal_type();
}
inline void MSG::_internal_set_type(::msg::MSG_Type value) {
  assert(::msg::MSG_Type_IsValid(value));
  _has_bits_[0] |= 0x00000200u;
  type_ = value;
}
inline void MSG::set_type(::msg::MSG_Type value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:msg.MSG.type)
}

// required int64 sequence = 2;
inline bool MSG::_internal_has_sequence() const {
  bool value = (_has_bits_[0] & 0x00000080u) != 0;
  return value;
}
inline bool MSG::has_sequence() const {
  return _internal_has_sequence();
}
inline void MSG::clear_sequence() {
  sequence_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000080u;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 MSG::_internal_sequence() const {
  return sequence_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 MSG::sequence() const {
  // @@protoc_insertion_point(field_get:msg.MSG.sequence)
  return _internal_sequence();
}
inline void MSG::_internal_set_sequence(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _has_bits_[0] |= 0x00000080u;
  sequence_ = value;
}
inline void MSG::set_sequence(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_sequence(value);
  // @@protoc_insertion_point(field_set:msg.MSG.sequence)
}

// required int64 timestamp = 3;
inline bool MSG::_internal_has_timestamp() const {
  bool value = (_has_bits_[0] & 0x00000100u) != 0;
  return value;
}
inline bool MSG::has_timestamp() const {
  return _internal_has_timestamp();
}
inline void MSG::clear_timestamp() {
  timestamp_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000100u;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 MSG::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 MSG::timestamp() const {
  // @@protoc_insertion_point(field_get:msg.MSG.timestamp)
  return _internal_timestamp();
}
inline void MSG::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _has_bits_[0] |= 0x00000100u;
  timestamp_ = value;
}
inline void MSG::set_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:msg.MSG.timestamp)
}

// optional .msg.Alarm alarm = 4;
inline bool MSG::_internal_has_alarm() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  PROTOBUF_ASSUME(!value || alarm_ != nullptr);
  return value;
}
inline bool MSG::has_alarm() const {
  return _internal_has_alarm();
}
inline const ::msg::Alarm& MSG::_internal_alarm() const {
  const ::msg::Alarm* p = alarm_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Alarm&>(
      ::msg::_Alarm_default_instance_);
}
inline const ::msg::Alarm& MSG::alarm() const {
  // @@protoc_insertion_point(field_get:msg.MSG.alarm)
  return _internal_alarm();
}
inline void MSG::unsafe_arena_set_allocated_alarm(
    ::msg::Alarm* alarm) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(alarm_);
  }
  alarm_ = alarm;
  if (alarm) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.alarm)
}
inline ::msg::Alarm* MSG::release_alarm() {
  _has_bits_[0] &= ~0x00000001u;
  ::msg::Alarm* temp = alarm_;
  alarm_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Alarm* MSG::unsafe_arena_release_alarm() {
  // @@protoc_insertion_point(field_release:msg.MSG.alarm)
  _has_bits_[0] &= ~0x00000001u;
  ::msg::Alarm* temp = alarm_;
  alarm_ = nullptr;
  return temp;
}
inline ::msg::Alarm* MSG::_internal_mutable_alarm() {
  _has_bits_[0] |= 0x00000001u;
  if (alarm_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Alarm>(GetArena());
    alarm_ = p;
  }
  return alarm_;
}
inline ::msg::Alarm* MSG::mutable_alarm() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.alarm)
  return _internal_mutable_alarm();
}
inline void MSG::set_allocated_alarm(::msg::Alarm* alarm) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(alarm_);
  }
  if (alarm) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(alarm)->GetArena();
    if (message_arena != submessage_arena) {
      alarm = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, alarm, submessage_arena);
    }
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  alarm_ = alarm;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.alarm)
}

// optional .msg.Algorithm algorithm = 5;
inline bool MSG::_internal_has_algorithm() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  PROTOBUF_ASSUME(!value || algorithm_ != nullptr);
  return value;
}
inline bool MSG::has_algorithm() const {
  return _internal_has_algorithm();
}
inline const ::msg::Algorithm& MSG::_internal_algorithm() const {
  const ::msg::Algorithm* p = algorithm_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Algorithm&>(
      ::msg::_Algorithm_default_instance_);
}
inline const ::msg::Algorithm& MSG::algorithm() const {
  // @@protoc_insertion_point(field_get:msg.MSG.algorithm)
  return _internal_algorithm();
}
inline void MSG::unsafe_arena_set_allocated_algorithm(
    ::msg::Algorithm* algorithm) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(algorithm_);
  }
  algorithm_ = algorithm;
  if (algorithm) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.algorithm)
}
inline ::msg::Algorithm* MSG::release_algorithm() {
  _has_bits_[0] &= ~0x00000002u;
  ::msg::Algorithm* temp = algorithm_;
  algorithm_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Algorithm* MSG::unsafe_arena_release_algorithm() {
  // @@protoc_insertion_point(field_release:msg.MSG.algorithm)
  _has_bits_[0] &= ~0x00000002u;
  ::msg::Algorithm* temp = algorithm_;
  algorithm_ = nullptr;
  return temp;
}
inline ::msg::Algorithm* MSG::_internal_mutable_algorithm() {
  _has_bits_[0] |= 0x00000002u;
  if (algorithm_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Algorithm>(GetArena());
    algorithm_ = p;
  }
  return algorithm_;
}
inline ::msg::Algorithm* MSG::mutable_algorithm() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.algorithm)
  return _internal_mutable_algorithm();
}
inline void MSG::set_allocated_algorithm(::msg::Algorithm* algorithm) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(algorithm_);
  }
  if (algorithm) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(algorithm)->GetArena();
    if (message_arena != submessage_arena) {
      algorithm = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, algorithm, submessage_arena);
    }
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  algorithm_ = algorithm;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.algorithm)
}

// optional .msg.Component component = 6;
inline bool MSG::_internal_has_component() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  PROTOBUF_ASSUME(!value || component_ != nullptr);
  return value;
}
inline bool MSG::has_component() const {
  return _internal_has_component();
}
inline const ::msg::Component& MSG::_internal_component() const {
  const ::msg::Component* p = component_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Component&>(
      ::msg::_Component_default_instance_);
}
inline const ::msg::Component& MSG::component() const {
  // @@protoc_insertion_point(field_get:msg.MSG.component)
  return _internal_component();
}
inline void MSG::unsafe_arena_set_allocated_component(
    ::msg::Component* component) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(component_);
  }
  component_ = component;
  if (component) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.component)
}
inline ::msg::Component* MSG::release_component() {
  _has_bits_[0] &= ~0x00000004u;
  ::msg::Component* temp = component_;
  component_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Component* MSG::unsafe_arena_release_component() {
  // @@protoc_insertion_point(field_release:msg.MSG.component)
  _has_bits_[0] &= ~0x00000004u;
  ::msg::Component* temp = component_;
  component_ = nullptr;
  return temp;
}
inline ::msg::Component* MSG::_internal_mutable_component() {
  _has_bits_[0] |= 0x00000004u;
  if (component_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Component>(GetArena());
    component_ = p;
  }
  return component_;
}
inline ::msg::Component* MSG::mutable_component() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.component)
  return _internal_mutable_component();
}
inline void MSG::set_allocated_component(::msg::Component* component) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(component_);
  }
  if (component) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(component)->GetArena();
    if (message_arena != submessage_arena) {
      component = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, component, submessage_arena);
    }
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  component_ = component;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.component)
}

// optional .msg.Crew crew = 7;
inline bool MSG::_internal_has_crew() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  PROTOBUF_ASSUME(!value || crew_ != nullptr);
  return value;
}
inline bool MSG::has_crew() const {
  return _internal_has_crew();
}
inline const ::msg::Crew& MSG::_internal_crew() const {
  const ::msg::Crew* p = crew_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Crew&>(
      ::msg::_Crew_default_instance_);
}
inline const ::msg::Crew& MSG::crew() const {
  // @@protoc_insertion_point(field_get:msg.MSG.crew)
  return _internal_crew();
}
inline void MSG::unsafe_arena_set_allocated_crew(
    ::msg::Crew* crew) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(crew_);
  }
  crew_ = crew;
  if (crew) {
    _has_bits_[0] |= 0x00000008u;
  } else {
    _has_bits_[0] &= ~0x00000008u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.crew)
}
inline ::msg::Crew* MSG::release_crew() {
  _has_bits_[0] &= ~0x00000008u;
  ::msg::Crew* temp = crew_;
  crew_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Crew* MSG::unsafe_arena_release_crew() {
  // @@protoc_insertion_point(field_release:msg.MSG.crew)
  _has_bits_[0] &= ~0x00000008u;
  ::msg::Crew* temp = crew_;
  crew_ = nullptr;
  return temp;
}
inline ::msg::Crew* MSG::_internal_mutable_crew() {
  _has_bits_[0] |= 0x00000008u;
  if (crew_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Crew>(GetArena());
    crew_ = p;
  }
  return crew_;
}
inline ::msg::Crew* MSG::mutable_crew() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.crew)
  return _internal_mutable_crew();
}
inline void MSG::set_allocated_crew(::msg::Crew* crew) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(crew_);
  }
  if (crew) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(crew)->GetArena();
    if (message_arena != submessage_arena) {
      crew = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, crew, submessage_arena);
    }
    _has_bits_[0] |= 0x00000008u;
  } else {
    _has_bits_[0] &= ~0x00000008u;
  }
  crew_ = crew;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.crew)
}

// optional .msg.Device device = 8;
inline bool MSG::_internal_has_device() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  PROTOBUF_ASSUME(!value || device_ != nullptr);
  return value;
}
inline bool MSG::has_device() const {
  return _internal_has_device();
}
inline const ::msg::Device& MSG::_internal_device() const {
  const ::msg::Device* p = device_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Device&>(
      ::msg::_Device_default_instance_);
}
inline const ::msg::Device& MSG::device() const {
  // @@protoc_insertion_point(field_get:msg.MSG.device)
  return _internal_device();
}
inline void MSG::unsafe_arena_set_allocated_device(
    ::msg::Device* device) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(device_);
  }
  device_ = device;
  if (device) {
    _has_bits_[0] |= 0x00000010u;
  } else {
    _has_bits_[0] &= ~0x00000010u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.device)
}
inline ::msg::Device* MSG::release_device() {
  _has_bits_[0] &= ~0x00000010u;
  ::msg::Device* temp = device_;
  device_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Device* MSG::unsafe_arena_release_device() {
  // @@protoc_insertion_point(field_release:msg.MSG.device)
  _has_bits_[0] &= ~0x00000010u;
  ::msg::Device* temp = device_;
  device_ = nullptr;
  return temp;
}
inline ::msg::Device* MSG::_internal_mutable_device() {
  _has_bits_[0] |= 0x00000010u;
  if (device_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Device>(GetArena());
    device_ = p;
  }
  return device_;
}
inline ::msg::Device* MSG::mutable_device() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.device)
  return _internal_mutable_device();
}
inline void MSG::set_allocated_device(::msg::Device* device) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(device_);
  }
  if (device) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(device)->GetArena();
    if (message_arena != submessage_arena) {
      device = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, device, submessage_arena);
    }
    _has_bits_[0] |= 0x00000010u;
  } else {
    _has_bits_[0] &= ~0x00000010u;
  }
  device_ = device;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.device)
}

// optional .msg.Status status = 9;
inline bool MSG::_internal_has_status() const {
  bool value = (_has_bits_[0] & 0x00000020u) != 0;
  PROTOBUF_ASSUME(!value || status_ != nullptr);
  return value;
}
inline bool MSG::has_status() const {
  return _internal_has_status();
}
inline const ::msg::Status& MSG::_internal_status() const {
  const ::msg::Status* p = status_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Status&>(
      ::msg::_Status_default_instance_);
}
inline const ::msg::Status& MSG::status() const {
  // @@protoc_insertion_point(field_get:msg.MSG.status)
  return _internal_status();
}
inline void MSG::unsafe_arena_set_allocated_status(
    ::msg::Status* status) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(status_);
  }
  status_ = status;
  if (status) {
    _has_bits_[0] |= 0x00000020u;
  } else {
    _has_bits_[0] &= ~0x00000020u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.status)
}
inline ::msg::Status* MSG::release_status() {
  _has_bits_[0] &= ~0x00000020u;
  ::msg::Status* temp = status_;
  status_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Status* MSG::unsafe_arena_release_status() {
  // @@protoc_insertion_point(field_release:msg.MSG.status)
  _has_bits_[0] &= ~0x00000020u;
  ::msg::Status* temp = status_;
  status_ = nullptr;
  return temp;
}
inline ::msg::Status* MSG::_internal_mutable_status() {
  _has_bits_[0] |= 0x00000020u;
  if (status_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Status>(GetArena());
    status_ = p;
  }
  return status_;
}
inline ::msg::Status* MSG::mutable_status() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.status)
  return _internal_mutable_status();
}
inline void MSG::set_allocated_status(::msg::Status* status) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(status_);
  }
  if (status) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(status)->GetArena();
    if (message_arena != submessage_arena) {
      status = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, status, submessage_arena);
    }
    _has_bits_[0] |= 0x00000020u;
  } else {
    _has_bits_[0] &= ~0x00000020u;
  }
  status_ = status;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.status)
}

// optional .msg.User user = 10;
inline bool MSG::_internal_has_user() const {
  bool value = (_has_bits_[0] & 0x00000040u) != 0;
  PROTOBUF_ASSUME(!value || user_ != nullptr);
  return value;
}
inline bool MSG::has_user() const {
  return _internal_has_user();
}
inline const ::msg::User& MSG::_internal_user() const {
  const ::msg::User* p = user_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::User&>(
      ::msg::_User_default_instance_);
}
inline const ::msg::User& MSG::user() const {
  // @@protoc_insertion_point(field_get:msg.MSG.user)
  return _internal_user();
}
inline void MSG::unsafe_arena_set_allocated_user(
    ::msg::User* user) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(user_);
  }
  user_ = user;
  if (user) {
    _has_bits_[0] |= 0x00000040u;
  } else {
    _has_bits_[0] &= ~0x00000040u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.MSG.user)
}
inline ::msg::User* MSG::release_user() {
  _has_bits_[0] &= ~0x00000040u;
  ::msg::User* temp = user_;
  user_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::User* MSG::unsafe_arena_release_user() {
  // @@protoc_insertion_point(field_release:msg.MSG.user)
  _has_bits_[0] &= ~0x00000040u;
  ::msg::User* temp = user_;
  user_ = nullptr;
  return temp;
}
inline ::msg::User* MSG::_internal_mutable_user() {
  _has_bits_[0] |= 0x00000040u;
  if (user_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::User>(GetArena());
    user_ = p;
  }
  return user_;
}
inline ::msg::User* MSG::mutable_user() {
  // @@protoc_insertion_point(field_mutable:msg.MSG.user)
  return _internal_mutable_user();
}
inline void MSG::set_allocated_user(::msg::User* user) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(user_);
  }
  if (user) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(user)->GetArena();
    if (message_arena != submessage_arena) {
      user = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, user, submessage_arena);
    }
    _has_bits_[0] |= 0x00000040u;
  } else {
    _has_bits_[0] &= ~0x00000040u;
  }
  user_ = user;
  // @@protoc_insertion_point(field_set_allocated:msg.MSG.user)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::msg::MSG_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::msg::MSG_Type>() {
  return ::msg::MSG_Type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Message_2eproto