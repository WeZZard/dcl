//===--- Traits.h - Darwin Traits -------------------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_TRAITS_H
#define DCL_BINARY_DARWIN_TRAITS_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <cstdint>
#include <mach-o/fixup-chains.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

namespace dcl {

namespace Binary {

namespace Darwin {

#pragma mark - Load Command Traits

template <typename Target, typename ByteOrder>
class LoadCommandTraitsBase {
public:
  using TargetTy = Target;
  using ByteOrderTy = ByteOrder;
};

template <typename Derived>
class LoadCommandTraits;

template <typename Target, typename ByteOrder>
class LoadCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<LoadCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::LoadCommandTy;
};

template <typename Target, typename ByteOrder>
class SegmentCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<SegmentCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::SegmentCommandTy;
};

template <typename Target, typename ByteOrder>
class DyldInfoCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<DyldInfoCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::DyldInfoCommandTy;
};

template <typename Target, typename ByteOrder>
class DylibCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<DylibCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::DylibCommandTy;
};

template <typename Target, typename ByteOrder>
class RPathCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<RPathCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::RPathCommandTy;
};

template <typename Target, typename ByteOrder>
class SymbolTableCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<SymbolTableCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::SymbolTableCommandTy;
};

template <typename Target, typename ByteOrder>
class LinkEditDataCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<LinkEditDataCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::LinkEditDataCommandTy;
};

template <typename Target, typename ByteOrder>
class ExportsTrieCommand;

template <typename Target, typename ByteOrder>
class LoadCommandTraits<ExportsTrieCommand<Target, ByteOrder>>
    : public LoadCommandTraitsBase<Target, ByteOrder> {
public:
  using CommandTy = typename Target::LinkEditDataCommandTy;
};

#pragma mark - Iterator Traits

template <typename IteratorTag>
class IteratorTraits;

template <typename Target, typename ByteOrder>
class LoadCommandIterator;

template <typename Target, typename ByteOrder>
class IteratorTraits<LoadCommandIterator<Target, ByteOrder>> {
public:
  using ValueTy = LoadCommand<Target, ByteOrder>;
  using DifferenceTy = ptrdiff_t;
  using PointerTy = typename std::add_pointer<ValueTy>::type;
  using ConstPointerTy = typename std::add_const<PointerTy>::type;
  using ReferenceTy = typename std::add_lvalue_reference<ValueTy>::type;
  using ConstReferenceTy = typename std::add_const<ReferenceTy>::type;
  using CategoryTy = std::forward_iterator_tag;
};

template <typename Target, typename ByteOrder>
class Section;

template <typename Target, typename ByteOrder>
class SectionIterator;

template <typename Target, typename ByteOrder>
class IteratorTraits<SectionIterator<Target, ByteOrder>> {
public:
  using ValueTy = Section<Target, ByteOrder>;
  using DifferenceTy = ptrdiff_t;
  using PointerTy = typename std::add_pointer<ValueTy>::type;
  using ConstPointerTy = typename std::add_const<PointerTy>::type;
  using ReferenceTy = typename std::add_lvalue_reference<ValueTy>::type;
  using ConstReferenceTy = typename std::add_const<ReferenceTy>::type;
  using CategoryTy = std::random_access_iterator_tag;
};

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_TRAITS_H
