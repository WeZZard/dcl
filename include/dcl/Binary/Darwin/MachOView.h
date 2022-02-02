//===--- MachOView.h - A Universal Interface for MachO & Fat ----*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_MACHOVIEW_H
#define DCL_BINARY_DARWIN_MACHOVIEW_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <dcl/Binary/Darwin/Fat.h>
#include <dcl/Binary/Darwin/Format.h>
#include <dcl/Binary/Darwin/MachO.h>
#include <dcl/Platform/TypeWrapper.h>

#include <cstdint>
#include <iterator>
#include <utility>

namespace dcl::Binary::Darwin {

namespace details {
/** concept `File`

 */
template <class Word>
class File {};
template <>
class File<uint32_t> {

public:
  using WordTy = uint32_t;

  static constexpr uint8_t wordSize = sizeof(WordTy);

  using PointerValueTy = uint32_t;

  using RelativePointerValueTy = uint16_t;

  using FatHeaderTy = fat_header;

  using FatArchTy = fat_arch;
};
template <>
class File<uint64_t> {

public:
  using WordTy = uint64_t;

  static constexpr uint8_t wordSize = sizeof(WordTy);

  using PointerValueTy = uint64_t;

  using RelativePointerValueTy = uint32_t;

  using FatHeaderTy = fat_header;

  using FatArchTy = fat_arch_64;
};

} // namespace details

#pragma mark - MachO

template <typename Target, typename Endianess>
class MachO {

private:
  MachHeader<Target, Endianess> _header_;

public:
  MachO(const MachO&) = delete;

  MachO(MachO&&) = delete;

  using PointerValueTy = typename Target::PointerValueTy;

  using CommandKind = typename Target::LoadCommandKindTy;

  DCL_ALWAYS_INLINE
  Format getFormat() const { return GetFormatWithBytes<MachOMagic>(&_header_); }

  DCL_ALWAYS_INLINE
  MachHeader<Target, Endianess> * getHeader() {
    return const_cast<MachHeader<Target, Endianess> *>(
      std::as_const(*this).getHeader());
  }

  DCL_ALWAYS_INLINE
  const MachHeader<Target, Endianess> * getHeader() const { return &_header_; }
};

#pragma mark - Fat

template <typename Target, typename Endianess>
class Fat {

private:
  void * _header;

  DCL_ALWAYS_INLINE
  uint8_t * getBase() { return reinterpret_cast<uint8_t *>(_header); }

  DCL_ALWAYS_INLINE
  const uint8_t * getBase() const {
    return reinterpret_cast<const uint8_t *>(_header);
  }

public:
  DCL_ALWAYS_INLINE
  explicit Fat(void * header) : _header(header) {}

  DCL_ALWAYS_INLINE
  FatHeader<Target, Endianess> * getHeader() const {
    return reinterpret_cast<FatHeader<Target, Endianess> *>(_header);
  }

  // MARK: FatHeader

  DCL_ALWAYS_INLINE
  const uint32_t getMagic() const { return getHeader()->getMagic(); }

  DCL_ALWAYS_INLINE
  const uint32_t getArchCount() const { return getHeader()->getArchCount(); }

  // MARK: Accessing Fat Arch Collection

  DCL_ALWAYS_INLINE
  FatArchCollection<Target, Endianess>& getArchs() {
    return *reinterpret_cast<FatArchCollection<Target, Endianess> *>(
      getHeader());
  }

  DCL_ALWAYS_INLINE
  const FatArchCollection<Target, Endianess>& getArchs() const {
    return *reinterpret_cast<const FatArchCollection<Target, Endianess> *>(
      getHeader());
  }

  // MARK: Accessing Mach Header

  DCL_ALWAYS_INLINE
  void * getMachHeaderAt(uint32_t index) {
    return reinterpret_cast<void *>(
      getBase() + getArchs().at(index).getOffset());
  }

  DCL_ALWAYS_INLINE
  const void * getMachHeaderAt(uint32_t index) const {
    return reinterpret_cast<const void *>(
      getBase() + getArchs().at(index).getOffset());
  }

