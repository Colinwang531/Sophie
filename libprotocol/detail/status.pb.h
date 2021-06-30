// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: status.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_status_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_status_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3015000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3015006 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_status_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_status_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_status_2eproto;
::PROTOBUF_NAMESPACE_ID::Metadata descriptor_table_status_2eproto_metadata_getter(int index);
namespace msg {
class Controller;
struct ControllerDefaultTypeInternal;
extern ControllerDefaultTypeInternal _Controller_default_instance_;
class State;
struct StateDefaultTypeInternal;
extern StateDefaultTypeInternal _State_default_instance_;
class StatusMessage;
struct StatusMessageDefaultTypeInternal;
extern StatusMessageDefaultTypeInternal _StatusMessage_default_instance_;
}  // namespace msg
PROTOBUF_NAMESPACE_OPEN
template<> ::msg::Controller* Arena::CreateMaybeMessage<::msg::Controller>(Arena*);
template<> ::msg::State* Arena::CreateMaybeMessage<::msg::State>(Arena*);
template<> ::msg::StatusMessage* Arena::CreateMaybeMessage<::msg::StatusMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace msg {

enum StatusMessage_Command : int {
  StatusMessage_Command_SET_REQ = 1,
  StatusMessage_Command_SET_REP = 2,
  StatusMessage_Command_QUERY_REQ = 3,
  StatusMessage_Command_QUERY_REP = 4
};
bool StatusMessage_Command_IsValid(int value);
constexpr StatusMessage_Command StatusMessage_Command_Command_MIN = StatusMessage_Command_SET_REQ;
constexpr StatusMessage_Command StatusMessage_Command_Command_MAX = StatusMessage_Command_QUERY_REP;
constexpr int StatusMessage_Command_Command_ARRAYSIZE = StatusMessage_Command_Command_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* StatusMessage_Command_descriptor();
template<typename T>
inline const std::string& StatusMessage_Command_Name(T enum_t_value) {
  static_assert(::std::is_same<T, StatusMessage_Command>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function StatusMessage_Command_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    StatusMessage_Command_descriptor(), enum_t_value);
}
inline bool StatusMessage_Command_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, StatusMessage_Command* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<StatusMessage_Command>(
    StatusMessage_Command_descriptor(), name, value);
}
// ===================================================================

