//===--- FixupChains.h - Dyld Fixup Chains Link Edit Data ---*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_DYLD_DYLDFIXUPCHAINS_H
#define DCL_BINARY_DARWIN_DYLD_DYLDFIXUPCHAINS_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <dcl/Binary/Darwin/Dyld/Traits.h>
#include <dcl/Binary/Darwin/Iterators.h>
#include <dcl/Binary/Darwin/MachO.h>
#include <dcl/Platform/TypeWrapper.h>

#include <cstdint>
#include <utility>

#include <mach-o/fixup-chains.h>

namespace dcl::Binary::Darwin::Dyld {

// Relies on specific version of dyld fix-chain.
static_assert(__MACH_O_FIXUP_CHAINS__ == 6);

enum class ChainedPointerFormat : uint16_t {
  Generic32 = DYLD_CHAINED_PTR_32,
  Generic32Cache = DYLD_CHAINED_PTR_32_CACHE,
  Generic32Firmware = DYLD_CHAINED_PTR_32_FIRMWARE,
  Generic64 = DYLD_CHAINED_PTR_64,
  Generic64KernelCache = DYLD_CHAINED_PTR_64_KERNEL_CACHE,
  Generic64Offset = DYLD_CHAINED_PTR_64_OFFSET,
  Arm64E = DYLD_CHAINED_PTR_ARM64E,
  Arm64EFirmware = DYLD_CHAINED_PTR_ARM64E_FIRMWARE,
  Arm64EKernal = DYLD_CHAINED_PTR_ARM64E_KERNEL,
  Arm64EUserland24 = DYLD_CHAINED_PTR_ARM64E_USERLAND24,
  Arm64EUserland = DYLD_CHAINED_PTR_ARM64E_USERLAND,
};
static_assert(sizeof(ChainedPointerFormat) == sizeof(uint16_t));

enum class ChainedImportFormat : uint32_t {
  Generic = DYLD_CHAINED_IMPORT,
  Addend = DYLD_CHAINED_IMPORT_ADDEND,
  Addend64 = DYLD_CHAINED_IMPORT_ADDEND64,
};
static_assert(sizeof(ChainedImportFormat) == sizeof(uint32_t));

enum class ChainedSymbolFormat : uint32_t {
  Uncompressed = 0,
  ZlibCompressed = 1,
};
static_assert(sizeof(ChainedSymbolFormat) == sizeof(uint32_t));

template <typename Target, typename ByteOrder>
class ChainedStartsInImage;

template <typename Target, typename ByteOrder>
class ChainedFixupsHeader
  : public Platform::
      TypeWrapper<typename Target::DyldChainedFixupsHeaderTy, ByteOrder> {
public:
  using ImportFormat = ChainedImportFormat;
  using SymbolFormat = ChainedSymbolFormat;

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, FixupsVersion, fixups_version);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, StartsOffset, starts_offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, ImportsOffset, imports_offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, SymbolsOffset, symbols_offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, ImportsCount, imports_count);

  DCL_PLATFORM_TYPE_GETTER(ImportFormat, ImportsFormat, imports_format);

  DCL_PLATFORM_TYPE_GETTER(SymbolFormat, SymbolsFormat, symbols_format);

  DCL_ALWAYS_INLINE
  const ChainedStartsInImage<Target, ByteOrder> * getStarts() {
    return reinterpret_cast<ChainedStartsInImage<Target, ByteOrder> *>(
      this->getBase() + getStartsOffset());
  }

  DCL_ALWAYS_INLINE
  const ChainedStartsInImage<Target, ByteOrder> * const getStarts() const {
    return reinterpret_cast<ChainedStartsInImage<Target, ByteOrder> *>(
      this->getBase() + getStartsOffset());
  }
};

template <typename Target, typename ByteOrder>
class ChainedStartsInSegment;

