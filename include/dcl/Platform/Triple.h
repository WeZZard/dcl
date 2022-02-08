//===--- Triple.h - Platform Triples ----------------------------*- C++ -*-===//
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

#ifndef DCL_PLATFORM_TRIPLE_H
#define DCL_PLATFORM_TRIPLE_H

#include <cstdint>

namespace dcl::Platform {

class Triple {

public:
  enum class Arch : uint8_t {
#define ARCH(NAME, _2, _3) NAME,
#include <dcl/Platform/Triple/Arch.def>
  };

  /**
   * @brief Parse architecture from a given string.
   *
   * @return Arch
   */
  static Arch getArch(const char *);

  enum class SubArch : uint8_t {
#define SUB_ARCH(NAME, _2, _3) NAME,
#include <dcl/Platform/Triple/SubArch.def>
  };

  /**
   * @brief Parse sub-architecture from a given string.
   *
   * @return SubArch
   */
  static SubArch getSubArch(const char *);

  enum class Vendor : uint8_t {
#define VENDOR(NAME, _2, _3) NAME,
#include <dcl/Platform/Triple/Vendor.def>
  };

  /**
   * @brief Parse vendor from a given string.
   *
   * @return Vendor
   */
  static Vendor getVendor(const char *);

  enum class OS : uint8_t {
#define OS(NAME, _2, _3) NAME,
#include <dcl/Platform/Triple/OS.def>
  };

  /**
   * @brief Parse OS from a given string.
   *
   * @return OS
   */
  static OS getOS(const char *);

private:
  Arch _arch;

  SubArch _subArch;

  Vendor _vendor;

  OS _os;

public:
  Triple(Arch arch, SubArch subArch, Vendor vendor, OS os)
    : _arch(arch), _subArch(subArch), _vendor(vendor), _os(os) {}
};

} // namespace dcl::Platform

#endif // DCL_PLATFORM_TRIPLE_H
