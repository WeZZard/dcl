add_library(dclBinary INTERFACE)

target_link_libraries(
  dclBinary
  INTERFACE
  dclPlatform
  dclADT
)
