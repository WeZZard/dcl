//===--- Fat.h - Abstraction for Fat ----------------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_FAT_H
#define DCL_BINARY_DARWIN_FAT_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <cstdint>
#include <iterator>
#include <mach-o/fat.h>

#include <dcl/Platform/TypeWrapper.h>

namespace dcl {

namespace Binary {

namespace Darwin {

template <typename Target, typename ByteOrder>
class FatHeader
  : public Platform::TypeWrapper<typename Target::FatHeaderTy, ByteOrder> {

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, Magic, magic);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, ArchCount, nfat_arch);
};
template <typename Target, typename ByteOrder>
class FatArch
  : public Platform::TypeWrapper<typename Target::FatArchTy, ByteOrder> {

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, Offset, offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Size, size);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Align, align);
};

template <typename Target, typename Endianess>
class FatArchCollection {

private:
  FatHeader<Target, Endianess> _header;

public:
  FatArchCollection(FatHeader<Target, Endianess> header) = delete;
  FatArchCollection(const FatArchCollection&) = delete;
  FatArchCollection& operator=(const FatArchCollection&) = delete;

  FatHeader<Target, Endianess> * getHeader() { return &_header; }

  const FatHeader<Target, Endianess> * getHeader() const { return &_header; }

  using Iterator = FatArch<Target, Endianess> *;
  using ConstIterator = const FatArch<Target, Endianess> *;

  DCL_ALWAYS_INLINE
  Iterator begin() {
    return const_cast<Iterator>(std::as_const(*this).begin());
  }

  DCL_ALWAYS_INLINE
  Iterator end() { return const_cast<Iterator>(std::as_const(*this).end()); }

  DCL_ALWAYS_INLINE
  const Iterator begin() const {
    return const_cast<Iterator>(std::as_const(*this).begin());
  }

  DCL_ALWAYS_INLINE
  const Iterator end() const {
    return const_cast<Iterator>(std::as_const(*this).end());
  }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const {
    return reinterpret_cast<ConstIterator>(
      getHeader()->getBase() + (uintptr_t)sizeof(getHeader()));
  }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const { return cbegin() + getHeader()->getArchCount(); }

  DCL_ALWAYS_INLINE
  FatArch<Target, Endianess>& at(uint32_t index) { return *(begin() + index); }

  DCL_ALWAYS_INLINE
  const FatArch<Target, Endianess>& at(uint32_t index) const {
    return *(cbegin() + index);
  }
};

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_FAT_H
