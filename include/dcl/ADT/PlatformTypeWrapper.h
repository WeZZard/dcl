
//===--- PlatformTypeWrapper.h - Platform Type Wrapper ----------*- C++ -*-===//
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

#ifndef DCL_ADT_PLATFORM_TYPE_WRAPPER_H
#define DCL_ADT_PLATFORM_TYPE_WRAPPER_H

#include <dcl/ADT/ByteOrder.h>
#include <dcl/Basic/Basic.h>

namespace dcl {

namespace ADT {

template <typename PlatformType, typename ByteOrder>
class PlatformTypeWrapper {

private:
  PlatformType _platformValue;

public:
  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  PlatformTypeWrapper(PlatformType platformValue) noexcept
      : _platformValue(platformValue) {}

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  PlatformType &getPlatformValue() noexcept { return _platformValue; }

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  const PlatformType &getPlatformValue() const noexcept {
    return _platformValue;
  }

  DCL_ALWAYS_INLINE
  void setPlatformValue(PlatformType platformValue) noexcept {
    _platformValue = platformValue;
  }

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  const uint8_t *getBase() noexcept { 
    return const_cast<const uint8_t *>(std::as_const(*this).getBase()); 
  }

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  const uint8_t *const getBase() const noexcept { 
    return reinterpret_cast<const uint8_t * const>(this); 
  }
};

#define DCL_PLATFORM_TYPE_GETTER(TYPE, NAME, MEMBER)                           \
  DCL_ALWAYS_INLINE                                                            \
  TYPE get##NAME() const {                                                     \
    if (std::is_same<ByteOrder, dcl::ADT::HostByteOrder>()) {                  \
      return static_cast<TYPE>(this->getPlatformValue().MEMBER);               \
    } else {                                                                   \
      return static_cast<TYPE>(                                                \
          ByteOrder::swapToHost(this->getPlatformValue().MEMBER));             \
    }                                                                          \
  }

#define DCL_PLATFORM_TYPE_SETTER(TYPE, NAME, MEMBER)                           \
  DCL_ALWAYS_INLINE                                                            \
  void set##NAME(TYPE x) {                                                     \
    using MemberTy = decltype(this->getPlatformValue().MEMBER);                \
    if (std::is_same<ByteOrder, dcl::ADT::HostByteOrder>()) {                  \
      this->getPlatformValue().MEMBER = static_cast<MemberTy>(x);              \
    } else {                                                                   \
      this->getPlatformValue().MEMBER =                                        \
          static_cast<MemberTy>(ByteOrder::swapFromHost(x));                   \
    }                                                                          \
  }

#define DCL_PLATFORM_TYPE_ACCESSOR(TYPE, NAME, MEMBER)                         \
  DCL_PLATFORM_TYPE_GETTER(TYPE, NAME, MEMBER)                                 \
  DCL_PLATFORM_TYPE_SETTER(TYPE, NAME, MEMBER)

} // namespace ADT

} // namespace dcl

#endif // DCL_ADT_PLATFORM_TYPE_WRAPPER_H
