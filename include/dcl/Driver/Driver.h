//===--- Driver.h - Compilers Driver ----------------------------*- C++ -*-===//
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

#ifndef DCL_DRIVER_DRIVER_H
#define DCL_DRIVER_DRIVER_H

namespace dcl::Driver {

class Configuration {
public:
};

class Driver;

/**
 * @brief  Defines a suite of compiler driver.
 *
 */
class Suite {
private:
  const char * _rootPath;

public:
  Driver makeDriver() const noexcept;
};

/**
 * @brief Represents a compiler driver instance.
 *
 */
class Driver {
public:
  /**
   * @brief Compiles source code with given path.
   *
   * @param arguments Raw arguments sent to the compiler.
   * @return const char* The path of compiled production.
   */
  const char * compile(const char * arguments) noexcept;
};

} // namespace dcl::Driver

#endif // DCL_DRIVER_DRIVER_H