template <typename Target, typename ByteOrder>
class ChainedStartsInImage
  : public Platform::
      TypeWrapper<typename Target::DyldChainedStartsInImageTy, ByteOrder> {

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, SegmentCount, seg_count);

  DCL_ALWAYS_INLINE
  uint32_t getSegmentInfoOffsetAtIndex(uint32_t index) {
    return this->getWrappedValue().seg_info_offset[index];
  }

  DCL_ALWAYS_INLINE
  uint32_t getSegmentInfoOffsetAtIndex(uint32_t index) const {
    return this->getWrappedValue().seg_info_offset[index];
  }
};

template <typename Target, typename ByteOrder>
class ChainedStartsInSegment
  : public Platform::
      TypeWrapper<typename Target::DyldChainedStartsInSegmentTy, ByteOrder> {
public:
  using PointerFormat = ChainedPointerFormat;

public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, Size, size);

  DCL_PLATFORM_TYPE_GETTER(uint16_t, PageSize, page_size);

  DCL_PLATFORM_TYPE_GETTER(PointerFormat, PointerFormat, pointer_format);

  DCL_PLATFORM_TYPE_GETTER(uint64_t, SegmentOffset, segment_offset);

  DCL_PLATFORM_TYPE_GETTER(uint64_t, MaxValidPointer, max_valid_pointer);

  DCL_PLATFORM_TYPE_GETTER(uint16_t, PageCount, page_count);

  DCL_ALWAYS_INLINE
  uint16_t getPageStartAtIndex(uint16_t index) const {
    return this->getWrappedValue().page_start[index];
  }
};

#pragma mark - Chained Import

template <typename ByteOrder>
class ChainedImport
  : public Platform::
      TypeWrapper<ChainedImportTraits<ChainedImport<ByteOrder>>, ByteOrder> {
public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, LibraryOrdinal, lib_ordinal);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, WeakImport, weak_import);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, NameOffset, name_offset);
};

template <typename ByteOrder>
class ChainedImportAddend
  : public Platform::TypeWrapper<
      ChainedImportTraits<ChainedImportAddend<ByteOrder>>,
      ByteOrder> {
public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, LibraryOrdinal, lib_ordinal);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, WeakImport, weak_import);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, NameOffset, name_offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Addend, addend);
};

template <typename ByteOrder>
class ChainedImportAddend64
  : public Platform::TypeWrapper<
      ChainedImportTraits<ChainedImportAddend64<ByteOrder>>,
      ByteOrder> {
public:
  DCL_PLATFORM_TYPE_GETTER(uint32_t, LibraryOrdinal, lib_ordinal);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, WeakImport, weak_import);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Reserved, reserved);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, NameOffset, name_offset);

  DCL_PLATFORM_TYPE_GETTER(uint32_t, Addend, addend);
};

template <
  typename Target,
  typename ByteOrder,
  template <typename>
  typename ImportFormat>
class ChainedImportIterator
  : public RandomAccessIterator<
      ChainedImportIterator<Target, ByteOrder, ImportFormat>> {
public:
  using ChainedImport = ImportFormat<ByteOrder>;
  using difference_type = typename IteratorTraits<
    ChainedImportIterator<Target, ByteOrder, ImportFormat>>::DifferenceTy;

private:
  ChainedImport * _import;

public:
  explicit ChainedImportIterator(ChainedImport * import) : _import(import) {}

  explicit ChainedImportIterator(const ChainedImport * import)
    : _import(import) {}

  DCL_ALWAYS_INLINE
  ChainedImport * get() { return _import; }

  DCL_ALWAYS_INLINE
  const ChainedImport * get() const { return _import; }

  DCL_ALWAYS_INLINE
  void advance(difference_type distance) { _import += distance; }

  DCL_ALWAYS_INLINE
  bool isEqual(
    const ChainedImportIterator<Target, ByteOrder, ImportFormat>& other) const {
    return get() == other.get();
  }

  DCL_ALWAYS_INLINE
  difference_type distance(
    const ChainedImportIterator<Target, ByteOrder, ImportFormat>& other) const {
    return get() - other.get();
  }

  DCL_ALWAYS_INLINE
  bool lessThan(
    const ChainedImportIterator<Target, ByteOrder, ImportFormat>& other) const {
    return get() < other.get();
  }
};

