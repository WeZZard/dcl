//===--- File.h - Decompiler Components Library -----------------*- C++ -*-===//
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

#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <memory>

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

  explicit File(const char * path, uint32_t flags);

  ~File();

  File(const File&) = delete;

  File& operator = (const File&) = delete;

  File(File&& another) {
    * this = std::move(another);
  }

  File& operator = (File&& another) {
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

  void * getBytes() {
    return _buffer;
  }

  const void * getBytes() const {
    return _buffer;
  }

  size_t getSize() {
    return _size;
  }

  size_t getSize() const {
    return _size;
  }

  int getFd() {
    return _fd;
  }

  int getFd() const {
    return _fd;
  }

};

} // namespace dcl::OS

} // namespace dcl
