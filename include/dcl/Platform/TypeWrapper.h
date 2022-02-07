//===--- TypeWrapper.h - Platform Type Wrapper ------------------*- C++ -*-===//
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

#ifndef DCL_PLATFORM_TYPEWRAPPER_H
#define DCL_PLATFORM_TYPEWRAPPER_H

#include <dcl/Basic/Basic.h>
#include <dcl/Platform/ByteOrder.h>
#include <utility>

namespace dcl::Platform {

template <typename Wrapped, typename ByteOrder>
class TypeWrapper {

private:
  Wrapped _wrappedValue;

public:
  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  explicit TypeWrapper(Wrapped wrappedValue) noexcept
    : _wrappedValue(wrappedValue) {}

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  Wrapped& getWrappedValue() noexcept { return _wrappedValue; }

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  const Wrapped& getWrappedValue() const noexcept { return _wrappedValue; }

  DCL_ALWAYS_INLINE
  void setWrappedValue(Wrapped wrappedValue) noexcept {
    _wrappedValue = wrappedValue;
  }

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  const uint8_t * getBase() noexcept {
    return const_cast<const uint8_t *>(std::as_const(*this).getBase());
  }

  DCL_ALWAYS_INLINE
  DCL_CONSTEXPR
  const uint8_t * getBase() const noexcept {
    return reinterpret_cast<const uint8_t *>(this);
  }
};

#define DCL_PLATFORM_TYPE_GETTER(TYPE, NAME, MEMBER)                           \
  DCL_ALWAYS_INLINE                                                            \
  TYPE get##NAME() const {                                                     \
    if (std::is_same<ByteOrder, dcl::Platform::HostByteOrder>()) {             \
      return static_cast<TYPE>(this->getWrappedValue().MEMBER);                \
    }                                                                          \
    return static_cast<TYPE>(                                                  \
      ByteOrder::swapToHost(this->getWrappedValue().MEMBER));                  \
  }

#define DCL_PLATFORM_TYPE_SETTER(TYPE, NAME, MEMBER)                           \
  DCL_ALWAYS_INLINE                                                            \
  void set##NAME(TYPE x) {                                                     \
    using MemberTy = decltype(this->getWrappedValue().MEMBER);                 \
    if (std::is_same<ByteOrder, dcl::Platform::HostByteOrder>()) {             \
      this->getWrappedValue().MEMBER = static_cast<MemberTy>(x);               \
    }                                                                          \
    this->getWrappedValue().MEMBER =                                           \
      static_cast<MemberTy>(ByteOrder::swapFromHost(x));                       \
  }

#define DCL_PLATFORM_TYPE_ACCESSOR(TYPE, NAME, MEMBER)                         \
  DCL_PLATFORM_TYPE_GETTER(TYPE, NAME, MEMBER)                                 \
  DCL_PLATFORM_TYPE_SETTER(TYPE, NAME, MEMBER)

} // namespace dcl::Platform

#endif // DCL_PLATFORM_TYPEWRAPPER_H
