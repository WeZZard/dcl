//===--- File.h - File Abstraction Over Variant OSes ------------*- C++ -*-===//
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
//
//  This file define basic interface of the file abstraction.
//
//===----------------------------------------------------------------------===//

#ifndef DCL_OS_FILE_H
#define DCL_OS_FILE_H

#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <memory>

#include <dcl/Basic/Basic.h>

namespace dcl {

namespace OS {

/**
 * @brief `File` is a move-only resource.
 * 
 */
class File {

private:

  int _fd;

  void * _buffer;

  size_t _size;

#if DEBUG
  char _path[PATH_MAX + 1];
#endif

public:

  explicit File(const char * path, uint32_t flags) noexcept;

  ~File() noexcept;

  File(const File&) = delete;

  File& operator = (const File&) = delete;

  DCL_ALWAYS_INLINE
  File(File&& another) noexcept {
    * this = std::move(another);
  }

  DCL_ALWAYS_INLINE
  File& operator = (File&& another) noexcept {
    _buffer = another._buffer;
    another._buffer = nullptr;
    _size = another._size;
    another._size = 0;
    _fd = another._fd;
    another._fd = -1;
#if DEBUG
    std::memset((void *)_path, 0, PATH_MAX + 1);
    std::memcpy((void *)_path, another._path, strlen(another._path));
    std::memset((void *)another._path, 0, PATH_MAX + 1);
#endif
    return *this;
  }

#pragma mark - Accessing File Contents

public:

  DCL_ALWAYS_INLINE
  void * getBytes() noexcept {
    return _buffer;
  }

  DCL_ALWAYS_INLINE
  const void * getBytes() const noexcept {
    return _buffer;
  }

  DCL_ALWAYS_INLINE
  size_t getSize() noexcept {
    return _size;
  }

  DCL_ALWAYS_INLINE
  size_t getSize() const noexcept {
    return _size;
  }

  DCL_ALWAYS_INLINE
  int getFd() noexcept {
    return _fd;
  }

  DCL_ALWAYS_INLINE
  int getFd() const noexcept {
    return _fd;
  }

};

} // namespace dcl::OS

} // namespace dcl

#endif // DCL_OS_FILE_H