  template <typename T, typename E>
  DCL_ALWAYS_INLINE
  const MachO<T, E> * getMachHeaderAt(uint32_t index) const {
    const void * machOHeader = getMachHeaderAt(index);
    switch (GetFormatWithBytes<MachOMagic>(getMachHeaderAt(index))) {
    case Format::LittleEndianess64Bit:
      if (
        T::wordSize == sizeof(uint64_t) &&
        std::is_same<E, Platform::LittleEndianess>()) {
        return reinterpret_cast<const MachO<T, E> *>(machOHeader);
      }
      break;
    case Format::LittleEndianess32Bit:
      if (
        T::wordSize == sizeof(uint32_t) &&
        std::is_same<E, Platform::LittleEndianess>()) {
        return reinterpret_cast<const MachO<T, E> *>(machOHeader);
      }
      break;
    case Format::BigEndianess64Bit:
      if (
        T::wordSize == sizeof(uint64_t) &&
        std::is_same<E, Platform::BigEndianess>()) {
        return reinterpret_cast<const MachO<T, E> *>(machOHeader);
      }
      break;
    case Format::BigEndianess32Bit:
      if (
        T::wordSize == sizeof(uint32_t) &&
        std::is_same<E, Platform::BigEndianess>()) {
        return reinterpret_cast<const MachO<T, E> *>(machOHeader);
      }
      break;
    case Format::Unknown:
      return nullptr;
    }
    return nullptr;
  }

  DCL_ALWAYS_INLINE
  Format getMachOFormatAt(uint32_t index) {
    return GetFormatWithBytes<MachOMagic>(getMachHeaderAt(index));
  }

  DCL_ALWAYS_INLINE
  Format getMachOFormatAt(uint32_t index) const {
    return GetFormatWithBytes<MachOMagic>(getMachHeaderAt(index));
  }
};

#pragma mark - MachOView

class MachOView {

public:
  class Slice;

private:
  enum class SliceKind : uint8_t {
    Fat,
    MachO,
  };

private:
  struct IteratorFat {

  private:
    void * _header;

    Format _format;

    uint32_t _index;

    mutable void * _machO;

  private:
    DCL_ALWAYS_INLINE
    IteratorFat(void * header, uint32_t index)
      : IteratorFat(
          header,
          GetFormatWithBytes<FatMagic>(header),
          index,
          nullptr) {}

    DCL_ALWAYS_INLINE
    IteratorFat(void * header, Format format, uint32_t index, void * machO)
      : _header(header), _format(format), _index(index), _machO(machO) {}

  public:
    DCL_ALWAYS_INLINE
    explicit IteratorFat(void * header) : IteratorFat(header, 0) {}

  public:
    DCL_ALWAYS_INLINE
    IteratorFat& operator++() {
      _index++;
      return *this;
    }

    DCL_ALWAYS_INLINE
    IteratorFat operator++(int) {
      IteratorFat iterator = *this;
      ++(*this);
      return iterator;
    }

    DCL_ALWAYS_INLINE
    bool operator==(IteratorFat other) const {
      return _header == other._header && _index == other._index;
    }

    DCL_ALWAYS_INLINE
    bool operator!=(IteratorFat other) const { return !(*this == other); }

    DCL_ALWAYS_INLINE
    Slice operator*() const { return Slice(*this); }

  private:
    DCL_ALWAYS_INLINE
    Format getFatFormat() const { return _format; }

  public:
    DCL_ALWAYS_INLINE
    Format getMachOFormat() const {
      switch (getFatFormat()) {
      case Format::LittleEndianess64Bit: {
        auto fat =
          Fat<details::File<uint64_t>, Platform::LittleEndianess>(_header);
        return fat.getMachOFormatAt(_index);
      }
      case Format::LittleEndianess32Bit: {
        auto fat =
          Fat<details::File<uint32_t>, Platform::BigEndianess>(_header);
        return fat.getMachOFormatAt(_index);
      }
      case Format::BigEndianess64Bit: {
        auto fat =
          Fat<details::File<uint64_t>, Platform::LittleEndianess>(_header);
        return fat.getMachOFormatAt(_index);
      }
      case Format::BigEndianess32Bit: {
        auto fat =
          Fat<details::File<uint32_t>, Platform::BigEndianess>(_header);
        return fat.getMachOFormatAt(_index);
      }
      case Format::Unknown:
        return Format::Unknown;
      }
    }

