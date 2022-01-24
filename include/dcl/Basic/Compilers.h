//===--- Compilers.h - Configurations for Variant Compilers -----*- C++ -*-===//
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
//===----------------------------------------------------------------------===//`

#ifndef DCL_BASIC_COMPILERS_H
#define DCL_BASIC_COMPILERS_H

#if defined(_MSC_VER) && !defined(__clang__)
#define DCL_COMPILER_IS_MSVC 1
#else
#define DCL_COMPILER_IS_MSVC 0
#endif

// Workaround non-clang compilers
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if __has_builtin(__builtin_assume)
#define DCL_ASSUME(x) __builtin_assume(x)
#else
#define DCL_ASSUME(x)
#endif

#if __has_attribute(constructor)
#define DCL_CONSTRUCTOR __attribute__((constructor))
#else
#define DCL_CONSTRUCTOR
#endif

/// \macro DCL_GNUC_PREREQ
/// Extend the default __GNUC_PREREQ even if glibc's features.h isn't
/// available.
#ifndef DCL_GNUC_PREREQ
# if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#  define DCL_GNUC_PREREQ(maj, min, patch) \
    ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) + __GNUC_PATCHLEVEL__ >= \
     ((maj) << 20) + ((min) << 10) + (patch))
# elif defined(__GNUC__) && defined(__GNUC_MINOR__)
#  define DCL_GNUC_PREREQ(maj, min, patch) \
    ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) >= ((maj) << 20) + ((min) << 10))
# else
#  define DCL_GNUC_PREREQ(maj, min, patch) 0
# endif
#endif

/// DCL_NOINLINE - On compilers where we have a directive to do so, mark a 
/// method "not for inlining".
#if __has_attribute(noinline) || DCL_GNUC_PREREQ(3, 4, 0)
#define DCL_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define DCL_NOINLINE __declspec(noinline)
#else
#define DCL_NOINLINE
#endif

/// DCL_ALWAYS_INLINE - On compilers where we have a directive to do so, mark a
/// method "always inline" because it is performance sensitive. GCC 3.4 
/// supported this but is buggy in various cases and produces unimplemented
/// errors, just use it in GCC 4.0 and later.
#if __has_attribute(always_inline) || DCL_GNUC_PREREQ(4, 0, 0)
#define DCL_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define DCL_ALWAYS_INLINE __forceinline
#else
#define DCL_ALWAYS_INLINE
#endif

#ifdef __GNUC__
#define DCL_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define DCL_NORETURN __declspec(noreturn)
#else
#define DCL_NORETURN
#endif

// Conditionally exclude declarations or statements that are only needed for
// assertions from release builds (NDEBUG) without cluttering the surrounding
// code by #ifdefs.
//
// struct DoThings  {
//   DCL_ASSERT_ONLY_DECL(unsigned verifyCount = 0);
//   DoThings() {
//     DCL_ASSERT_ONLY(verifyCount = getNumberOfThingsToDo());
//   }
//   void doThings() {
//     do {
//       // ... do each thing
//       DCL_ASSERT_ONLY(--verifyCount);
//     } while (!done());
//     assert(verifyCount == 0 && "did not do everything");
//   }
// };
#ifdef NDEBUG
#define DCL_ASSERT_ONLY_DECL(...)
#define DCL_ASSERT_ONLY(...) do { } while (false)
#else
#define DCL_ASSERT_ONLY_DECL(...) __VA_ARGS__
#define DCL_ASSERT_ONLY(...) do { __VA_ARGS__; } while (false)
#endif

#if defined(__LP64__) || defined(_WIN64)
#define DCL_POINTER_IS_8_BYTES 1
#define DCL_POINTER_IS_4_BYTES 0
#else
// TODO: consider supporting 16-bit targets
#define DCL_POINTER_IS_8_BYTES 0
#define DCL_POINTER_IS_4_BYTES 1
#endif

// Produce a string literal for the raw argument tokens.
#define DCL_STRINGIFY_RAW(TOK) #TOK

// Produce a string literal for the macro-expanded argument tokens.
#define DCL_STRINGIFY_EXPANDED(TOK) DCL_STRINGIFY_RAW(TOK)

#if defined(__USER_LABEL_PREFIX__)
#define DCL_SYMBOL_PREFIX_STRING \
  DCL_STRINGIFY_EXPANDED(__USER_LABEL_PREFIX__)
#else
// Clang and GCC always define __USER_LABEL_PREFIX__, so this should
// only come up with MSVC, and Windows doesn't use a prefix.
#define DCL_SYMBOL_PREFIX_STRING ""
#endif

// An attribute to override the symbol name of a declaration.
// This does not compensate for platform symbol prefixes; for that,
// use DCL_ASM_LABEL_WITH_PREFIX.
//
// This only actually works on Clang or GCC; MSVC does not provide
// an attribute to change the asm label.
#define DCL_ASM_LABEL_RAW(STRING) __asm__(STRING)
#define DCL_ASM_LABEL_WITH_PREFIX(STRING) \
  DCL_ASM_LABEL_RAW(DCL_SYMBOL_PREFIX_STRING STRING)
  
#endif // DCL_BASIC_COMPILERS_H
