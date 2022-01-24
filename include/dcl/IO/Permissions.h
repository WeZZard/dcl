//===--- Permissions.h - IO Permissions -------------------------*- C++ -*-===//
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

#ifndef DCL_IO_PERMISSIONS_H
#define DCL_IO_PERMISSIONS_H

#include <dcl/Basic/Basic.h>

namespace dcl {

namespace IO {

enum class Permissions : uint8_t {

  None = 0x0,
  Read = 0x1,
  Write = 0x2,
  Execute = 0x4,
  ReadWrite = Read | Write,
  Full = Read | Write | Execute,

};

DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Permissions operator|(Permissions l, Permissions r) noexcept;

DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Permissions operator&(Permissions l, Permissions r) noexcept;

DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Permissions operator~(Permissions c) noexcept;

namespace {

template <typename T>
using Underlying = typename std::underlying_type<T>::type;
template <typename T>
DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Underlying<T> underlying(T t) { return Underlying<T>(t); }

} // namespace

DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Permissions operator|(Permissions l, Permissions r) noexcept {
  return Permissions(underlying(l) | underlying(r));
}
DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Permissions operator&(Permissions l, Permissions r) noexcept {
  return Permissions(underlying(l) & underlying(r));
}
DCL_ALWAYS_INLINE
DCL_CONSTEXPR
Permissions operator~(Permissions c) noexcept {
  return Permissions(~underlying(c));
}

} // namespace IO

} // namespace dcl

#endif // DCL_IO_PERMISSIONS_H
