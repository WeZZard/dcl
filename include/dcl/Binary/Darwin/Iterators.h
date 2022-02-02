//===--- Iterators.h - Darwin Binary Iterators ------------------*- C++ -*-===//
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

#ifndef DCL_BINARY_DARWIN_ITERATORS_H
#define DCL_BINARY_DARWIN_ITERATORS_H

#include <dcl/Basic/Basic.h>

#if DCL_TARGET_OS_DARWIN

#include <dcl/Binary/Darwin/Traits.h>

#include <utility>

namespace dcl {

namespace Binary {

namespace Darwin {

#pragma mark - Abstract

template <typename Derived>
class ForwardIterator {

protected:
  DCL_ALWAYS_INLINE
  Derived& asDerived() { return *static_cast<Derived *>(this); }

  DCL_ALWAYS_INLINE
  const Derived& asDerived() const {
    return *static_cast<const Derived *>(this);
  }

public:
  using value_type = typename IteratorTraits<Derived>::ValueTy;
  using difference_type = typename IteratorTraits<Derived>::DifferenceTy;
  using pointer = typename IteratorTraits<Derived>::PointerTy;
  using const_pointer = typename IteratorTraits<Derived>::ConstPointerTy;
  using reference = typename IteratorTraits<Derived>::ReferenceTy;
  using const_reference = typename IteratorTraits<Derived>::ConstReferenceTy;
  using iterator_category = typename IteratorTraits<Derived>::CategoryTy;

  DCL_ALWAYS_INLINE
  Derived advanced(difference_type distance) {
    Derived iterator = asDerived();
    iterator.advance(distance);
    return iterator;
  }

  DCL_ALWAYS_INLINE
  Derived& operator++() {
    asDerived().advance(1);
    return asDerived();
  }

  DCL_ALWAYS_INLINE
  Derived operator++(int) { return advanced(1); }

  DCL_ALWAYS_INLINE
  bool operator==(Derived other) const {
    return this->asDerived().isEqual(other);
  }

  DCL_ALWAYS_INLINE
  bool operator!=(Derived other) const { return !(this->asDerived() == other); }

  DCL_ALWAYS_INLINE
  reference operator*() { return *(operator->()); }

  DCL_ALWAYS_INLINE
  const_reference operator*() const { return *(operator->()); }

  DCL_ALWAYS_INLINE
  pointer operator->() {
    return const_cast<pointer>(std::as_const(asDerived()).get());
  }

  DCL_ALWAYS_INLINE
  const_pointer operator->() const { return asDerived()->get(); }
};

template <typename Derived>
class RandomAccessIterator : ForwardIterator<Derived> {

public:
  using value_type = typename IteratorTraits<Derived>::ValueTy;
  using difference_type = typename IteratorTraits<Derived>::DifferenceTy;
  using pointer = typename IteratorTraits<Derived>::PointerTy;
  using const_pointer = typename IteratorTraits<Derived>::ConstPointerTy;
  using reference = typename IteratorTraits<Derived>::ReferenceTy;
  using const_reference = typename IteratorTraits<Derived>::ConstReferenceTy;
  using iterator_category = typename IteratorTraits<Derived>::CategoryTy;

  DCL_ALWAYS_INLINE
  Derived& operator--() {
    this->asDerived().advance(-1);
    return this->asDerived();
  }

  DCL_ALWAYS_INLINE
  Derived operator--(int) {
    Derived iterator = this->asDerived();
    iterator.asDerived();
    return iterator;
  }

  DCL_ALWAYS_INLINE
  difference_type operator-(const Derived& rhs) const {
    return this->asDerived().distance(rhs);
  }

  DCL_ALWAYS_INLINE
  Derived operator+(difference_type rhs) const {
    return this->asDerived().advance(rhs);
  }