template <
  typename Target,
  typename ByteOrder,
  template <typename>
  typename ImportFormat>
class ChainedImportList {
private:
  ChainedFixupsHeader<Target, ByteOrder> * _header;

public:
  DCL_ALWAYS_INLINE
  explicit ChainedImportList(ChainedFixupsHeader<Target, ByteOrder> * header)
    : _header(header) {}

  DCL_ALWAYS_INLINE
  explicit ChainedImportList(
    const ChainedFixupsHeader<Target, ByteOrder> * header)
    : _header(const_cast<ChainedFixupsHeader<Target, ByteOrder> *>(header)) {}

  using Iterator = ChainedImportIterator<Target, ByteOrder, ImportFormat>;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() {
    return const_cast<Iterator>(std::as_const(*this).begin());
  }

  DCL_ALWAYS_INLINE
  Iterator end() { return const_cast<Iterator>(std::as_const(*this).end()); }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const {
    return Iterator{(uint8_t *)_header + _header->getImportsOffset()};
  }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const { return cbegin() + _header->getImportsCount(); }
};

#pragma mark - Chained-Pointer

#pragma mark Chained-Pointer Prototype

struct ChainedPointer64Prototype {
  uint64_t paddding : 51;
  uint64_t next : 12;
  uint64_t bind : 1;
};
struct ChainedPointerArm64EPrototype {
  uint64_t paddding : 51;
  uint64_t next : 11;
  uint64_t bind : 1;
  uint64_t auth : 1;
};

#pragma mark Chained-Pointer Base

template <typename Derived>
class ChainedPointerBase {
public:
  using Prototype = typename ChainedPointerTraits<Derived>::PrototypeTy;
  using Base = typename ChainedPointerTraits<Derived>::UnderlyingTy;

  static_assert(sizeof(Prototype) == sizeof(Base));

protected:
  union {
    Prototype _prototype;
    Base _base;
  };

  const uint8_t * getBase() const {
    return reinterpret_cast<const uint8_t *>(&_prototype);
  }

public:
  DCL_ALWAYS_INLINE
  bool isBind() { return std::as_const(*this).isBind(); }

  DCL_ALWAYS_INLINE
  bool isBind() const { return _prototype.bind; }

  DCL_ALWAYS_INLINE
  Derived * getNext() {
    return const_cast<Derived *>(std::as_const(*this).getNext());
  }

  DCL_ALWAYS_INLINE
  const Derived * getNext() const {
    if (_prototype.next == 0) {
      return nullptr;
    }
    return reinterpret_cast<Derived *>(getBase() + 4 * _prototype.next);
  }
};

#pragma mark Chained-Pointers

class ChainedPointer64Bind;
class ChainedPointerArm64EBind;
class ChainedPointerArm64EAuthBind;

class ChainedPointer64 : public ChainedPointerBase<ChainedPointer64> {};

class ChainedPointerArm64E : public ChainedPointerBase<ChainedPointerArm64E> {
public:
  DCL_ALWAYS_INLINE
  bool isAuth() { return std::as_const(*this).isAuth(); }

  DCL_ALWAYS_INLINE
  bool isAuth() const { return _base.auth; }
};

class ChainedPointer64Bind : public ChainedPointerBase<ChainedPointer64Bind> {

public:
  DCL_ALWAYS_INLINE
  static uint64_t
  bind(ChainedPointer64Bind * pointer, const void * const * addressList) {
    *reinterpret_cast<uint64_t *>(pointer) =
      *reinterpret_cast<const uint64_t *>(addressList + pointer->getOrdinal()) +
      pointer->getAddend();
    return *reinterpret_cast<uint64_t *>(pointer);
  }

  DCL_ALWAYS_INLINE
  uint64_t getOrdinal() { return _base.ordinal; }

