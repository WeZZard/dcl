//===--- MachO.h - Abstraction for MachO ------------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_MACHO_H
#define DCL_BINARY_DARWIN_MACHO_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <dcl/ADT/PlatformTypeWrapper.h>
#include <dcl/Binary/Darwin/Targets.h>
#include <dcl/Binary/Darwin/Traits.h>

#include <cstdint>
#include <mach-o/loader.h>

namespace dcl {

namespace Binary {

namespace Darwin {

#pragma mark - Supporting Types

template <typename Target, typename ByteOrder>
class Dylib
  : public ADT::PlatformTypeWrapper<typename Target::DylibTy, ByteOrder> {

public:
  DCL_PLATFORM_TYPE_GETTER(lc_str, Name, name);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Timestamp, timestamp);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, CurrentVersion, current_version);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, CompatibleVersion, compatibility_version);
};

enum class LoadCommandKind32 : uint32_t {
  Semgent = LC_SEGMENT,
  StabSymbolTable = LC_SYMTAB,
  GdbSymbolTable = LC_SYMSEG,
  Thread = LC_THREAD,
  UnixThread = LC_UNIXTHREAD,
  FixedVMSharedLibraryLoad = LC_LOADFVMLIB,
  FixedVMSharedLibraryIdentification = LC_IDFVMLIB,
  ObjectIdentification = LC_IDENT,
  FixedVMFileInclusion = LC_FVMFILE,
  Prepage = LC_PREPAGE,
  DynamicLinkEditSymbolTable = LC_DYSYMTAB,
  DylibLoad = LC_LOAD_DYLIB,
  DylibIdentification = LC_ID_DYLIB,
  DynamicLinkerLoad = LC_LOAD_DYLINKER,
  DynamicLinkerIdentification = LC_ID_DYLINKER,
  PreboundDylib = LC_PREBOUND_DYLIB,
  Routines = LC_ROUTINES,
  SubFramework = LC_SUB_FRAMEWORK,
  SubUmbrella = LC_SUB_UMBRELLA,
  SubClient = LC_SUB_CLIENT,
  SubLibrary = LC_SUB_LIBRARY,
  TwoLevelHints = LC_TWOLEVEL_HINTS,
  PrebindChecksum = LC_PREBIND_CKSUM,
  DylibLoadWeak = LC_LOAD_WEAK_DYLIB,
  UUID = LC_UUID,
  RPath = LC_RPATH,
  CodeSignature = LC_CODE_SIGNATURE,
  SegmentSplitInfo = LC_SEGMENT_SPLIT_INFO,
  ReexportDylib = LC_REEXPORT_DYLIB,
  DylibLoadLazy = LC_LAZY_LOAD_DYLIB,
  EncriptionInfo = LC_ENCRYPTION_INFO,
  DyldInfo = LC_DYLD_INFO,
  DyldInfoOnly = LC_DYLD_INFO_ONLY,
  DylibLoadUpward = LC_LOAD_UPWARD_DYLIB,
  VersionMin_macOS = LC_VERSION_MIN_MACOSX,
  VersionMin_iOS = LC_VERSION_MIN_IPHONEOS,
  VersionMin_tvOS = LC_VERSION_MIN_TVOS,
  VersionMin_watchOS = LC_VERSION_MIN_WATCHOS,
  FunctionStarts = LC_FUNCTION_STARTS,
  DyldEnvironment = LC_DYLD_ENVIRONMENT,
  Main = LC_MAIN,
  DataInCode = LC_DATA_IN_CODE,
  SourceVersion = LC_SOURCE_VERSION,
  DylibCodeSignDRs = LC_DYLIB_CODE_SIGN_DRS,
  LinkerOption = LC_LINKER_OPTION,
  LinkerOptimizationHint = LC_LINKER_OPTIMIZATION_HINT,
  Note = LC_NOTE,
  BuildVersion = LC_BUILD_VERSION,
  DyldExportsTrie = LC_DYLD_EXPORTS_TRIE,
  DyldChainedFixups = LC_DYLD_CHAINED_FIXUPS,
  FilesetEntry = LC_FILESET_ENTRY,
};