  DCL_ALWAYS_INLINE
  Derived operator-(difference_type rhs) const {
    return this->asDerived().advance(rhs);
  }
  DCL_ALWAYS_INLINE
  bool operator>(const Derived& rhs) const {
    return !this->asDerived()->lessThan(rhs) &&
           !this->asDerived()->isEuqal(rhs);
  }

  DCL_ALWAYS_INLINE
  bool operator<(const Derived& rhs) const {
    return this->asDerived()->lessThan(rhs);
  }

  DCL_ALWAYS_INLINE
  bool operator>=(const Derived& rhs) const {
    return this > rhs || this == rhs;
  }

  DCL_ALWAYS_INLINE
  bool operator<=(const Derived& rhs) const {
    return this < rhs || this == rhs;
  }

  DCL_ALWAYS_INLINE
  friend Derived operator+(difference_type lhs, const Derived& rhs) {
    return rhs.advanced(lhs);
  }

  DCL_ALWAYS_INLINE
  friend Derived operator-(difference_type lhs, const Derived& rhs) {
    return rhs.advanced(lhs);
  }
};

#pragma mark - Concrete

template <typename Target, typename ByteOrder>
class LoadCommand;

template <typename Target, typename ByteOrder>
class LoadCommandIterator
  : public ForwardIterator<LoadCommandIterator<Target, ByteOrder>> {

private:
  LoadCommand<Target, ByteOrder> * _base;

public:
  using difference_type = typename ForwardIterator<
    LoadCommandIterator<Target, ByteOrder>>::difference_type;

  DCL_ALWAYS_INLINE
  LoadCommandIterator(LoadCommand<Target, ByteOrder> * base) : _base(base) {}

  DCL_ALWAYS_INLINE
  LoadCommand<Target, ByteOrder> * get() { return _base; }

  DCL_ALWAYS_INLINE
  LoadCommand<Target, ByteOrder> * const get() const { return _base; }

  DCL_ALWAYS_INLINE
  void advance(difference_type distance) {
    DCLAssert(
      distance > 0 && "LoadCommandIterator is a forward-only iterator.");
    while (distance) {
      auto loadCommand = get();
      _base = const_cast<LoadCommand<Target, ByteOrder> *>(
        reinterpret_cast<const LoadCommand<Target, ByteOrder> *>(
          loadCommand->getBase() + loadCommand->getCommandSize()));
    }
    distance--;
  }

  DCL_ALWAYS_INLINE
  bool isEqual(const LoadCommandIterator<Target, ByteOrder>& other) const {
    return get() == other.get();
  }
};

template <typename Target, typename ByteOrder>
class Section;

template <typename Target, typename ByteOrder>
class SectionIterator
  : public RandomAccessIterator<SectionIterator<Target, ByteOrder>> {

private:
  Section<Target, ByteOrder> * _section;

public:
  using difference_type = typename RandomAccessIterator<
    LoadCommandIterator<Target, ByteOrder>>::difference_type;

  DCL_ALWAYS_INLINE
  SectionIterator(Section<Target, ByteOrder> * section) : _section(section) {}

  DCL_ALWAYS_INLINE
  Section<Target, ByteOrder> * get() { return _section; }

  DCL_ALWAYS_INLINE
  Section<Target, ByteOrder> * const get() const { return _section; }

  DCL_ALWAYS_INLINE
  void advance(difference_type distance) { _section = get() + distance; }

  DCL_ALWAYS_INLINE
  bool isEqual(const SectionIterator<Target, ByteOrder>& other) const {
    return get() == other.get();
  }

  DCL_ALWAYS_INLINE
  difference_type
  distance(const SectionIterator<Target, ByteOrder>& other) const {
    return get() - other.get();
  }

  DCL_ALWAYS_INLINE
  bool lessThan(const SectionIterator<Target, ByteOrder>& other) const {
    return get() < other.get();
  }
};

} // namespace Darwin

} // namespace Binary

} // namespace dcl

#endif // DCL_TARGET_OS_DARWIN

#endif // DCL_BINARY_DARWIN_ITERATORS_H
