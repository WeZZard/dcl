/**
 * Macros for metaprogramming
 * ExtendedC
 *
 * Copyright (C) 2012 Justin Spahr-Summers
 * Released under the MIT license
 */

#ifndef DCL_BASIC_META_MACROS_H
#define DCL_BASIC_META_MACROS_H

/**
 * Executes one or more expressions (which may have a void type, such as a call
 * to a function that returns no value) and always returns true.
 */
#define DCL_META_EXPRIFY(...) \
    ((__VA_ARGS__), true)

/**
 * Returns a string representation of VALUE after full macro expansion.
 */
#define DCL_META_STRINGIFY(VALUE) \
        DCL_META_STRINGIFY_(VALUE)

/**
 * Returns A and B concatenated after full macro expansion.
 */
#define DCL_META_CONCAT(A, B) \
        DCL_META_CONCAT_(A, B)

/**
 * Returns the Nth variadic argument (starting from zero). At least
 * N + 1 variadic arguments must be given. N must be between zero and twenty,
 * inclusive.
 */
#define DCL_META_AT(N, ...) \
        DCL_META_CONCAT(DCL_META_AT, N)(__VA_ARGS__)

/**
 * Returns the number of arguments (up to twenty) provided to the macro. At
 * least one argument must be provided.
 *
 * Inspired by P99: http://p99.gforge.inria.fr
 */