class State PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.State) */ {
 public:
  inline State() : State(nullptr) {}
  virtual ~State();
  explicit constexpr State(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  State(const State& from);
  State(State&& from) noexcept
    : State() {
    *this = ::std::move(from);
  }

  inline State& operator=(const State& from) {
    CopyFrom(from);
    return *this;
  }
  inline State& operator=(State&& from) noexcept {
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
  static const State& default_instance() {
    return *internal_default_instance();
  }
  static inline const State* internal_default_instance() {
    return reinterpret_cast<const State*>(
               &_State_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(State& a, State& b) {
    a.Swap(&b);
  }
  inline void Swap(State* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(State* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline State* New() const final {
    return CreateMaybeMessage<State>(nullptr);
  }

  State* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<State>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const State& from);
  void MergeFrom(const State& from);
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
  void InternalSwap(State* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.State";
  }
  protected:
  explicit State(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    return ::descriptor_table_status_2eproto_metadata_getter(kIndexInFileMessages);
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStateFieldNumber = 1,
  };
  // required int32 state = 1;
  bool has_state() const;
  private:
  bool _internal_has_state() const;
  public:
  void clear_state();
  ::PROTOBUF_NAMESPACE_ID::int32 state() const;
  void set_state(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_state() const;
  void _internal_set_state(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:msg.State)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int32 state_;
  friend struct ::TableStruct_status_2eproto;
};
// -------------------------------------------------------------------

class Controller PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.Controller) */ {
 public:
  inline Controller() : Controller(nullptr) {}
  virtual ~Controller();
  explicit constexpr Controller(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Controller(const Controller& from);
  Controller(Controller&& from) noexcept
    : Controller() {
    *this = ::std::move(from);
  }

  inline Controller& operator=(const Controller& from) {
    CopyFrom(from);
    return *this;
  }
  inline Controller& operator=(Controller&& from) noexcept {
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
  static const Controller& default_instance() {
    return *internal_default_instance();
  }
  static inline const Controller* internal_default_instance() {
    return reinterpret_cast<const Controller*>(
               &_Controller_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Controller& a, Controller& b) {
    a.Swap(&b);
  }
  inline void Swap(Controller* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Controller* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Controller* New() const final {
    return CreateMaybeMessage<Controller>(nullptr);
  }

  Controller* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Controller>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Controller& from);
  void MergeFrom(const Controller& from);
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
  void InternalSwap(Controller* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.Controller";
  }
  protected:
  explicit Controller(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    return ::descriptor_table_status_2eproto_metadata_getter(kIndexInFileMessages);
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kOperationFieldNumber = 1,
    kStateFieldNumber = 2,
  };
  // required bool operation = 1;
  bool has_operation() const;
  private:
  bool _internal_has_operation() const;
  public:
  void clear_operation();
  bool operation() const;
  void set_operation(bool value);
  private:
  bool _internal_operation() const;
  void _internal_set_operation(bool value);
  public:

  // optional bool state = 2;
  bool has_state() const;
  private:
  bool _internal_has_state() const;
  public:
  void clear_state();
  bool state() const;
  void set_state(bool value);
  private:
  bool _internal_state() const;
  void _internal_set_state(bool value);
  public:

  // @@protoc_insertion_point(class_scope:msg.Controller)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  bool operation_;
  bool state_;
  friend struct ::TableStruct_status_2eproto;
};
// -------------------------------------------------------------------

class StatusMessage PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.StatusMessage) */ {
 public:
  inline StatusMessage() : StatusMessage(nullptr) {}
  virtual ~StatusMessage();
  explicit constexpr StatusMessage(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  StatusMessage(const StatusMessage& from);
  StatusMessage(StatusMessage&& from) noexcept
    : StatusMessage() {
    *this = ::std::move(from);
  }

  inline StatusMessage& operator=(const StatusMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline StatusMessage& operator=(StatusMessage&& from) noexcept {
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
  static const StatusMessage& default_instance() {
    return *internal_default_instance();
  }
  static inline const StatusMessage* internal_default_instance() {
    return reinterpret_cast<const StatusMessage*>(
               &_StatusMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(StatusMessage& a, StatusMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(StatusMessage* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(StatusMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline StatusMessage* New() const final {
    return CreateMaybeMessage<StatusMessage>(nullptr);
  }

  StatusMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<StatusMessage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const StatusMessage& from);
  void MergeFrom(const StatusMessage& from);
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
  void InternalSwap(StatusMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.StatusMessage";
  }
  protected:
  explicit StatusMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    return ::descriptor_table_status_2eproto_metadata_getter(kIndexInFileMessages);
  }

  public:

  // nested types ----------------------------------------------------

  typedef StatusMessage_Command Command;
  static constexpr Command SET_REQ =
    StatusMessage_Command_SET_REQ;
  static constexpr Command SET_REP =
    StatusMessage_Command_SET_REP;
  static constexpr Command QUERY_REQ =
    StatusMessage_Command_QUERY_REQ;
  static constexpr Command QUERY_REP =
    StatusMessage_Command_QUERY_REP;
  static inline bool Command_IsValid(int value) {
    return StatusMessage_Command_IsValid(value);
  }
  static constexpr Command Command_MIN =
    StatusMessage_Command_Command_MIN;
  static constexpr Command Command_MAX =
    StatusMessage_Command_Command_MAX;
  static constexpr int Command_ARRAYSIZE =
    StatusMessage_Command_Command_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Command_descriptor() {
    return StatusMessage_Command_descriptor();
  }
  template<typename T>
  static inline const std::string& Command_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Command>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Command_Name.");
    return StatusMessage_Command_Name(enum_t_value);
  }
  static inline bool Command_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      Command* value) {
    return StatusMessage_Command_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kPidFieldNumber = 5,
    kStateFieldNumber = 2,
    kControllerFieldNumber = 3,
    kStatusFieldNumber = 4,
    kCommandFieldNumber = 1,
  };
  // optional bytes pid = 5;
  bool has_pid() const;
  private:
  bool _internal_has_pid() const;
  public:
  void clear_pid();
  const std::string& pid() const;
  void set_pid(const std::string& value);
  void set_pid(std::string&& value);
  void set_pid(const char* value);
  void set_pid(const void* value, size_t size);
  std::string* mutable_pid();
  std::string* release_pid();
  void set_allocated_pid(std::string* pid);
  private:
  const std::string& _internal_pid() const;
  void _internal_set_pid(const std::string& value);
  std::string* _internal_mutable_pid();
  public:

  // optional .msg.State state = 2;
  bool has_state() const;
  private:
  bool _internal_has_state() const;
  public:
  void clear_state();
  const ::msg::State& state() const;
  ::msg::State* release_state();
  ::msg::State* mutable_state();
  void set_allocated_state(::msg::State* state);
  private:
  const ::msg::State& _internal_state() const;
  ::msg::State* _internal_mutable_state();
  public:
  void unsafe_arena_set_allocated_state(
      ::msg::State* state);
  ::msg::State* unsafe_arena_release_state();

  // optional .msg.Controller controller = 3;
  bool has_controller() const;
  private:
  bool _internal_has_controller() const;
  public:
  void clear_controller();
  const ::msg::Controller& controller() const;
  ::msg::Controller* release_controller();
  ::msg::Controller* mutable_controller();
  void set_allocated_controller(::msg::Controller* controller);
  private:
  const ::msg::Controller& _internal_controller() const;
  ::msg::Controller* _internal_mutable_controller();
  public:
  void unsafe_arena_set_allocated_controller(
      ::msg::Controller* controller);
  ::msg::Controller* unsafe_arena_release_controller();

  // optional int32 status = 4;
  bool has_status() const;
  private:
  bool _internal_has_status() const;
  public:
  void clear_status();
  ::PROTOBUF_NAMESPACE_ID::int32 status() const;
  void set_status(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_status() const;
  void _internal_set_status(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required .msg.StatusMessage.Command command = 1;
  bool has_command() const;
  private:
  bool _internal_has_command() const;
  public:
  void clear_command();
  ::msg::StatusMessage_Command command() const;
  void set_command(::msg::StatusMessage_Command value);
  private:
  ::msg::StatusMessage_Command _internal_command() const;
  void _internal_set_command(::msg::StatusMessage_Command value);
  public:

  // @@protoc_insertion_point(class_scope:msg.StatusMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr pid_;
  ::msg::State* state_;
  ::msg::Controller* controller_;
  ::PROTOBUF_NAMESPACE_ID::int32 status_;
  int command_;
  friend struct ::TableStruct_status_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// State

// required int32 state = 1;
inline bool State::_internal_has_state() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool State::has_state() const {
  return _internal_has_state();
}
inline void State::clear_state() {
  state_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 State::_internal_state() const {
  return state_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 State::state() const {
  // @@protoc_insertion_point(field_get:msg.State.state)
  return _internal_state();
}
inline void State::_internal_set_state(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000001u;
  state_ = value;
}
inline void State::set_state(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:msg.State.state)
}

// -------------------------------------------------------------------

// Controller

// required bool operation = 1;
inline bool Controller::_internal_has_operation() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool Controller::has_operation() const {
  return _internal_has_operation();
}
inline void Controller::clear_operation() {
  operation_ = false;
  _has_bits_[0] &= ~0x00000001u;
}
inline bool Controller::_internal_operation() const {
  return operation_;
}
inline bool Controller::operation() const {
  // @@protoc_insertion_point(field_get:msg.Controller.operation)
  return _internal_operation();
}
inline void Controller::_internal_set_operation(bool value) {
  _has_bits_[0] |= 0x00000001u;
  operation_ = value;
}
inline void Controller::set_operation(bool value) {
  _internal_set_operation(value);
  // @@protoc_insertion_point(field_set:msg.Controller.operation)
}

// optional bool state = 2;
inline bool Controller::_internal_has_state() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool Controller::has_state() const {
  return _internal_has_state();
}
inline void Controller::clear_state() {
  state_ = false;
  _has_bits_[0] &= ~0x00000002u;
}
inline bool Controller::_internal_state() const {
  return state_;
}
inline bool Controller::state() const {
  // @@protoc_insertion_point(field_get:msg.Controller.state)
  return _internal_state();
}
inline void Controller::_internal_set_state(bool value) {
  _has_bits_[0] |= 0x00000002u;
  state_ = value;
}
inline void Controller::set_state(bool value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:msg.Controller.state)
}

// -------------------------------------------------------------------

// StatusMessage

// required .msg.StatusMessage.Command command = 1;
inline bool StatusMessage::_internal_has_command() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  return value;
}
inline bool StatusMessage::has_command() const {
  return _internal_has_command();
}
inline void StatusMessage::clear_command() {
  command_ = 1;
  _has_bits_[0] &= ~0x00000010u;
}
inline ::msg::StatusMessage_Command StatusMessage::_internal_command() const {
  return static_cast< ::msg::StatusMessage_Command >(command_);
}
inline ::msg::StatusMessage_Command StatusMessage::command() const {
  // @@protoc_insertion_point(field_get:msg.StatusMessage.command)
  return _internal_command();
}
inline void StatusMessage::_internal_set_command(::msg::StatusMessage_Command value) {
  assert(::msg::StatusMessage_Command_IsValid(value));
  _has_bits_[0] |= 0x00000010u;
  command_ = value;
}
inline void StatusMessage::set_command(::msg::StatusMessage_Command value) {
  _internal_set_command(value);
  // @@protoc_insertion_point(field_set:msg.StatusMessage.command)
}

// optional .msg.State state = 2;
inline bool StatusMessage::_internal_has_state() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  PROTOBUF_ASSUME(!value || state_ != nullptr);
  return value;
}
inline bool StatusMessage::has_state() const {
  return _internal_has_state();
}
inline void StatusMessage::clear_state() {
  if (state_ != nullptr) state_->Clear();
  _has_bits_[0] &= ~0x00000002u;
}
inline const ::msg::State& StatusMessage::_internal_state() const {
  const ::msg::State* p = state_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::State&>(
      ::msg::_State_default_instance_);
}
inline const ::msg::State& StatusMessage::state() const {
  // @@protoc_insertion_point(field_get:msg.StatusMessage.state)
  return _internal_state();
}
inline void StatusMessage::unsafe_arena_set_allocated_state(
    ::msg::State* state) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(state_);
  }
  state_ = state;
  if (state) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.StatusMessage.state)
}
inline ::msg::State* StatusMessage::release_state() {
  _has_bits_[0] &= ~0x00000002u;
  ::msg::State* temp = state_;
  state_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::State* StatusMessage::unsafe_arena_release_state() {
  // @@protoc_insertion_point(field_release:msg.StatusMessage.state)
  _has_bits_[0] &= ~0x00000002u;
  ::msg::State* temp = state_;
  state_ = nullptr;
  return temp;
}
inline ::msg::State* StatusMessage::_internal_mutable_state() {
  _has_bits_[0] |= 0x00000002u;
  if (state_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::State>(GetArena());
    state_ = p;
  }
  return state_;
}
inline ::msg::State* StatusMessage::mutable_state() {
  // @@protoc_insertion_point(field_mutable:msg.StatusMessage.state)
  return _internal_mutable_state();
}
inline void StatusMessage::set_allocated_state(::msg::State* state) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete state_;
  }
  if (state) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(state);
    if (message_arena != submessage_arena) {
      state = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, state, submessage_arena);
    }
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  state_ = state;
  // @@protoc_insertion_point(field_set_allocated:msg.StatusMessage.state)
}

// optional .msg.Controller controller = 3;
inline bool StatusMessage::_internal_has_controller() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  PROTOBUF_ASSUME(!value || controller_ != nullptr);
  return value;
}
inline bool StatusMessage::has_controller() const {
  return _internal_has_controller();
}
inline void StatusMessage::clear_controller() {
  if (controller_ != nullptr) controller_->Clear();
  _has_bits_[0] &= ~0x00000004u;
}
inline const ::msg::Controller& StatusMessage::_internal_controller() const {
  const ::msg::Controller* p = controller_;
  return p != nullptr ? *p : reinterpret_cast<const ::msg::Controller&>(
      ::msg::_Controller_default_instance_);
}
inline const ::msg::Controller& StatusMessage::controller() const {
  // @@protoc_insertion_point(field_get:msg.StatusMessage.controller)
  return _internal_controller();
}
inline void StatusMessage::unsafe_arena_set_allocated_controller(
    ::msg::Controller* controller) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(controller_);
  }
  controller_ = controller;
  if (controller) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.StatusMessage.controller)
}
inline ::msg::Controller* StatusMessage::release_controller() {
  _has_bits_[0] &= ~0x00000004u;
  ::msg::Controller* temp = controller_;
  controller_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::msg::Controller* StatusMessage::unsafe_arena_release_controller() {
  // @@protoc_insertion_point(field_release:msg.StatusMessage.controller)
  _has_bits_[0] &= ~0x00000004u;
  ::msg::Controller* temp = controller_;
  controller_ = nullptr;
  return temp;
}
inline ::msg::Controller* StatusMessage::_internal_mutable_controller() {
  _has_bits_[0] |= 0x00000004u;
  if (controller_ == nullptr) {
    auto* p = CreateMaybeMessage<::msg::Controller>(GetArena());
    controller_ = p;
  }
  return controller_;
}
inline ::msg::Controller* StatusMessage::mutable_controller() {
  // @@protoc_insertion_point(field_mutable:msg.StatusMessage.controller)
  return _internal_mutable_controller();
}
inline void StatusMessage::set_allocated_controller(::msg::Controller* controller) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete controller_;
  }
  if (controller) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(controller);
    if (message_arena != submessage_arena) {
      controller = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, controller, submessage_arena);
    }
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  controller_ = controller;
  // @@protoc_insertion_point(field_set_allocated:msg.StatusMessage.controller)
}

// optional int32 status = 4;
inline bool StatusMessage::_internal_has_status() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool StatusMessage::has_status() const {
  return _internal_has_status();
}
inline void StatusMessage::clear_status() {
  status_ = 0;
  _has_bits_[0] &= ~0x00000008u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 StatusMessage::_internal_status() const {
  return status_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 StatusMessage::status() const {
  // @@protoc_insertion_point(field_get:msg.StatusMessage.status)
  return _internal_status();
}
inline void StatusMessage::_internal_set_status(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000008u;
  status_ = value;
}
inline void StatusMessage::set_status(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:msg.StatusMessage.status)
}

// optional bytes pid = 5;
inline bool StatusMessage::_internal_has_pid() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool StatusMessage::has_pid() const {
  return _internal_has_pid();
}
inline void StatusMessage::clear_pid() {
  pid_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& StatusMessage::pid() const {
  // @@protoc_insertion_point(field_get:msg.StatusMessage.pid)
  return _internal_pid();
}
inline void StatusMessage::set_pid(const std::string& value) {
  _internal_set_pid(value);
  // @@protoc_insertion_point(field_set:msg.StatusMessage.pid)
}
inline std::string* StatusMessage::mutable_pid() {
  // @@protoc_insertion_point(field_mutable:msg.StatusMessage.pid)
  return _internal_mutable_pid();
}
inline const std::string& StatusMessage::_internal_pid() const {
  return pid_.Get();
}
inline void StatusMessage::_internal_set_pid(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  pid_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void StatusMessage::set_pid(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  pid_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.StatusMessage.pid)
}
inline void StatusMessage::set_pid(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  pid_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:msg.StatusMessage.pid)
}
inline void StatusMessage::set_pid(const void* value,
    size_t size) {
  _has_bits_[0] |= 0x00000001u;
  pid_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.StatusMessage.pid)
}
inline std::string* StatusMessage::_internal_mutable_pid() {
  _has_bits_[0] |= 0x00000001u;
  return pid_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* StatusMessage::release_pid() {
  // @@protoc_insertion_point(field_release:msg.StatusMessage.pid)
  if (!_internal_has_pid()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return pid_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void StatusMessage::set_allocated_pid(std::string* pid) {
  if (pid != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  pid_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), pid,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.StatusMessage.pid)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::msg::StatusMessage_Command> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::msg::StatusMessage_Command>() {
  return ::msg::StatusMessage_Command_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_status_2eproto
