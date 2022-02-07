//===--- BlobGen.cpp - Blob Generator ---------------------------*- C++ -*-===//
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

#include <dcl/BlobGen/BlobGen.h>

namespace dcl::BlobGen {

const char * getPathv(
  const char ** sourceFilePaths,
  size_t sourceFileCount,
  dcl::Platform::Triple triple,
  dcl::Driver::Suite compilerSuite,
  va_list compilerArgs);

std::vector<std::string> getAllTriples() { return {}; }

std::vector<std::string> getAllCompilerSuites() { return {}; }

const char * getPath(
  const char * sourceFilePath,
  dcl::Platform::Triple triple,
  dcl::Driver::Suite compilerSuite,
  ...) {
  va_list compilerArgs;
  va_start(compilerArgs, compilerSuite);
  const char * path =
    getPathv(&sourceFilePath, 1, triple, compilerSuite, compilerArgs);
  va_end(compilerArgs);
  return path;
}

const char * getPath(
  const char ** sourceFilePaths,
  size_t sourceFileCount,
  dcl::Platform::Triple triple,
  dcl::Driver::Suite compilerSuite,
  ...) {
  va_list compilerArgs;
  va_start(compilerArgs, compilerSuite);
  const char * path = getPathv(
    sourceFilePaths, sourceFileCount, triple, compilerSuite, compilerArgs);
  va_end(compilerArgs);
  return path;
}

const char * getPathv(
  const char ** sourceFilePaths,
  size_t sourceFileCount,
  dcl::Platform::Triple triple,
  dcl::Driver::Suite compilerSuite,
  va_list compilerArgs) {
  return nullptr;
}

} // namespace dcl::BlobGen