  DCL_ALWAYS_INLINE
  uint64_t getOrdinal() const { return _base.ordinal; }

  DCL_ALWAYS_INLINE
  uint64_t getAddend() { return _base.addend; }

  DCL_ALWAYS_INLINE
  uint64_t getAddend() const { return _base.addend; }
};

class ChainedPointerArm64EBind
  : public ChainedPointerBase<ChainedPointerArm64EBind> {

public:
  DCL_ALWAYS_INLINE
  static uint64_t
  bind(ChainedPointerArm64EBind * pointer, const void * const * addressList) {
    *reinterpret_cast<uint64_t *>(pointer) =
      *reinterpret_cast<const uint64_t *>(addressList + pointer->getOrdinal()) +
      pointer->getAddend();
    return *reinterpret_cast<uint64_t *>(pointer);
  }

  DCL_ALWAYS_INLINE
  uint64_t getZero() { return _base.zero; }

  DCL_ALWAYS_INLINE
  uint64_t getZero() const { return _base.zero; }

  DCL_ALWAYS_INLINE
  uint64_t getOrdinal() { return _base.ordinal; }

  DCL_ALWAYS_INLINE
  uint64_t getOrdinal() const { return _base.ordinal; }

  DCL_ALWAYS_INLINE
  uint64_t getAddend() { return _base.addend; }

  DCL_ALWAYS_INLINE
  uint64_t getAddend() const { return _base.addend; }
};
class ChainedPointerArm64EAuthBind
  : public ChainedPointerBase<ChainedPointerArm64EAuthBind> {

public:
  DCL_ALWAYS_INLINE
  static void bind(
    ChainedPointerArm64EAuthBind * pointer,
    const void * const * addressList) {
    dcl::notImplemented();
  }

  DCL_ALWAYS_INLINE
  uint64_t getZero() { return _base.zero; }

  DCL_ALWAYS_INLINE
  uint64_t getZero() const { return _base.zero; }

  DCL_ALWAYS_INLINE
  uint64_t getDiversity() { return _base.diversity; }

  DCL_ALWAYS_INLINE
  uint64_t getDiversity() const { return _base.diversity; }

  DCL_ALWAYS_INLINE
  uint64_t getAddrDiv() { return _base.addrDiv; }

  DCL_ALWAYS_INLINE
  uint64_t getAddrDiv() const { return _base.addrDiv; }

  DCL_ALWAYS_INLINE
  uint64_t getKey() { return _base.key; }

  DCL_ALWAYS_INLINE
  uint64_t getKey() const { return _base.key; }
};

#pragma mark - Chained-Pointer Iterator

template <typename ChainedPointer>
class ChainedPointerIterator {
private:
  ChainedPointer * _base;

public:
  DCL_ALWAYS_INLINE
  ChainedPointerIterator<ChainedPointer>& operator++() {
    _base = _base->getNext();
    return *this;
  }

  DCL_ALWAYS_INLINE
  ChainedPointerIterator<ChainedPointer> operator++(int) {
    ChainedPointerIterator<ChainedPointer> iterator = *this;
    ++(*this);
    return iterator;
  }

  DCL_ALWAYS_INLINE
  bool operator==(ChainedPointerIterator<ChainedPointer> other) const {
    return _base == other._base;
  }

  DCL_ALWAYS_INLINE
  bool operator!=(ChainedPointerIterator<ChainedPointer> other) const {
    return !(*this == other);
  }

  DCL_ALWAYS_INLINE
  ChainedPointer operator*() {
    return const_cast<ChainedPointer>(std::as_const(*this).operator*());
  }

  DCL_ALWAYS_INLINE
  const ChainedPointer operator*() const { return *_base; }
};

#pragma mark - Chaiend-Fixups Page Info

template <typename Target, typename ByteOrder, typename Format>
class ChainedFixupsPageInfoIterator {

private:
  MachHeader<Target, ByteOrder> * _machHeader;

