//===--- Collections.h - Convenient Collections for MachO -------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_COLLECTIONS_H
#define DCL_BINARY_DARWIN_COLLECTIONS_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <dcl/Binary/Darwin/Iterators.h>
#include <dcl/Binary/Darwin/Traits.h>

namespace dcl::Binary::Darwin {

template <typename Target, typename ByteOrder>
class MachHeader;

template <typename Target, typename ByteOrder>
class LoadCommand;

template <typename Target, typename ByteOrder>
class LoadCommandIterator;

template <typename Target, typename ByteOrder>
class LoadCommandCollection {

private:
  MachHeader<Target, ByteOrder> * _header;

public:
  LoadCommandCollection(MachHeader<Target, ByteOrder> * header)
    : _header(header) {}

  using Iterator = LoadCommandIterator<Target, ByteOrder>;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() { return std::as_const(*this).begin(); }

  DCL_ALWAYS_INLINE
  Iterator end() { return std::as_const(*this).end(); }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() const {
    auto base = reinterpret_cast<LoadCommand<Target, ByteOrder> *>(
      (uintptr_t)_header + sizeof(MachHeader<Target, ByteOrder>));
    return LoadCommandIterator(base);
  }

  DCL_ALWAYS_INLINE
  ConstIterator cend() const {
    auto base = reinterpret_cast<LoadCommand<Target, ByteOrder> *>(
      (uintptr_t)cbegin().get() + _header->getSizeOfCommands());
    return LoadCommandIterator(base);
  }
};

template <typename Target, typename ByteOrder>
class SegmentCommand;

template <typename Target, typename ByteOrder>
class SectionIterator;

template <typename Target, typename ByteOrder>
class Section;

template <typename Target, typename ByteOrder>
class SectionCollection {

private:
  SegmentCommand<Target, ByteOrder> * _command;

public:
  SectionCollection(SegmentCommand<Target, ByteOrder> * command)
    : _command(command) {}

  using Iterator = SectionIterator<Target, ByteOrder>;
  using ConstIterator = typename std::add_const<Iterator>::type;

  DCL_ALWAYS_INLINE
  Iterator begin() { return Iterator{std::as_const(*this).begin()}; }

  DCL_ALWAYS_INLINE
  Iterator end() { return Iterator{std::as_const(*this).end()}; }

  DCL_ALWAYS_INLINE
  const Iterator begin() const { return cbegin(); }

  DCL_ALWAYS_INLINE
  const Iterator end() const { return cend(); }

  DCL_ALWAYS_INLINE
  ConstIterator cbegin() {
    auto sectionPtr = reinterpret_cast<Section<Target, ByteOrder> *>(
      (uintptr_t)_command + sizeof(SegmentCommand<Target, ByteOrder>));
    return Iterator{*sectionPtr};
  }

  DCL_ALWAYS_INLINE
  ConstIterator cend() {
    auto sectionPtr = reinterpret_cast<Section<Target, ByteOrder> *>(
      (uintptr_t)_command + sizeof(SegmentCommand<Target, ByteOrder>));
    return Iterator{*(sectionPtr + _command->getSectionCount())};
  }
};

} // namespace dcl::Binary::Darwin

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_COLLECTIONS_H
