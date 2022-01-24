//===--- Targets.h - Targets for Binary -------------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_UTILITIES_H
#define DCL_BINARY_DARWIN_UTILITIES_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <cstdint>

namespace dcl {

namespace Binary {

namespace Darwin {

DCL_ALWAYS_INLINE
static uintptr_t readUleb128(const uint8_t *&p, const uint8_t *end) {
  uint64_t result = 0;
  int bit = 0;
  do {
    if (p == end) {
      dcl::preconditionFailure("malformed uleb128\n");
    }
    uint64_t slice = *p & 0x7f;
    if (bit > 63) {
      dcl::preconditionFailure(
          "uleb128 too big for uint64, bit=%d, result=0x%0llX\n", bit, result);
    } else {
      result |= (slice << bit);
      bit += 7;
    }
  } while (*p++ & 0x80);
  return result;
}

DCL_ALWAYS_INLINE
static intptr_t readSleb128(const uint8_t *&p, const uint8_t *end) {
  int64_t result = 0;
  int bit = 0;
  uint8_t byte;
  do {
    if (p == end) {
      dcl::preconditionFailure("malformed sleb128\n");
    }
    byte = *p++;
    result |= (((int64_t)(byte & 0x7f)) << bit);
    bit += 7;
  } while (byte & 0x80);
  // sign extend negative numbers
  if ((byte & 0x40) != 0)
    result |= (-1LL) << bit;
  return result;
}

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_UTILITIES_H
