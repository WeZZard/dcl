//===--- RuntimeAssertions.cpp - Runtime Assertion Utilieise ------*- C++ -*-===//
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

#include <dcl/Basic/RuntimeAssertions.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace dcl {

namespace {

DCL_ALWAYS_INLINE
void assertv(bool predicate, const char *__restrict format, va_list args) {
#if DEBUG
  if (!predicate) {
    vprintf(format, args);
    abort();
  }
#endif
}

DCL_ALWAYS_INLINE
void preconditionv(bool predicate, const char *__restrict format,
                   va_list args) {
  if (!predicate) {
    vprintf(format, args);
    abort();
  }
}

DCL_ALWAYS_INLINE
DCL_NORETURN
void preconditionFailurev(const char *__restrict format, va_list args) {
  vprintf(format, args);
  abort();
}

DCL_ALWAYS_INLINE
DCL_NORETURN
void unreachablev(const char *__restrict format, va_list args) {
  vprintf(format, args);
  abort();
}

} // namespace

void _assert(bool predicate, const char *__restrict format, ...) {
#if DEBUG
  va_list args;
  va_start(args, format);
  assertv(predicate, format, args);
  va_end(args);
#endif
}

void _assert(bool predicate) {
#if DEBUG
  _assert(predicate, "assertion failure.");
#endif
}

void precondition(bool predicate, const char *format, ...) {
  va_list args;
  va_start(args, format);
  preconditionv(predicate, format, args);
  va_end(args);
}

void preconditionFailure(const char *__restrict format, ...) {
  va_list args;
  va_start(args, format);
  preconditionFailurev(format, args);
  va_end(args);
}

void unreachable() { 
  unreachable("Unreachable branch.");
 }

void unreachable(const char *__restrict format, ...) {
  va_list args;
  va_start(args, format);
  unreachablev(format, args);
  va_end(args);
}

void notImplemented() {
  printf("not implemented.");
  abort();
}

} // namespace dcl