enum class LoadCommandKind64 : uint32_t {
  Semgent = LC_SEGMENT_64,
  StabSymbolTable = LC_SYMTAB,
  GdbSymbolTable = LC_SYMSEG,
  Thread = LC_THREAD,
  UnixThread = LC_UNIXTHREAD,
  FixedVMSharedLibraryLoad = LC_LOADFVMLIB,
  FixedVMSharedLibraryIdentification = LC_IDFVMLIB,
  ObjectIdentification = LC_IDENT,
  FixedVMFileInclusion = LC_FVMFILE,
  Prepage = LC_PREPAGE,
  DynamicLinkEditSymbolTable = LC_DYSYMTAB,
  DylibLoad = LC_LOAD_DYLIB,
  DylibIdentification = LC_ID_DYLIB,
  DynamicLinkerLoad = LC_LOAD_DYLINKER,
  DynamicLinkerIdentification = LC_ID_DYLINKER,
  PreboundDylib = LC_PREBOUND_DYLIB,
  Routines = LC_ROUTINES_64,
  SubFramework = LC_SUB_FRAMEWORK,
  SubUmbrella = LC_SUB_UMBRELLA,
  SubClient = LC_SUB_CLIENT,
  SubLibrary = LC_SUB_LIBRARY,
  TwoLevelHints = LC_TWOLEVEL_HINTS,
  PrebindChecksum = LC_PREBIND_CKSUM,
  DylibLoadWeak = LC_LOAD_WEAK_DYLIB,
  UUID = LC_UUID,
  RPath = LC_RPATH,
  CodeSignature = LC_CODE_SIGNATURE,
  SegmentSplitInfo = LC_SEGMENT_SPLIT_INFO,
  ReexportDylib = LC_REEXPORT_DYLIB,
  DylibLoadLazy = LC_LAZY_LOAD_DYLIB,
  EncriptionInfo = LC_ENCRYPTION_INFO_64,
  DyldInfo = LC_DYLD_INFO,
  DyldInfoOnly = LC_DYLD_INFO_ONLY,
  DylibLoadUpward = LC_LOAD_UPWARD_DYLIB,
  VersionMin_macOS = LC_VERSION_MIN_MACOSX,
  VersionMin_iOS = LC_VERSION_MIN_IPHONEOS,
  VersionMin_tvOS = LC_VERSION_MIN_TVOS,
  VersionMin_watchOS = LC_VERSION_MIN_WATCHOS,
  FunctionStarts = LC_FUNCTION_STARTS,
  DyldEnvironment = LC_DYLD_ENVIRONMENT,
  Main = LC_MAIN,
  DataInCode = LC_DATA_IN_CODE,
  SourceVersion = LC_SOURCE_VERSION,
  DylibCodeSignDRs = LC_DYLIB_CODE_SIGN_DRS,
  LinkerOption = LC_LINKER_OPTION,
  LinkerOptimizationHint = LC_LINKER_OPTIMIZATION_HINT,
  Note = LC_NOTE,
  BuildVersion = LC_BUILD_VERSION,
  DyldExportsTrie = LC_DYLD_EXPORTS_TRIE,
  DyldChainedFixups = LC_DYLD_CHAINED_FIXUPS,
  FilesetEntry = LC_FILESET_ENTRY,
};

#pragma mark - Mach-O Header

template <typename Target, typename ByteOrder>
class MachHeader
  : public ADT::PlatformTypeWrapper<typename Target::MachHeaderTy, ByteOrder> {

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, Magic, magic);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, NumberOfCommands, cnmds);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, SizeOfCommands, sizeofcmds);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Flags, flags);
};

#pragma mark - Load Commands

template <typename Derived>
class LoadCommandBase : public ADT::PlatformTypeWrapper<
                          typename LoadCommandTraits<Derived>::CommandTy,
                          typename LoadCommandTraits<Derived>::ByteOrderTy> {

public:
  using Target = typename LoadCommandTraits<Derived>::TargetTy;
  using ByteOrder = typename LoadCommandTraits<Derived>::ByteOrderTy;
  using CommandKind = typename Target::LoadCommandKindTy;

public:
  DCL_PLATFORM_TYPE_GETTER(CommandKind, Command, cmd);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, CommandSize, cmdsize);

protected:
  const char * getString(const union lc_str& str) {
    return const_cast<const char *>(std::as_const(*this).getString());
  }

  const char * const getString(const union lc_str& str) const {
#ifndef __LP64__
    return str.ptr;
#else
    return reinterpret_cast<const char *>(&this->getBase() + str.offset);
#endif
  }
};

template <typename Target, typename ByteOrder>
class LoadCommand : public LoadCommandBase<LoadCommand<Target, ByteOrder>> {};