    template <typename Target, typename Endianess>
    DCL_ALWAYS_INLINE
    MachO<Target, Endianess> * getMachO() {
      return const_cast<MachO<Target, Endianess> *>(
        std::as_const(*this).getMachO<Target, Endianess>());
    }

    template <typename Target, typename Endianess>
    DCL_ALWAYS_INLINE
    const MachO<Target, Endianess> * getMachO() const {
      switch (getFatFormat()) {
      case Format::LittleEndianess64Bit: {
        auto fat =
          Fat<details::File<uint64_t>, Platform::LittleEndianess>(_header);
        return fat.getMachHeaderAt<Target, Endianess>(_index);
      }
      case Format::LittleEndianess32Bit: {
        auto fat =
          Fat<details::File<uint32_t>, Platform::BigEndianess>(_header);
        return fat.getMachHeaderAt<Target, Endianess>(_index);
      }
      case Format::BigEndianess64Bit: {
        auto fat =
          Fat<details::File<uint64_t>, Platform::LittleEndianess>(_header);
        return fat.getMachHeaderAt<Target, Endianess>(_index);
      }
      case Format::BigEndianess32Bit: {
        auto fat =
          Fat<details::File<uint32_t>, Platform::BigEndianess>(_header);
        return fat.getMachHeaderAt<Target, Endianess>(_index);
      }
      default:
        return nullptr;
      }
    }

    DCL_ALWAYS_INLINE
    IteratorFat end() const {
      uint32_t archCount;
      switch (getFatFormat()) {
      case Format::LittleEndianess64Bit: {
        archCount =
          Fat<details::File<uint64_t>, Platform::LittleEndianess>(_header)
            .getArchCount();
        break;
      }
      case Format::LittleEndianess32Bit: {
        archCount =
          Fat<details::File<uint32_t>, Platform::BigEndianess>(_header)
            .getArchCount();
        break;
      }
      case Format::BigEndianess64Bit: {
        archCount =
          Fat<details::File<uint64_t>, Platform::LittleEndianess>(_header)
            .getArchCount();
        break;
      }
      case Format::BigEndianess32Bit: {
        archCount =
          Fat<details::File<uint32_t>, Platform::BigEndianess>(_header)
            .getArchCount();
        break;
      }
      case Format::Unknown:
        archCount = 0;
      }
      return IteratorFat(_header, _format, archCount, nullptr);
    }
  };

  struct IteratorMachO {

  private:
    void * _header;

    bool _reaches_end;

  private:
    DCL_ALWAYS_INLINE
    IteratorMachO(void * header, bool reachesEnd)
      : _header(header), _reaches_end(reachesEnd) {}

  public:
    DCL_ALWAYS_INLINE
    explicit IteratorMachO(void * header) : IteratorMachO(header, false) {}

  public:
    DCL_ALWAYS_INLINE
    IteratorMachO& operator++() {
      DCLAssert(!_reaches_end);
      _reaches_end = true;
      return *this;
    }

    DCL_ALWAYS_INLINE
    IteratorMachO operator++(int) {
      IteratorMachO iterator = *this;
      ++(*this);
      return iterator;
    }

    DCL_ALWAYS_INLINE
    bool operator==(IteratorMachO other) const {
      return _header == other._header && _reaches_end == other._reaches_end;
    }

    DCL_ALWAYS_INLINE
    bool operator!=(IteratorMachO other) const { return !(*this == other); }

    DCL_ALWAYS_INLINE
    Slice operator*() const { return Slice(*this); }

  public:
    DCL_ALWAYS_INLINE
    Format getMachOFormat() const {
      return GetFormatWithBytes<MachOMagic>(_header);
    }

    template <typename Target, typename Endianess>
    DCL_ALWAYS_INLINE
    MachO<Target, Endianess> * getMachO() {
      return const_cast<MachO<Target, Endianess> *>(
        std::as_const(*this).getMachO<Target, Endianess>());
    }

