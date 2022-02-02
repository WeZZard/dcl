//===--- File.h - File Abstraction over Variant OSes ------------*- C++ -*-===//
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

#ifndef DCL_IO_FILE_H
#define DCL_IO_FILE_H

#include <cstdint>
#include <cstdlib>
#include <memory>

#include <dcl/Basic/Basic.h>
#include <dcl/IO/Permissions.h>

namespace dcl {

namespace IO {

/// A move-only resource.
class File {

private:
  int _fd;

  void * _buffer;

  size_t _size;

#if DEBUG
  char _path[PATH_MAX + 1];
#endif

public:
  explicit File(const char * path, Permissions permissions) noexcept;

  ~File() noexcept;

  File(const File&) = delete;

  File& operator=(const File&) = delete;

  DCL_ALWAYS_INLINE
  File(File&& another) noexcept { *this = std::move(another); }

  DCL_ALWAYS_INLINE
  File& operator=(File&& another) noexcept {
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
  void * getBytes() noexcept { return _buffer; }

  DCL_ALWAYS_INLINE
  const void * getBytes() const noexcept { return _buffer; }

  DCL_ALWAYS_INLINE
  size_t getSize() noexcept { return _size; }

  DCL_ALWAYS_INLINE
  size_t getSize() const noexcept { return _size; }

  DCL_ALWAYS_INLINE
  int getFd() noexcept { return _fd; }

  DCL_ALWAYS_INLINE
  int getFd() const noexcept { return _fd; }

#if DEBUG
  DCL_ALWAYS_INLINE
  char * getPath() noexcept { return _path; }

  DCL_ALWAYS_INLINE
  const char * getPath() const noexcept { return _path; }
#endif
};

} // namespace IO

} // namespace dcl

#endif // DCL_IO_FILE_H