  ChainedStartsInSegment<Target, ByteOrder> * _startsInSegment;

  size_t _pageIndex;

public:
  ChainedFixupsPageInfoIterator(
    MachHeader<Target, ByteOrder> * machHeader,
    ChainedStartsInSegment<Target, ByteOrder> * startsInSegment,
    size_t pageIndex)
    : _machHeader(machHeader), _startsInSegment(startsInSegment),
      _pageIndex(pageIndex) {}

  DCL_ALWAYS_INLINE
  ChainedFixupsPageInfoIterator<Target, ByteOrder, Format>& operator++() {
    _pageIndex++;
    return *this;
  }

  DCL_ALWAYS_INLINE
  ChainedFixupsPageInfoIterator<Target, ByteOrder, Format> operator++(int) {
    ChainedFixupsPageInfoIterator<Target, ByteOrder, Format> iterator = *this;
    ++(*this);
    return iterator;
  }

  DCL_ALWAYS_INLINE
  bool operator==(
    ChainedFixupsPageInfoIterator<Target, ByteOrder, Format> other) const {
    return _machHeader == other.machHeader &&
           _startsInSegment == other._startsInSegment &&
           _pageIndex == other._pageIndex;
  }

  DCL_ALWAYS_INLINE
  bool operator!=(
    ChainedFixupsPageInfoIterator<Target, ByteOrder, Format> other) const {
    return !(*this == other);
  }

  DCL_ALWAYS_INLINE
  ChainedPointerIterator<Format> operator*() {
    return const_cast<ChainedPointerIterator<Format>>(
      std::as_const(*this).operator*());
  }

  DCL_ALWAYS_INLINE
  const ChainedPointerIterator<Format> operator*() const {
    uintptr_t address = _machHeader->getBase() +
                        _startsInSegment->getSegmentOffset() +
                        _startsInSegment->getPageSize() * _pageIndex +
                        _startsInSegment->getPageStartAtIndex(_pageIndex);
    return ChainedPointerIterator{reinterpret_cast<Format *>(address)};
  }
};

template <typename Target, typename ByteOrder, typename Format>
class ChainedFixupsPageInfoCollection {

private:
  MachHeader<Target, ByteOrder> * _machHeader;

  ChainedStartsInSegment<Target, ByteOrder> * _startsInSegment;

public:
  DCL_ALWAYS_INLINE
  ChainedFixupsPageInfoCollection(
    MachHeader<Target, ByteOrder> * machHeader,
    ChainedStartsInSegment<Target, ByteOrder> * startsInSegment)
    : _machHeader(machHeader), _startsInSegment(startsInSegment) {}

  using Iterator = ChainedFixupsPageInfoIterator<Target, ByteOrder, Format>;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() {
    return const_cast<Iterator>(std::as_const(*this).begin());
  }

  DCL_ALWAYS_INLINE
  Iterator end() { return const_cast<Iterator>(std::as_const(*this).end()); }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const {
    return Iterator{_machHeader, _startsInSegment, 0};
  }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const {
    return Iterator{
      _machHeader, _startsInSegment, _startsInSegment->SegmentCount()};
  }
};

#pragma mark - Chaiend-Fixups Segment Info Collection

template <typename Target, typename ByteOrder, typename Format>
class ChainedFixupsPageInfoCollection;

template <typename Target, typename ByteOrder>
class ChainedFixupsSegmentInfo {

private:
  MachHeader<Target, ByteOrder> * _machHeader;

  ChainedStartsInSegment<Target, ByteOrder> * _startsInSegment;

public:
  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfo(
    MachHeader<Target, ByteOrder> * machHeader,
    ChainedStartsInSegment<Target, ByteOrder> * startsInSegment)
    : _machHeader(machHeader), _startsInSegment(startsInSegment) {}

  DCL_ALWAYS_INLINE
  MachHeader<Target, ByteOrder> * getMachHeader() { return _machHeader; }