#define DCL_META_ARGCOUNT(...) \
        DCL_META_AT(20, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

/**
 * Identical to #DCL_META_FOREACH_CXT, except that no CONTEXT argument is
 * given. Only the index and current argument will thus be passed to MACRO.
 */
#define DCL_META_FOREACH(MACRO, SEP, ...) \
        DCL_META_FOREACH_CXT(DCL_META_FOREACH_ITER, SEP, MACRO, __VA_ARGS__)

/**
 * For each consecutive variadic argument (up to twenty), MACRO is passed the
 * zero-based index of the current argument, CONTEXT, and then the argument
 * itself. The results of adjoining invocations of MACRO are then separated by
 * SEP.
 *
 * Inspired by P99: http://p99.gforge.inria.fr
 */
#define DCL_META_FOREACH_CXT(MACRO, SEP, CONTEXT, ...) \
        DCL_META_CONCAT(DCL_META_FOREACH_CXT, DCL_META_ARGCOUNT(__VA_ARGS__))(MACRO, SEP, CONTEXT, __VA_ARGS__)

/**
 * Identical to #DCL_META_FOREACH_CXT. This can be used when the former would
 * fail due to recursive macro expansion.
 */
#define DCL_META_FOREACH_CXT_RECURSIVE(MACRO, SEP, CONTEXT, ...) \
        DCL_META_CONCAT(DCL_META_FOREACH_CXT_RECURSIVE, DCL_META_ARGCOUNT(__VA_ARGS__))(MACRO, SEP, CONTEXT, __VA_ARGS__)

/**
 * In consecutive order, appends each variadic argument (up to twenty) onto
 * BASE. The resulting concatenations are then separated by SEP.
 *
 * This is primarily useful to manipulate a list of macro invocations into instead
 * invoking a different, possibly related macro.
 */
#define DCL_META_FOREACH_CONCAT(BASE, SEP, ...) \
        DCL_META_FOREACH_CXT(DCL_META_FOREACH_CONCAT_ITER, SEP, BASE, __VA_ARGS__)

/**
 * Iterates COUNT times, each time invoking MACRO with the current index
 * (starting at zero) and CONTEXT. The results of adjoining invocations of MACRO
 * are then separated by SEP.
 *
 * COUNT must be an integer between zero and twenty, inclusive.
 */
#define DCL_META_FOR_CXT(COUNT, MACRO, SEP, CONTEXT) \
        DCL_META_CONCAT(DCL_META_FOR_CXT, COUNT)(MACRO, SEP, CONTEXT)

/**
 * Returns the first argument given. At least one argument must be provided.
 *
 * This is useful when implementing a variadic macro, where you may have only
 * one variadic argument, but no way to retrieve it (for example, because \c ...
 * always needs to match at least one argument).
 *
 * @code
#define varmacro(...) \
    DCL_META_HEAD(__VA_ARGS__)
 * @endcode
 */
#define DCL_META_HEAD(...) \
        DCL_META_HEAD_(__VA_ARGS__, 0)

/**
 * Returns every argument except the first. At least two arguments must be
 * provided.
 */
#define DCL_META_TAIL(...) \
        DCL_META_TAIL_(__VA_ARGS__)

/**
 * Returns the first N (up to twenty) variadic arguments as a new argument list.
 * At least N variadic arguments must be provided.
 */
#define DCL_META_TAKE(N, ...) \
        DCL_META_CONCAT(DCL_META_TAKE, N)(__VA_ARGS__)

/**
 * Removes the first N (up to twenty) variadic arguments from the given argument
 * list. At least N variadic arguments must be provided.
 */
#define DCL_META_DROP(N, ...) \
        DCL_META_CONCAT(DCL_META_DROP, N)(__VA_ARGS__)

/**
 * Decrements VAL, which must be a number between zero and twenty, inclusive.
 *
 * This is primarily useful when dealing with indexes and counts in
 * metaprogramming.
 */
#define DCL_META_DEC(VAL) \
        DCL_META_AT(VAL, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)

/**
 * Increments VAL, which must be a number between zero and twenty, inclusive.
 *
 * This is primarily useful when dealing with indexes and counts in
 * metaprogramming.
 */
#define DCL_META_INC(VAL) \
        DCL_META_AT(VAL, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21)

/**
 * If A is equal to B, the next argument list is expanded; otherwise, the
 * argument list after that is expanded. A and B must be numbers between zero
 * and twenty, inclusive. Additionally, B must be greater than or equal to A.
 *
 * @code
// expands to true
DCL_META_IF_EQ(0, 0)(true)(false)
// expands to false
DCL_META_IF_EQ(0, 1)(true)(false)
 * @endcode
 *
 * This is primarily useful when dealing with indexes and counts in
 * metaprogramming.
 */
#define DCL_META_IF_EQ(A, B) \
        DCL_META_CONCAT(DCL_META_IF_EQ, A)(B)

/**
 * Identical to #DCL_META_IF_EQ. This can be used when the former would fail
 * due to recursive macro expansion.
 */
#define DCL_META_IF_EQ_RECURSIVE(A, B) \
        DCL_META_CONCAT(DCL_META_IF_EQ_RECURSIVE, A)(B)

/**
 * Returns 1 if N is an even number, or 0 otherwise. N must be between zero and
 * twenty, inclusive.
 *
 * For the purposes of this test, zero is considered even.
 */
#define DCL_META_IS_EVEN(N) \
        DCL_META_AT(N, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1)

/**
 * Returns the logical NOT of B, which must be the number zero or one.
 */
#define DCL_META_NOT(B) \
        DCL_META_AT(B, 1, 0)

// IMPLEMENTATION DETAILS FOLLOW!
// Do not write code that depends on anything below this line.
#define DCL_META_STRINGIFY_(VALUE) # VALUE
#define DCL_META_CONCAT_(A, B) A ## B
#define DCL_META_FOREACH_ITER(INDEX, MACRO, ARG) MACRO(INDEX, ARG)
#define DCL_META_HEAD_(FIRST, ...) FIRST
#define DCL_META_TAIL_(FIRST, ...) __VA_ARGS__
#define DCL_META_CONSUME_(...)
#define DCL_META_EXPAND_(...) __VA_ARGS__

// implemented from scratch so that DCL_META_CONCAT() doesn't end up nesting
#define DCL_META_FOREACH_CONCAT_ITER(INDEX, BASE, ARG) DCL_META_FOREACH_CONCAT_ITER_(BASE, ARG)
#define DCL_META_FOREACH_CONCAT_ITER_(BASE, ARG) BASE ## ARG

// DCL_META_AT expansions
#define DCL_META_AT0(...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT1(_0, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT2(_0, _1, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT3(_0, _1, _2, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT4(_0, _1, _2, _3, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT5(_0, _1, _2, _3, _4, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT6(_0, _1, _2, _3, _4, _5, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT7(_0, _1, _2, _3, _4, _5, _6, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT8(_0, _1, _2, _3, _4, _5, _6, _7, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT9(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_AT20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) DCL_META_HEAD(__VA_ARGS__)

// DCL_META_FOREACH_CXT expansions
#define DCL_META_FOREACH_CXT0(MACRO, SEP, CONTEXT)
#define DCL_META_FOREACH_CXT1(MACRO, SEP, CONTEXT, _0) MACRO(0, CONTEXT, _0)

#define DCL_META_FOREACH_CXT2(MACRO, SEP, CONTEXT, _0, _1) \
    DCL_META_FOREACH_CXT1(MACRO, SEP, CONTEXT, _0) \
    SEP \
    MACRO(1, CONTEXT, _1)

#define DCL_META_FOREACH_CXT3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    DCL_META_FOREACH_CXT2(MACRO, SEP, CONTEXT, _0, _1) \
    SEP \
    MACRO(2, CONTEXT, _2)

#define DCL_META_FOREACH_CXT4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    DCL_META_FOREACH_CXT3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    SEP \
    MACRO(3, CONTEXT, _3)

#define DCL_META_FOREACH_CXT5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    DCL_META_FOREACH_CXT4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    SEP \
    MACRO(4, CONTEXT, _4)

#define DCL_META_FOREACH_CXT6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    DCL_META_FOREACH_CXT5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    SEP \
    MACRO(5, CONTEXT, _5)

#define DCL_META_FOREACH_CXT7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    DCL_META_FOREACH_CXT6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    SEP \
    MACRO(6, CONTEXT, _6)

#define DCL_META_FOREACH_CXT8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    DCL_META_FOREACH_CXT7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    SEP \
    MACRO(7, CONTEXT, _7)

#define DCL_META_FOREACH_CXT9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    DCL_META_FOREACH_CXT8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    SEP \
    MACRO(8, CONTEXT, _8)

#define DCL_META_FOREACH_CXT10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    DCL_META_FOREACH_CXT9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    SEP \
    MACRO(9, CONTEXT, _9)

#define DCL_META_FOREACH_CXT11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    DCL_META_FOREACH_CXT10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    SEP \
    MACRO(10, CONTEXT, _10)

#define DCL_META_FOREACH_CXT12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    DCL_META_FOREACH_CXT11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    SEP \
    MACRO(11, CONTEXT, _11)

#define DCL_META_FOREACH_CXT13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    DCL_META_FOREACH_CXT12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    SEP \
    MACRO(12, CONTEXT, _12)

#define DCL_META_FOREACH_CXT14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    DCL_META_FOREACH_CXT13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    SEP \
    MACRO(13, CONTEXT, _13)

#define DCL_META_FOREACH_CXT15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    DCL_META_FOREACH_CXT14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    SEP \
    MACRO(14, CONTEXT, _14)

#define DCL_META_FOREACH_CXT16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    DCL_META_FOREACH_CXT15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    SEP \
    MACRO(15, CONTEXT, _15)

#define DCL_META_FOREACH_CXT17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    DCL_META_FOREACH_CXT16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    SEP \
    MACRO(16, CONTEXT, _16)

#define DCL_META_FOREACH_CXT18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    DCL_META_FOREACH_CXT17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    SEP \
    MACRO(17, CONTEXT, _17)

#define DCL_META_FOREACH_CXT19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    DCL_META_FOREACH_CXT18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    SEP \
    MACRO(18, CONTEXT, _18)

#define DCL_META_FOREACH_CXT20(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) \
    DCL_META_FOREACH_CXT19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    SEP \
    MACRO(19, CONTEXT, _19)

// DCL_META_FOREACH_CXT_RECURSIVE expansions
#define DCL_META_FOREACH_CXT_RECURSIVE0(MACRO, SEP, CONTEXT)
#define DCL_META_FOREACH_CXT_RECURSIVE1(MACRO, SEP, CONTEXT, _0) MACRO(0, CONTEXT, _0)

#define DCL_META_FOREACH_CXT_RECURSIVE2(MACRO, SEP, CONTEXT, _0, _1) \
    DCL_META_FOREACH_CXT_RECURSIVE1(MACRO, SEP, CONTEXT, _0) \
    SEP \
    MACRO(1, CONTEXT, _1)

#define DCL_META_FOREACH_CXT_RECURSIVE3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    DCL_META_FOREACH_CXT_RECURSIVE2(MACRO, SEP, CONTEXT, _0, _1) \
    SEP \
    MACRO(2, CONTEXT, _2)

#define DCL_META_FOREACH_CXT_RECURSIVE4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    DCL_META_FOREACH_CXT_RECURSIVE3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    SEP \
    MACRO(3, CONTEXT, _3)

#define DCL_META_FOREACH_CXT_RECURSIVE5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    DCL_META_FOREACH_CXT_RECURSIVE4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    SEP \
    MACRO(4, CONTEXT, _4)

#define DCL_META_FOREACH_CXT_RECURSIVE6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    DCL_META_FOREACH_CXT_RECURSIVE5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    SEP \
    MACRO(5, CONTEXT, _5)

#define DCL_META_FOREACH_CXT_RECURSIVE7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    DCL_META_FOREACH_CXT_RECURSIVE6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    SEP \
    MACRO(6, CONTEXT, _6)

#define DCL_META_FOREACH_CXT_RECURSIVE8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    DCL_META_FOREACH_CXT_RECURSIVE7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    SEP \
    MACRO(7, CONTEXT, _7)

#define DCL_META_FOREACH_CXT_RECURSIVE9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    DCL_META_FOREACH_CXT_RECURSIVE8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    SEP \
    MACRO(8, CONTEXT, _8)

#define DCL_META_FOREACH_CXT_RECURSIVE10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    DCL_META_FOREACH_CXT_RECURSIVE9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    SEP \
    MACRO(9, CONTEXT, _9)

#define DCL_META_FOREACH_CXT_RECURSIVE11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    DCL_META_FOREACH_CXT_RECURSIVE10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    SEP \
    MACRO(10, CONTEXT, _10)

#define DCL_META_FOREACH_CXT_RECURSIVE12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    DCL_META_FOREACH_CXT_RECURSIVE11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    SEP \
    MACRO(11, CONTEXT, _11)

#define DCL_META_FOREACH_CXT_RECURSIVE13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    DCL_META_FOREACH_CXT_RECURSIVE12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    SEP \
    MACRO(12, CONTEXT, _12)

#define DCL_META_FOREACH_CXT_RECURSIVE14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    DCL_META_FOREACH_CXT_RECURSIVE13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    SEP \
    MACRO(13, CONTEXT, _13)

#define DCL_META_FOREACH_CXT_RECURSIVE15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    DCL_META_FOREACH_CXT_RECURSIVE14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    SEP \
    MACRO(14, CONTEXT, _14)

#define DCL_META_FOREACH_CXT_RECURSIVE16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    DCL_META_FOREACH_CXT_RECURSIVE15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    SEP \
    MACRO(15, CONTEXT, _15)

#define DCL_META_FOREACH_CXT_RECURSIVE17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    DCL_META_FOREACH_CXT_RECURSIVE16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    SEP \
    MACRO(16, CONTEXT, _16)

#define DCL_META_FOREACH_CXT_RECURSIVE18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    DCL_META_FOREACH_CXT_RECURSIVE17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    SEP \
    MACRO(17, CONTEXT, _17)

#define DCL_META_FOREACH_CXT_RECURSIVE19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    DCL_META_FOREACH_CXT_RECURSIVE18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    SEP \
    MACRO(18, CONTEXT, _18)

#define DCL_META_FOREACH_CXT_RECURSIVE20(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) \
    DCL_META_FOREACH_CXT_RECURSIVE19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    SEP \
    MACRO(19, CONTEXT, _19)

// DCL_META_FOR_CXT expansions
#define DCL_META_FOR_CXT0(MACRO, SEP, CONTEXT)
#define DCL_META_FOR_CXT1(MACRO, SEP, CONTEXT) MACRO(0, CONTEXT)

#define DCL_META_FOR_CXT2(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT1(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(1, CONTEXT)

#define DCL_META_FOR_CXT3(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT2(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(2, CONTEXT)

#define DCL_META_FOR_CXT4(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT3(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(3, CONTEXT)

#define DCL_META_FOR_CXT5(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT4(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(4, CONTEXT)

#define DCL_META_FOR_CXT6(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT5(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(5, CONTEXT)

#define DCL_META_FOR_CXT7(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT6(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(6, CONTEXT)

#define DCL_META_FOR_CXT8(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT7(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(7, CONTEXT)

#define DCL_META_FOR_CXT9(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT8(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(8, CONTEXT)

#define DCL_META_FOR_CXT10(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT9(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(9, CONTEXT)

#define DCL_META_FOR_CXT11(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT10(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(10, CONTEXT)

#define DCL_META_FOR_CXT12(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT11(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(11, CONTEXT)

#define DCL_META_FOR_CXT13(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT12(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(12, CONTEXT)

#define DCL_META_FOR_CXT14(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT13(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(13, CONTEXT)

#define DCL_META_FOR_CXT15(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT14(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(14, CONTEXT)

#define DCL_META_FOR_CXT16(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT15(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(15, CONTEXT)

#define DCL_META_FOR_CXT17(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT16(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(16, CONTEXT)

#define DCL_META_FOR_CXT18(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT17(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(17, CONTEXT)

#define DCL_META_FOR_CXT19(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT18(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(18, CONTEXT)

#define DCL_META_FOR_CXT20(MACRO, SEP, CONTEXT) \
    DCL_META_FOR_CXT19(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(19, CONTEXT)

// DCL_META_IF_EQ expansions
#define DCL_META_IF_EQ0(VALUE) \
    DCL_META_CONCAT(DCL_META_IF_EQ0_, VALUE)

#define DCL_META_IF_EQ0_0(...) __VA_ARGS__ DCL_META_CONSUME_
#define DCL_META_IF_EQ0_1(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_2(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_3(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_4(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_5(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_6(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_7(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_8(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_9(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_10(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_11(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_12(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_13(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_14(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_15(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_16(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_17(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_18(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_19(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ0_20(...) DCL_META_EXPAND_

#define DCL_META_IF_EQ1(VALUE) DCL_META_IF_EQ0(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ2(VALUE) DCL_META_IF_EQ1(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ3(VALUE) DCL_META_IF_EQ2(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ4(VALUE) DCL_META_IF_EQ3(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ5(VALUE) DCL_META_IF_EQ4(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ6(VALUE) DCL_META_IF_EQ5(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ7(VALUE) DCL_META_IF_EQ6(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ8(VALUE) DCL_META_IF_EQ7(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ9(VALUE) DCL_META_IF_EQ8(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ10(VALUE) DCL_META_IF_EQ9(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ11(VALUE) DCL_META_IF_EQ10(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ12(VALUE) DCL_META_IF_EQ11(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ13(VALUE) DCL_META_IF_EQ12(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ14(VALUE) DCL_META_IF_EQ13(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ15(VALUE) DCL_META_IF_EQ14(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ16(VALUE) DCL_META_IF_EQ15(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ17(VALUE) DCL_META_IF_EQ16(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ18(VALUE) DCL_META_IF_EQ17(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ19(VALUE) DCL_META_IF_EQ18(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ20(VALUE) DCL_META_IF_EQ19(DCL_META_DEC(VALUE))

// DCL_META_IF_EQ_RECURSIVE expansions
#define DCL_META_IF_EQ_RECURSIVE0(VALUE) \
    DCL_META_CONCAT(DCL_META_IF_EQ_RECURSIVE0_, VALUE)

#define DCL_META_IF_EQ_RECURSIVE0_0(...) __VA_ARGS__ DCL_META_CONSUME_
#define DCL_META_IF_EQ_RECURSIVE0_1(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_2(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_3(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_4(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_5(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_6(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_7(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_8(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_9(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_10(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_11(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_12(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_13(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_14(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_15(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_16(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_17(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_18(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_19(...) DCL_META_EXPAND_
#define DCL_META_IF_EQ_RECURSIVE0_20(...) DCL_META_EXPAND_

#define DCL_META_IF_EQ_RECURSIVE1(VALUE) DCL_META_IF_EQ_RECURSIVE0(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE2(VALUE) DCL_META_IF_EQ_RECURSIVE1(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE3(VALUE) DCL_META_IF_EQ_RECURSIVE2(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE4(VALUE) DCL_META_IF_EQ_RECURSIVE3(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE5(VALUE) DCL_META_IF_EQ_RECURSIVE4(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE6(VALUE) DCL_META_IF_EQ_RECURSIVE5(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE7(VALUE) DCL_META_IF_EQ_RECURSIVE6(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE8(VALUE) DCL_META_IF_EQ_RECURSIVE7(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE9(VALUE) DCL_META_IF_EQ_RECURSIVE8(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE10(VALUE) DCL_META_IF_EQ_RECURSIVE9(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE11(VALUE) DCL_META_IF_EQ_RECURSIVE10(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE12(VALUE) DCL_META_IF_EQ_RECURSIVE11(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE13(VALUE) DCL_META_IF_EQ_RECURSIVE12(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE14(VALUE) DCL_META_IF_EQ_RECURSIVE13(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE15(VALUE) DCL_META_IF_EQ_RECURSIVE14(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE16(VALUE) DCL_META_IF_EQ_RECURSIVE15(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE17(VALUE) DCL_META_IF_EQ_RECURSIVE16(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE18(VALUE) DCL_META_IF_EQ_RECURSIVE17(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE19(VALUE) DCL_META_IF_EQ_RECURSIVE18(DCL_META_DEC(VALUE))
#define DCL_META_IF_EQ_RECURSIVE20(VALUE) DCL_META_IF_EQ_RECURSIVE19(DCL_META_DEC(VALUE))

// DCL_META_TAKE expansions
#define DCL_META_TAKE0(...)
#define DCL_META_TAKE1(...) DCL_META_HEAD(__VA_ARGS__)
#define DCL_META_TAKE2(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE1(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE3(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE2(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE4(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE3(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE5(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE4(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE6(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE5(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE7(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE6(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE8(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE7(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE9(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE8(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE10(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE9(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE11(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE10(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE12(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE11(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE13(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE12(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE14(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE13(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE15(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE14(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE16(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE15(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE17(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE16(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE18(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE17(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE19(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE18(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_TAKE20(...) DCL_META_HEAD(__VA_ARGS__), DCL_META_TAKE19(DCL_META_TAIL(__VA_ARGS__))

// DCL_META_DROP expansions
#define DCL_META_DROP0(...) __VA_ARGS__
#define DCL_META_DROP1(...) DCL_META_TAIL(__VA_ARGS__)
#define DCL_META_DROP2(...) DCL_META_DROP1(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP3(...) DCL_META_DROP2(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP4(...) DCL_META_DROP3(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP5(...) DCL_META_DROP4(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP6(...) DCL_META_DROP5(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP7(...) DCL_META_DROP6(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP8(...) DCL_META_DROP7(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP9(...) DCL_META_DROP8(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP10(...) DCL_META_DROP9(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP11(...) DCL_META_DROP10(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP12(...) DCL_META_DROP11(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP13(...) DCL_META_DROP12(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP14(...) DCL_META_DROP13(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP15(...) DCL_META_DROP14(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP16(...) DCL_META_DROP15(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP17(...) DCL_META_DROP16(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP18(...) DCL_META_DROP17(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP19(...) DCL_META_DROP18(DCL_META_TAIL(__VA_ARGS__))
#define DCL_META_DROP20(...) DCL_META_DROP19(DCL_META_TAIL(__VA_ARGS__))

#endif // DCL_BASIC_META_MACROS_H