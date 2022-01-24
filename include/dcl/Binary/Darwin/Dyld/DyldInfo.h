//===--- DyldInfo.h - Dyld Info Link Edit Data ------------------*- C++ -*-===//
//
// This source file is part of the DCL open source project
//
// Copyright (c) 2022 Li Yu-Long and the DCL project authors
// Licensed under Apache 2.0 License
//
// See https://github.com/dcl-project/dcl/LICENSE.txt for license information
// See https://github.com/dcl-project/dcl/graphs/contributors for the list of
// DCL project authors
//
//===----------------------------------------------------------------------===//

#ifndef DCL_BINARY_DARWIN_DYLD_DYLD_INFO_H
#define DCL_BINARY_DARWIN_DYLD_DYLD_INFO_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <dcl/Binary/Darwin/MachO.h>
#include <dcl/Binary/Darwin/Utilities.h>

#include <cstdint>
#include <iterator>
#include <utility>

#include <mach-o/loader.h>

namespace dcl {

namespace Binary {

namespace Darwin {

namespace Dyld {

class BindOpcode {

public:
  enum class Kind : uint8_t {
#define DYLD_BIND_OPCODE(CASE_NAME, CONSTANT, ...) CASE_NAME = CONSTANT,
#include <dcl/Binary/Darwin/Dyld/BindOpcode.def>
#undef DYLD_BIND_OPCODE
  };

  enum class SubOpcode : uint8_t {
#define DYLD_BIND_SUB_OPCODE(CASE_NAME, CONSTANT, ...) CASE_NAME = CONSTANT,
#include <dcl/Binary/Darwin/Dyld/BindSubOpcode.def>
#undef DYLD_BIND_SUB_OPCODE
  };

private:
  uint8_t _raw;

public:
  DCL_ALWAYS_INLINE
  BindOpcode(uint8_t raw) : _raw(raw) {}

  DCL_ALWAYS_INLINE
  uint8_t getRaw() const { return _raw; }

  DCL_ALWAYS_INLINE
  const uint8_t *getAddress() const { return (uint8_t *)&_raw; }

  DCL_ALWAYS_INLINE
  const uint8_t *getTrailingContentsAddress() const { return getAddress() + 1; }

  DCL_ALWAYS_INLINE
  Kind getKind() const { return Kind(_raw & BIND_OPCODE_MASK); }

  DCL_ALWAYS_INLINE
  uint8_t getImmediate() const { return _raw & BIND_IMMEDIATE_MASK; }
};

class BindOpcodeIterator {

public:
  using value_type = BindOpcode;
  using difference_type = ptrdiff_t;
  using pointer = typename std::add_pointer<value_type>::type;
  using const_pointer = typename std::add_const<pointer>::type;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using const_reference = typename std::add_const<reference>::type;
  using iterator_category = std::forward_iterator_tag;

private:
  const uint8_t *_begin;

  const uint8_t *_address;

  const uint8_t *_end;

private:
  DCL_ALWAYS_INLINE
  void advance() {

// Dyld bind opcode consumer
#define DYLD_CONSUME() (_address += 1)
#define DYLD_CONSUME_ULEB() (readUleb128(_address, _end))
#define DYLD_CONSUME_SLEB() (readSleb128(_address, _end))
#define DYLD_CONSUME_NULL_TERMINATED_STRING()                                  \
  while (*_address != '\0') {                                                  \
    ++_address;                                                                \
  }                                                                            \
  ++_address;
#define DYLD_CONSUME_SUB_OPCODE()                                              \
  {                                                                            \
    DYLD_CONSUME();                                                            \
    auto subopcode = BindOpcode::SubOpcode(immediate);                         \
    switch (subopcode) {                                                       \
    case BindOpcode::SubOpcode::SetBindOrdinalTableSizeUleb:                   \
      DYLD_CONSUME_ULEB();                                                     \
      return;                                                                  \
    case BindOpcode::SubOpcode::Apply:                                         \
      return;                                                                  \
    default:                                                                   \
      dcl::unreachable("Unsupported subopcode: %hhu\n", subopcode);            \
    }                                                                          \
  }

// Dyld bind opcode consumer invocator
#define DYLD_CONSUME_DO(INDEX, ARG) ARG()

// Dyld bind opcode branching
#define DYLD_BIND_OPCODE(CASE_NAME, CONSTANT, DESC, ...)                       \
  case BindOpcode::Kind::CASE_NAME: {                                          \
    DCL_META_FOREACH(DYLD_CONSUME_DO, ;, __VA_ARGS__);                         \
    break;                                                                     \
  }

    BindOpcode::Kind kind = operator*().getKind();
    uint8_t immediate = operator*().getImmediate();
    switch (kind) {
#include <dcl/Binary/Darwin/Dyld/BindOpcode.def>
    default:
      dcl::unreachable("Unsupported opcode: %hhu\n", kind);
    }
  }

public:
  DCL_ALWAYS_INLINE
  BindOpcodeIterator(const uint8_t *begin, const uint8_t *end)
      : BindOpcodeIterator(begin, begin, end) {}

  DCL_ALWAYS_INLINE
  BindOpcodeIterator(const uint8_t *begin, const uint8_t *address,
                     const uint8_t *end)
      : _begin(begin), _address(address), _end(end) {}

  DCL_ALWAYS_INLINE
  const uint8_t *getBegin() const { return _begin; }

  DCL_ALWAYS_INLINE
  const uint8_t *getAddress() const { return _address; }

  DCL_ALWAYS_INLINE
  const uint8_t *getEnd() const { return _end; }

  DCL_ALWAYS_INLINE
  const ptrdiff_t getOffset() const { return _address - _begin; }

  DCL_ALWAYS_INLINE
  BindOpcodeIterator &operator++() {
    advance();
    return *this;
  }

  DCL_ALWAYS_INLINE
  BindOpcodeIterator operator++(int arg0) {
    auto iterator = *this;
    ++(*this);
    return iterator;
  }

  DCL_ALWAYS_INLINE
  bool operator==(BindOpcodeIterator other) const {
    return _address == other._address;
  }

  DCL_ALWAYS_INLINE
  bool operator!=(BindOpcodeIterator other) const { return !(*this == other); }

  DCL_ALWAYS_INLINE
  reference operator*() const {
    return *const_cast<BindOpcode *>(
        reinterpret_cast<const BindOpcode *>(_address));
  }

  DCL_ALWAYS_INLINE
  pointer operator->() const {
    return const_cast<BindOpcode *>(
        reinterpret_cast<const BindOpcode *>(_address));
  }
};

class BindOpcodeStream {

private:
  const uint8_t *_begin;

  const uint8_t *_end;

public:
  DCL_ALWAYS_INLINE
  BindOpcodeStream(const uint8_t *begin, const uint8_t *end)
      : _begin(begin), _end(end) {}

  using Iterator = BindOpcodeIterator;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() { return Iterator{std::as_const(*this).begin()}; }

  DCL_ALWAYS_INLINE
  Iterator end() { return Iterator{std::as_const(*this).end()}; }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const { return Iterator{_begin, _end}; }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const { return Iterator{_begin, _end, _end}; }

  DCL_ALWAYS_INLINE
  uintptr_t getUleb128(const uint8_t *&begin) const {
    return readUleb128(begin, _end);
  }

  DCL_ALWAYS_INLINE
  uintptr_t getSleb128(const uint8_t *&begin) const {
    return readSleb128(begin, _end);
  }
};

} // namespace Dyld

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_DYLD_DYLD_INFO_H