  DCL_ALWAYS_INLINE
  const MachHeader<Target, ByteOrder> * const getMachHeader() const {
    return _machHeader;
  }

  DCL_ALWAYS_INLINE
  ChainedStartsInSegment<Target, ByteOrder> * getStartsInSegment() {
    return _startsInSegment;
  }

  DCL_ALWAYS_INLINE
  const ChainedStartsInSegment<Target, ByteOrder> * getStartsInSegment() const {
    return _startsInSegment;
  }
};

template <typename Target, typename ByteOrder>
class ChainedFixupsSegmentInfoIterator {

private:
  MachHeader<Target, ByteOrder> * _machHeader;

  ChainedStartsInImage<Target, ByteOrder> * _startsInImage;

  size_t _index;

public:
  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfoIterator(
    MachHeader<Target, ByteOrder> * machHeader,
    ChainedStartsInImage<Target, ByteOrder> * startsInImage,
    size_t index)
    : _startsInImage(startsInImage), _machHeader(machHeader), _index(index) {}

  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfoIterator<Target, ByteOrder>& operator++() {
    _index++;
    return *this;
  }

  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfoIterator<Target, ByteOrder> operator++(int) {
    ChainedFixupsSegmentInfoIterator<Target, ByteOrder> iterator = *this;
    ++(*this);
    return iterator;
  }

  DCL_ALWAYS_INLINE
  bool
  operator==(ChainedFixupsSegmentInfoIterator<Target, ByteOrder> other) const {
    return _machHeader == other.machHeader &&
           _startsInImage == other._startsInImage && _index == other._index;
  }

  DCL_ALWAYS_INLINE
  bool
  operator!=(ChainedFixupsSegmentInfoIterator<Target, ByteOrder> other) const {
    return !(*this == other);
  }

  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfo<Target, ByteOrder> operator*() {
    return const_cast<ChainedFixupsSegmentInfo<Target, ByteOrder>>(
      std::as_const(*this).operator*());
  }

  DCL_ALWAYS_INLINE
  const ChainedFixupsSegmentInfo<Target, ByteOrder> operator*() const {
    return ChainedFixupsSegmentInfo<Target, ByteOrder>{
      _machHeader,
      reinterpret_cast<ChainedStartsInSegment<Target, ByteOrder> *>(
        _startsInImage->getBase() +
        _startsInImage->getSegmentInfoOffsetAtIndex(_index))};
  }
};

template <typename Target, typename ByteOrder>
class ChainedFixupsSegmentInfoCollection {

private:
  MachHeader<Target, ByteOrder> * _machHeader;

  ChainedStartsInImage<Target, ByteOrder> * _startsInImage;

public:
  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfoCollection(
    MachHeader<Target, ByteOrder> * machHeader,
    ChainedFixupsHeader<Target, ByteOrder> * chainedFixupsHeader)
    : _machHeader(machHeader),
      _startsInImage(chainedFixupsHeader->getStarts()) {}

  DCL_ALWAYS_INLINE
  ChainedFixupsSegmentInfoCollection(
    const MachHeader<Target, ByteOrder> * machHeader,
    const ChainedFixupsHeader<Target, ByteOrder> * chainedFixupsHeader)
    : _machHeader(const_cast<MachHeader<Target, ByteOrder> *>(machHeader)),
      _startsInImage(chainedFixupsHeader->getStarts()) {}

  using Iterator = ChainedFixupsSegmentInfoIterator<Target, ByteOrder>;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() {
    return const_cast<Iterator>(std::as_const(*this).begin());
  }

  DCL_ALWAYS_INLINE
  Iterator end() { return const_cast<Iterator>(std::as_const(*this).end()); }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const {
    return Iterator{_machHeader, _startsInImage, 0};
  }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const {
    return Iterator{
      _machHeader, _startsInImage, _startsInImage->SegmentCount()};
  }
};

} // namespace dcl::Binary::Darwin::Dyld

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_DYLD_DYLDFIXUPCHAINS_H
