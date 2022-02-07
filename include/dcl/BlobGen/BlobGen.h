//===--- BlobGen.h - Blob Generator -----------------------------*- C++ -*-===//
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

#ifndef DCL_BLOBGEN_BLOBGEN_H
#define DCL_BLOBGEN_BLOBGEN_H

#include <dcl/Basic/Basic.h>
#include <dcl/Driver/Driver.h>
#include <dcl/Platform/Triple.h>
#include <string>
#include <vector>

namespace dcl::BlobGen {

/**
 * @brief Get all supported triples.
 *
 * @return std::vector<std::string>
 */
std::vector<std::string> getAllTriples();

/**
 * @brief Get all supported compiler suite.
 *
 * @return std::vector<std::string>
 */
std::vector<std::string> getAllCompilerSuites();

const char * getPath(
  const char * sourceFilePath,
  dcl::Platform::Triple triple,
  dcl::Driver::Suite compilerSuite,
  ... // additional compiler args
);

const char * getPath(
  const char ** sourceFilePaths,
  size_t sourceFileCount,
  dcl::Platform::Triple triple,
  dcl::Driver::Suite compilerSuite,
  ... // additional compiler args
);

} // namespace dcl::BlobGen

#endif // DCL_BLOBGEN_BLOBGEN_H
