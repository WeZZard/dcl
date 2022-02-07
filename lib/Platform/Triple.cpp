#include <dcl/Platform/Triple.h>

#include <cstring>

namespace dcl::Platform {

Triple::Arch Triple::getArch(const char * string) {
#define ARCH(NAME, PATTERN, _3)                                                \
  if (strcmp(string, #PATTERN) == 0) {                                         \
    return Triple::Arch::NAME;                                                 \
  }
#include <dcl/Platform/Triple/Arch.def>
  return Triple::Arch::Unknown;
}

Triple::SubArch Triple::getSubArch(const char * string) {
#define SUB_ARCH(NAME, PATTERN, _3)                                            \
  if (strcmp(string, #PATTERN) == 0) {                                         \
    return Triple::SubArch::NAME;                                              \
  }
#include <dcl/Platform/Triple/SubArch.def>
  return Triple::SubArch::NoSubArch;
}

Triple::Vendor Triple::getVendor(const char * string) {
#define VENDOR(NAME, PATTERN, _3)                                              \
  if (strcmp(string, #PATTERN) == 0) {                                         \
    return Triple::Vendor::NAME;                                               \
  }
#include <dcl/Platform/Triple/Vendor.def>
  return Triple::Vendor::Unknown;
}

Triple::OS Triple::getOS(const char * string) {
#define OS(NAME, PATTERN, _3)                                                  \
  if (strcmp(string, #PATTERN) == 0) {                                         \
    return Triple::OS::NAME;                                                   \
  }
#include <dcl/Platform/Triple/OS.def>
  return Triple::OS::Unknown;
}

} // namespace dcl::Platform
