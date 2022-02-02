//===--- Format.h - Darwin Binary Formats -----------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_FORMAT_H
#define DCL_BINARY_DARWIN_FORMAT_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <cstdint>
#include <mach-o/fat.h>
#include <mach-o/loader.h>

namespace dcl {

namespace Binary {

namespace Darwin {

class FatMagic {
public:
  constexpr static const uint32_t big32 = FAT_MAGIC;
  constexpr static const uint32_t little32 = FAT_CIGAM;
  constexpr static const uint32_t big64 = FAT_MAGIC_64;
  constexpr static const uint32_t little64 = FAT_CIGAM_64;
};

class MachOMagic {
public:
  constexpr static const uint32_t big32 = MH_MAGIC;
  constexpr static const uint32_t little32 = MH_CIGAM;
  constexpr static const uint32_t big64 = MH_MAGIC_64;
  constexpr static const uint32_t little64 = MH_CIGAM_64;
};

enum class Format : uint8_t {
  LittleEndianess32Bit,
  BigEndianess32Bit,
  LittleEndianess64Bit,
  BigEndianess64Bit,
  Unknown,
};

template <class Magic>
DCL_ALWAYS_INLINE
static Format GetFormatWithBytes(const void * bytes) noexcept {

  const uint32_t magic = *static_cast<const uint32_t *>(bytes);

  if (Magic::little64 == magic) {
    return Format::LittleEndianess64Bit;
  }

  if (Magic::big64 == magic) {
    return Format::BigEndianess64Bit;
  }

  if (Magic::little32 == magic) {
    return Format::LittleEndianess32Bit;
  }

  if (Magic::big32 == magic) {
    return Format::BigEndianess32Bit;
  }

  return Format::Unknown;
}

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_FORMAT_H
