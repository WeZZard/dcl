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

#ifndef DCL_BINARY_DARWIN_TARGETS_H
#define DCL_BINARY_DARWIN_TARGETS_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <cstddef>
#include <mach-o/fixup-chains.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

namespace dcl::Binary::Darwin {

enum class LoadCommandKind32 : uint32_t;
enum class LoadCommandKind64 : uint32_t;

template <class WordTy>
class RemoteBase;

template <>
class RemoteBase<uint32_t> {
public:
  using PointerValueTy = uint32_t;
  using RelativePointerValueTy = uint16_t;

  using MachHeaderTy = mach_header;
  using LoadCommandKindTy = LoadCommandKind32;
  using SegmentCommandTy = segment_command;
  using SectionTy = section;
  using NlistTy = struct nlist;
};

template <>
class RemoteBase<uint64_t> {
public:
  using PointerValueTy = uint64_t;
  using RelativePointerValueTy = uint32_t;

  using MachHeaderTy = mach_header_64;
  using LoadCommandKindTy = LoadCommandKind64;
  using SegmentCommandTy = segment_command_64;
  using SectionTy = section_64;
  using NlistTy = nlist_64;
};

template <class WordTy>
class Remote : public RemoteBase<WordTy> {
public:
  using PointerTy = void *;

  using LoadCommandTy = load_command;
  using DylibTy = dylib;
  using DylibCommandTy = dylib_command;
  using LinkEditDataCommandTy = linkedit_data_command;
  using DyldInfoCommandTy = dyld_info_command;
  using RPathCommandTy = rpath_command;
  using SymbolTableCommandTy = symtab_command;
  using DyldChainedFixupsHeaderTy = struct dyld_chained_fixups_header;
  using DyldChainedStartsInImageTy = struct dyld_chained_starts_in_image;
  using DyldChainedStartsInSegmentTy = struct dyld_chained_starts_in_segment;

  DCL_CONSTEXPR
  static const size_t wordSize = sizeof(PointerTy);
};

template <int wordSize>
class InProcessBase;

template <>
class InProcessBase<sizeof(uint32_t)> {
public:
  using PointerValueTy = uint32_t;
  using RelativePointerValueTy = uint16_t;

  using MachHeaderTy = mach_header;
  using LoadCommandKindTy = LoadCommandKind32;
  using SegmentCommandTy = segment_command;
  using SectionTy = section;
  using NlistTy = struct nlist;
};

template <>
class InProcessBase<sizeof(uint64_t)> {
public:
  using PointerValueTy = uint64_t;
  using RelativePointerValueTy = uint32_t;

  using MachHeaderTy = mach_header_64;
  using LoadCommandKindTy = LoadCommandKind64;
  using SegmentCommandTy = segment_command_64;
  using SectionTy = section_64;
  using NlistTy = nlist_64;
};

class InProcess : public InProcessBase<sizeof(uintptr_t)> {
public:
  using PointerTy = void *;

  using LoadCommandTy = load_command;
  using DylibTy = dylib;
  using DylibCommandTy = dylib_command;
  using LinkEditDataCommandTy = linkedit_data_command;
  using DyldInfoCommandTy = dyld_info_command;
  using RPathCommandTy = rpath_command;
  using SymbolTableCommandTy = symtab_command;
  using DyldChainedFixupsHeaderTy = struct dyld_chained_fixups_header;
  using DyldChainedStartsInImageTy = struct dyld_chained_starts_in_image;
  using DyldChainedStartsInSegmentTy = struct dyld_chained_starts_in_segment;

  DCL_CONSTEXPR
  static const size_t wordSize = sizeof(PointerTy);
};

} // namespace dcl::Binary::Darwin

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_TARGETS_H