template <typename Target, typename ByteOrder>
class SegmentCommand
  : public LoadCommandBase<SegmentCommand<Target, ByteOrder>> {
public:
  using PointerValueTy = typename Target::PointerValueTy;

public:
  DCL_PLATFORM_TYPE_GETTER(const char *, SegmentName, segname);

  DCL_PLATFORM_TYPE_GETTER(PointerValueTy, VirtualMemoryAddress, vmaddr);

  DCL_PLATFORM_TYPE_GETTER(PointerValueTy, VirtualMemorySize, vmsize);

  DCL_PLATFORM_TYPE_GETTER(PointerValueTy, FileOffset, fileoff);

  DCL_PLATFORM_TYPE_GETTER(PointerValueTy, FileSize, filesize);

  DCL_PLATFORM_TYPE_GETTER(vm_prot_t, MaximumVirtualMemoryProtection, vmprot);

  DCL_PLATFORM_TYPE_GETTER(vm_prot_t, InitialVirtualMemoryProtection, initprot);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, SectionCount, nsects);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Flags, flags);

  void * getBase(PointerValueTy baseAddress) {
    return baseAddress + getVirtualMemoryAddress() - getFileOffset();
  }

  void * const getBase(PointerValueTy baseAddress) const {
    return baseAddress + getVirtualMemoryAddress() - getFileOffset();
  }
};

template <typename Target, typename ByteOrder>
class DyldInfoCommand
  : public LoadCommandBase<DyldInfoCommand<Target, ByteOrder>> {
public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, RebaseOffset, rebase_off);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, RebaseSize, reabse_size);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, BindingInfoOffset, bind_off);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, BindingInfoSize, bind_size);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, WeakBindingInfoOffset, weak_bind_off);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, WeakBindingInfoSize, weak_bind_size);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, LazyBindingInfoOffset, lazy_bind_off);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, LazyBindingInfoSize, lazy_bind_size);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, ExportInfoOffset, export_off);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, ExportInfoSize, export_size);
};

template <typename Target, typename ByteOrder>
class DylibCommand : public LoadCommandBase<DylibCommand<Target, ByteOrder>> {
public:
  DCL_ALWAYS_INLINE
  const Dylib<Target, ByteOrder>& getDylib() const {
    return *reinterpret_cast<const Dylib<Target, ByteOrder> *>(
      &this->getPlatformValue().dylib);
  }

  DCL_ALWAYS_INLINE
  Dylib<Target, ByteOrder>& getDylib() {
    return *reinterpret_cast<Dylib<Target, ByteOrder> *>(
      &this->getPlatformValue().dylib);
  }

  DCL_ALWAYS_INLINE
  const char * getDylibName() {
    return this->getString(this->getPlatformValue().dylib.name);
  }

  DCL_ALWAYS_INLINE
  const char * const getDylibName() const {
    return this->getString(this->getPlatformValue().dylib.name);
  }
};

template <typename Target, typename ByteOrder>
class RPathCommand : public LoadCommandBase<RPathCommand<Target, ByteOrder>> {
public:
  DCL_ALWAYS_INLINE
  const char * getPath() {
    return this->getString(this->getPlatformValue().path);
  }

  DCL_ALWAYS_INLINE
  const char * const getPath() const {
    return this->getString(this->getPlatformValue().path);
  }
};

template <typename Target, typename ByteOrder>
class SymbolTableCommand
  : public LoadCommandBase<SymbolTableCommand<Target, ByteOrder>> {
public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, SymbolTableOffset, symoff);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, NumberOfSymbolTableEntries, nsyms);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, StringTableOffset, stroff);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, StringTableSize, strsize);
};

template <typename Target, typename ByteOrder>
class LinkEditDataCommand
  : public LoadCommandBase<LinkEditDataCommand<Target, ByteOrder>> {
public:
  using PointerValueTy = typename Target::PointerValueTy;

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, DataOffset, dataoff);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, DataSize, datasize);
};

template <typename Target, typename ByteOrder>
class Section
  : public ADT::PlatformTypeWrapper<typename Target::SectionTy, ByteOrder> {

public:
  using PointerValueTy = typename Target::PointerValueTy;

public:
  DCL_PLATFORM_TYPE_GETTER(const char *, SectionName, sectname);

  DCL_PLATFORM_TYPE_GETTER(const char *, SegmentName, segname);

  DCL_PLATFORM_TYPE_GETTER(PointerValueTy, VirtualMemoryAddress, addr);

  DCL_PLATFORM_TYPE_GETTER(PointerValueTy, VirtualMemorySize, size);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, FileOffset, offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Alignment, align);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, RelocationOffset, reloff);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, RelocationOffsetCount, nreloc);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Flags, flags);

  void * getBase(PointerValueTy baseAddress) {
    return baseAddress + getVirtualMemoryAddress();
  }

  void * const getBase(PointerValueTy baseAddress) const {
    return baseAddress + getVirtualMemoryAddress();
  }
};

template <typename Target, typename ByteOrder>
class ExportsTrieCommand
  : public LoadCommandBase<ExportsTrieCommand<Target, ByteOrder>> {
public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, DataOffset, dataoff);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, DataSize, datasize);
};

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_MACHO_H