    template <typename Target, typename Endianess>
    DCL_ALWAYS_INLINE
    const MachO<Target, Endianess> * getMachO() const {
      Format format = GetFormatWithBytes<MachOMagic>(_header);
      switch (format) {
      case Format::LittleEndianess64Bit:
        if (
          Target::wordSize == sizeof(uint64_t) &&
          std::is_same<Endianess, Platform::LittleEndianess>()) {
          return reinterpret_cast<MachO<Target, Endianess> *>(_header);
        }
        break;
      case Format::LittleEndianess32Bit:
        if (
          Target::wordSize == sizeof(uint32_t) &&
          std::is_same<Endianess, Platform::LittleEndianess>()) {
          return reinterpret_cast<MachO<Target, Endianess> *>(_header);
        }
        break;
      case Format::BigEndianess64Bit:
        if (
          Target::wordSize == sizeof(uint64_t) &&
          std::is_same<Endianess, Platform::BigEndianess>()) {
          return reinterpret_cast<MachO<Target, Endianess> *>(_header);
        }
        break;
      case Format::BigEndianess32Bit:
        if (
          Target::wordSize == sizeof(uint32_t) &&
          std::is_same<Endianess, Platform::BigEndianess>()) {
          return reinterpret_cast<MachO<Target, Endianess> *>(_header);
        }
        break;
      case Format::Unknown:
        return nullptr;
      }
      return nullptr;
    }

    DCL_ALWAYS_INLINE
    IteratorMachO end() const { return IteratorMachO(_header, true); }
  };

  class VariantIterator {

  private:
    union {
      IteratorFat fat;
      IteratorMachO machO;
    };

    SliceKind kind;

  public:
    DCL_ALWAYS_INLINE
    explicit VariantIterator(void * header) {
      uint32_t magic = *reinterpret_cast<uint32_t *>(header);
      if (
        magic == MH_MAGIC || magic == MH_CIGAM || magic == MH_MAGIC_64 ||
        magic == MH_CIGAM_64) {
        machO = IteratorMachO(header);
        kind = SliceKind::MachO;
      } else if (
        magic == FAT_MAGIC || magic == FAT_CIGAM || magic == FAT_MAGIC_64 ||
        magic == FAT_CIGAM_64) {
        fat = IteratorFat(header);
        kind = SliceKind::Fat;
      } else {
        // Unrecognized magic;
        exit(1);
      }
    }

    DCL_ALWAYS_INLINE
    explicit VariantIterator(IteratorFat fat)
      : fat(fat), kind(SliceKind::Fat) {}

    DCL_ALWAYS_INLINE
    explicit VariantIterator(IteratorMachO machO)
      : machO(machO), kind(SliceKind::MachO) {}

    DCL_ALWAYS_INLINE
    VariantIterator end() const {
      switch (kind) {
      case SliceKind::Fat:
        return VariantIterator(fat.end());
      case SliceKind::MachO:
        return VariantIterator(machO.end());
      }
    }

    DCL_ALWAYS_INLINE
    VariantIterator& operator++() {
      switch (kind) {
      case SliceKind::Fat:
        fat.operator++();
        break;
      case SliceKind::MachO:
        machO.operator++();
        break;
      }
      return *this;
    }

    DCL_ALWAYS_INLINE
    VariantIterator operator++(int arg0) {
      VariantIterator iterator = *this;
      ++(*this);
      return iterator;
    }

    DCL_ALWAYS_INLINE
    bool operator==(VariantIterator other) const {
      switch (kind) {
      case SliceKind::Fat:
        switch (other.kind) {
        case SliceKind::Fat:
          return fat == other.fat;
        case SliceKind::MachO:
          return false;
        }
      case SliceKind::MachO:
        switch (other.kind) {
        case SliceKind::Fat:
          return false;
        case SliceKind::MachO:
          return machO == other.machO;
        }
      }
    }

    DCL_ALWAYS_INLINE
    bool operator!=(VariantIterator other) const { return !(*this == other); }

    DCL_ALWAYS_INLINE
    Slice operator*() const {
      switch (kind) {
      case SliceKind::Fat:
        return fat.operator*();
      case SliceKind::MachO:
        return machO.operator*();
      }
    }
  };

public:
  class Slice {

