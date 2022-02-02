//===--- ByteOrder.h - Defines ByteOrder ------------------------*- C++ -*-===//
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

#ifndef DCL_ADT_BYTEORDER_H
#define DCL_ADT_BYTEORDER_H

#include <dcl/Basic/Basic.h>

#include <cstdint>
#include <limits>

namespace dcl::ADT {

namespace details {

template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapLittleToHost(ResultTy x);

template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapBigToHost(ResultTy x);

template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapHostToLittle(ResultTy x);

template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapHostToBig(ResultTy x);

} // namespace details

/** concept `ByteOrder`

 */
class LittleEndianess {

public:
  template <typename ResultTy>
  DCL_ALWAYS_INLINE
  static ResultTy swapToHost(ResultTy x) {
    static_assert(std::numeric_limits<ResultTy>::is_integer);
    return details::SwapLittleToHost(x);
  }

  template <typename ResultTy>
  DCL_ALWAYS_INLINE
  static ResultTy swapFromHost(ResultTy x) {
    static_assert(std::numeric_limits<ResultTy>::is_integer);
    return details::SwapHostToLittle(x);
  }
};

class BigEndianess {

public:
  template <typename ResultTy>
  DCL_ALWAYS_INLINE
  static ResultTy swapToHost(ResultTy x) {
    static_assert(std::numeric_limits<ResultTy>::is_integer);
    return details::SwapBigToHost(x);
  }

  template <typename ResultTy>
  DCL_ALWAYS_INLINE
  static ResultTy swapFromHost(ResultTy x) {
    static_assert(std::numeric_limits<ResultTy>::is_integer);
    return details::SwapHostToBig(x);
  }
};

#if __LITTLE_ENDIAN__
using HostByteOrder = LittleEndianess;
#endif
#if __BIG_ENDIAN__
using HostByteOrder = BigEndianess;
#endif

#pragma mark - Implementation Details

namespace details {

template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapLittleToHost(ResultTy x) { return ResultTy(); }
template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapBigToHost(ResultTy x) { return ResultTy(); }
template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapHostToLittle(ResultTy x) { return ResultTy(); }
template <typename ResultTy>
DCL_UNUSED
DCL_ALWAYS_INLINE
static ResultTy SwapHostToBig(ResultTy x) { return ResultTy(); }

} // namespace details

#pragma mark - Implementations for Darwin

#ifdef DCL_TARGET_OS_DARWIN

#include <libkern/OSByteOrder.h>

namespace details {
template <>
DCL_ALWAYS_INLINE
uint16_t SwapLittleToHost(uint16_t x) { return OSSwapLittleToHostInt16(x); }
template <>
DCL_ALWAYS_INLINE
uint32_t SwapLittleToHost(uint32_t x) { return OSSwapLittleToHostInt32(x); }
template <>
DCL_ALWAYS_INLINE
uint64_t SwapLittleToHost(uint64_t x) { return OSSwapLittleToHostInt64(x); }
template <>
DCL_ALWAYS_INLINE
uint16_t SwapBigToHost(uint16_t x) { return OSSwapBigToHostInt16(x); }
template <>
DCL_ALWAYS_INLINE
uint32_t SwapBigToHost(uint32_t x) { return OSSwapBigToHostInt32(x); }
template <>
DCL_ALWAYS_INLINE
uint64_t SwapBigToHost(uint64_t x) { return OSSwapBigToHostInt64(x); }
template <>
DCL_ALWAYS_INLINE
uint16_t SwapHostToLittle(uint16_t x) { return OSSwapHostToLittleInt16(x); }
template <>
DCL_ALWAYS_INLINE
uint32_t SwapHostToLittle(uint32_t x) { return OSSwapHostToLittleInt32(x); }
template <>
DCL_ALWAYS_INLINE
uint64_t SwapHostToLittle(uint64_t x) { return OSSwapHostToLittleInt64(x); }
template <>
DCL_ALWAYS_INLINE
uint16_t SwapHostToBig(uint16_t x) { return OSSwapHostToBigInt16(x); }
template <>
DCL_ALWAYS_INLINE
uint32_t SwapHostToBig(uint32_t x) { return OSSwapHostToBigInt32(x); }
template <>
DCL_ALWAYS_INLINE
uint64_t SwapHostToBig(uint64_t x) { return OSSwapHostToBigInt64(x); }

} // namespace details

#endif // DCL_TARGET_OS_DARWIN

} // namespace dcl::ADT

#endif // DCL_ADT_BYTEORDER_H
