// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: clock.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_clock_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_clock_2eproto

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_clock_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_clock_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_clock_2eproto;
::PROTOBUF_NAMESPACE_ID::Metadata descriptor_table_clock_2eproto_metadata_getter(int index);
namespace msg {
class ClockMessage;
struct ClockMessageDefaultTypeInternal;
extern ClockMessageDefaultTypeInternal _ClockMessage_default_instance_;
}  // namespace msg
PROTOBUF_NAMESPACE_OPEN
template<> ::msg::ClockMessage* Arena::CreateMaybeMessage<::msg::ClockMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace msg {

// ===================================================================

class ClockMessage PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.ClockMessage) */ {
 public:
  inline ClockMessage() : ClockMessage(nullptr) {}
  virtual ~ClockMessage();
  explicit constexpr ClockMessage(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ClockMessage(const ClockMessage& from);
  ClockMessage(ClockMessage&& from) noexcept
    : ClockMessage() {
    *this = ::std::move(from);
  }

  inline ClockMessage& operator=(const ClockMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline ClockMessage& operator=(ClockMessage&& from) noexcept {
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
  static const ClockMessage& default_instance() {
    return *internal_default_instance();
  }
  static inline const ClockMessage* internal_default_instance() {
    return reinterpret_cast<const ClockMessage*>(
               &_ClockMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ClockMessage& a, ClockMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(ClockMessage* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ClockMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ClockMessage* New() const final {
    return CreateMaybeMessage<ClockMessage>(nullptr);
  }

  ClockMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ClockMessage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ClockMessage& from);
  void MergeFrom(const ClockMessage& from);
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
  void InternalSwap(ClockMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.ClockMessage";
  }
  protected:
  explicit ClockMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    return ::descriptor_table_clock_2eproto_metadata_getter(kIndexInFileMessages);
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kClockFieldNumber = 1,
  };
  // required bytes clock = 1;
  bool has_clock() const;
  private:
  bool _internal_has_clock() const;
  public:
  void clear_clock();
  const std::string& clock() const;
  void set_clock(const std::string& value);
  void set_clock(std::string&& value);
  void set_clock(const char* value);
  void set_clock(const void* value, size_t size);
  std::string* mutable_clock();
  std::string* release_clock();
  void set_allocated_clock(std::string* clock);
  private:
  const std::string& _internal_clock() const;
  void _internal_set_clock(const std::string& value);
  std::string* _internal_mutable_clock();
  public:

  // @@protoc_insertion_point(class_scope:msg.ClockMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr clock_;
  friend struct ::TableStruct_clock_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ClockMessage

// required bytes clock = 1;
inline bool ClockMessage::_internal_has_clock() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool ClockMessage::has_clock() const {
  return _internal_has_clock();
}
inline void ClockMessage::clear_clock() {
  clock_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& ClockMessage::clock() const {
  // @@protoc_insertion_point(field_get:msg.ClockMessage.clock)
  return _internal_clock();
}
inline void ClockMessage::set_clock(const std::string& value) {
  _internal_set_clock(value);
  // @@protoc_insertion_point(field_set:msg.ClockMessage.clock)
}
inline std::string* ClockMessage::mutable_clock() {
  // @@protoc_insertion_point(field_mutable:msg.ClockMessage.clock)
  return _internal_mutable_clock();
}
inline const std::string& ClockMessage::_internal_clock() const {
  return clock_.Get();
}
inline void ClockMessage::_internal_set_clock(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  clock_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void ClockMessage::set_clock(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  clock_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.ClockMessage.clock)
}
inline void ClockMessage::set_clock(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  clock_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:msg.ClockMessage.clock)
}
inline void ClockMessage::set_clock(const void* value,
    size_t size) {
  _has_bits_[0] |= 0x00000001u;
  clock_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.ClockMessage.clock)
}
inline std::string* ClockMessage::_internal_mutable_clock() {
  _has_bits_[0] |= 0x00000001u;
  return clock_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* ClockMessage::release_clock() {
  // @@protoc_insertion_point(field_release:msg.ClockMessage.clock)
  if (!_internal_has_clock()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return clock_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void ClockMessage::set_allocated_clock(std::string* clock) {
  if (clock != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  clock_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), clock,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.ClockMessage.clock)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_clock_2eproto