  private:
    union {
      IteratorFat _fat;
      IteratorMachO _machO;
    };

    SliceKind _kind;

  public:
    DCL_ALWAYS_INLINE
    explicit Slice(IteratorFat fat) : _fat(fat), _kind(SliceKind::Fat){};

    DCL_ALWAYS_INLINE
    explicit Slice(IteratorMachO machO)
      : _machO(machO), _kind(SliceKind::MachO){};

    DCL_ALWAYS_INLINE
    SliceKind getKind() const { return _kind; }

    DCL_ALWAYS_INLINE
    Format getMachOFormat() const {
      switch (_kind) {
      case SliceKind::Fat:
        return _fat.getMachOFormat();
      case SliceKind::MachO:
        return _machO.getMachOFormat();
      }
    }

    template <typename Target, typename Endianess>
    DCL_ALWAYS_INLINE
    MachO<Target, Endianess> * getMachO() {
      return const_cast<MachO<Target, Endianess> *>(
        std::as_const(*this).getMachO<Target, Endianess>());
    }

    template <typename Target, typename Endianess>
    DCL_ALWAYS_INLINE
    const MachO<Target, Endianess> * getMachO() const {
      switch (_kind) {
      case SliceKind::Fat:
        return _fat.getMachO<Target, Endianess>();
      case SliceKind::MachO:
        return _machO.getMachO<Target, Endianess>();
      }
    }
  };

  class Iterator : public std::iterator<std::forward_iterator_tag, Slice> {

  private:
    VariantIterator _variant;

    DCL_ALWAYS_INLINE
    explicit Iterator(VariantIterator variant) : _variant(variant) {}

  public:
    DCL_ALWAYS_INLINE
    explicit Iterator(void * header) : _variant(header) {}

  public:
    DCL_ALWAYS_INLINE
    Iterator& operator++() {
      _variant.operator++();
      return *this;
    }

    DCL_ALWAYS_INLINE
    Iterator operator++(int) { return Iterator{_variant.operator++()}; }

    DCL_ALWAYS_INLINE
    bool operator==(Iterator other) const {
      return _variant.operator==(other._variant);
    }

    DCL_ALWAYS_INLINE
    bool operator!=(Iterator other) const {
      return _variant.operator!=(other._variant);
    }

    DCL_ALWAYS_INLINE
    Slice operator*() const { return _variant.operator*(); }

    DCL_ALWAYS_INLINE
    Iterator end() const { return Iterator{_variant.end()}; }

    DCL_ALWAYS_INLINE
    static Iterator makeBegin(void * header) { return Iterator{header}; }

    DCL_ALWAYS_INLINE
    static Iterator makeEnd(void * header) { return Iterator{header}.end(); }
  };

private:
  void * _address;

public:
  DCL_ALWAYS_INLINE
  explicit MachOView(void * buffer) : _address(buffer) {}

#pragma mark - Accessing Raw Bytes

  DCL_ALWAYS_INLINE
  void * getBytes() {
    return const_cast<void *>(std::as_const(*this).getBytes());
  }

  DCL_ALWAYS_INLINE
  const void * getBytes() const { return _address; }

#pragma mark - Accessing Slices

  using value_type = Slice;
  using reference = Slice&;
  using Iterator = Iterator;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() { return Iterator::makeBegin(_address); }

  DCL_ALWAYS_INLINE
  Iterator end() { return Iterator::makeEnd(_address); }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const { return begin(); }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const { return end(); }

#pragma mark - Accessing MachO Slice

  template <typename Target, typename Endianess>
  DCL_ALWAYS_INLINE
  MachO<Target, Endianess> * getMachO() {
    return const_cast<MachO<Target, Endianess> *>(
      std::as_const(*this).getMachO<Target, Endianess>());
  }

  template <typename Target, typename Endianess>
  DCL_ALWAYS_INLINE
  const MachO<Target, Endianess> * getMachO() const {
    for (auto eachSlice : *this) {
      auto machO = eachSlice.getMachO<Target, Endianess>();
      if (machO) {
        return machO;
      }
    }
    return nullptr;
  }
};

} // namespace dcl::Binary::Darwin

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_MACHOVIEW_H
