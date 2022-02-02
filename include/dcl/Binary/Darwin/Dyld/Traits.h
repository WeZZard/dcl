//===--- Traits.h - Darwin Dyld Traits --------------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_DYLD_TRAITS_H
#define DCL_BINARY_DARWIN_DYLD_TRAITS_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <cstddef>
#include <mach-o/fixup-chains.h>

namespace dcl::Binary::Darwin::Dyld {

#pragma mark - Chained-Fixups Import Format Traits

template <typename ByteOrder>
class ChainedImport;

template <typename ByteOrder>
class ChainedImportAddend;

template <typename ByteOrder>
class ChainedImportAddend64;

template <typename Format>
class ChainedImportTraits {};

template <typename ByteOrder>
class ChainedImportTraits<ChainedImport<ByteOrder>> {
public:
  using UnderlyingTy = dyld_chained_import;
  DCL_CONSTEXPR
  static size_t Stride = sizeof(UnderlyingTy);
};

template <typename ByteOrder>
class ChainedImportTraits<ChainedImportAddend<ByteOrder>> {
public:
  using UnderlyingTy = dyld_chained_import_addend;
  DCL_CONSTEXPR
  static size_t Stride = sizeof(UnderlyingTy);
};

template <typename ByteOrder>
class ChainedImportTraits<ChainedImportAddend64<ByteOrder>> {
public:
  using UnderlyingTy = dyld_chained_import_addend64;
  DCL_CONSTEXPR
  static size_t Stride = sizeof(UnderlyingTy);
};

#pragma mark - Chained-Pointer Traits

class ChainedPointer64Prototype;
class ChainedPointer64;
class ChainedPointer64Bind;
class ChainedPointer64Rebase;

class ChainedPointerArm64EPrototype;
class ChainedPointerArm64E;
class ChainedPointerArm64EBind;
class ChainedPointerArm64ERebase;
class ChainedPointerArm64EAuthBind;
class ChainedPointerArm64EAuthRebase;

template <typename PointerFormat>
class ChainedPointerTraits;

template <>
class ChainedPointerTraits<ChainedPointer64> {
public:
  using PrototypeTy = ChainedPointer64Prototype;
  using UnderlyingTy = ChainedPointer64Prototype;
};

template <>
class ChainedPointerTraits<ChainedPointer64Bind> {
public:
  using PrototypeTy = ChainedPointer64Prototype;
  using UnderlyingTy = dyld_chained_ptr_64_bind;
};

template <>
class ChainedPointerTraits<ChainedPointer64Rebase> {
public:
  using PrototypeTy = ChainedPointer64Prototype;
  using UnderlyingTy = dyld_chained_ptr_64_rebase;
};

template <>
class ChainedPointerTraits<ChainedPointerArm64E> {
public:
  using PrototypeTy = ChainedPointerArm64EPrototype;
  using UnderlyingTy = ChainedPointerArm64EPrototype;
};

template <>
class ChainedPointerTraits<ChainedPointerArm64EBind> {
public:
  using PrototypeTy = ChainedPointerArm64EPrototype;
  using UnderlyingTy = dyld_chained_ptr_arm64e_bind;
};

template <>
class ChainedPointerTraits<ChainedPointerArm64ERebase> {
public:
  using PrototypeTy = ChainedPointerArm64EPrototype;
  using UnderlyingTy = dyld_chained_ptr_arm64e_rebase;
};

template <>
class ChainedPointerTraits<ChainedPointerArm64EAuthBind> {
public:
  using PrototypeTy = ChainedPointerArm64EPrototype;
  using UnderlyingTy = dyld_chained_ptr_arm64e_auth_bind;
};

template <>
class ChainedPointerTraits<ChainedPointerArm64EAuthRebase> {
public:
  using PrototypeTy = ChainedPointerArm64EPrototype;
  using UnderlyingTy = dyld_chained_ptr_arm64e_auth_rebase;
};

} // namespace dcl::Binary::Darwin::Dyld

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_DYLD_TRAITS_H
