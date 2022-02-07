include_directories(./)

add_library(
  dclBlobGen
  STATIC
  BlobGen.cpp
)

target_link_libraries(
  dclBlobGen
  dclPlatform
  dclDriver
)
