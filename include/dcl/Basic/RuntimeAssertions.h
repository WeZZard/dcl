//===--- RuntimeAssertions.h - Runtime Assertion Utilieise ------*- C++ -*-===//
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

#ifndef DCL_BASIC_RUNTIME_ASSERTIONS_H
#define DCL_BASIC_RUNTIME_ASSERTIONS_H

#include <dcl/Basic/Compilers.h>

#include <cstdarg>

#pragma mark - Interfaces

/**
 * @brief The assert macro in C standard library occupies the identifier 
 * `assert`. Thus we have the following macro and then foward to dcl::_assert.
 * 
 */
#define DCLAssert(...) dcl::_assert(__VA_ARGS__)

namespace dcl {

/**
 * @brief The assert macro in C standard library occupies the identifier 
 * `assert`. Thus this function is named with `_assert`.
 * 
 */
DCL_ALWAYS_INLINE
void _assert(bool predicate, const char *__restrict format, ...)
    DCL_PRINTF_LIKE(2, 3);

DCL_ALWAYS_INLINE
void _assert(bool predicate);

void precondition(bool predicate, const char *__restrict format, ...)
    DCL_PRINTF_LIKE(2, 3);

DCL_NORETURN
void preconditionFailure(const char *__restrict format, ...)
    DCL_PRINTF_LIKE(1, 2);

DCL_NORETURN
void unreachable(const char *__restrict format, ...) DCL_PRINTF_LIKE(1, 2);

DCL_NORETURN
void unreachable();

DCL_NORETURN
void notImplemented();

} // namespace dcl

#endif // DCL_BASIC_RUNTIME_ASSERTIONS_H