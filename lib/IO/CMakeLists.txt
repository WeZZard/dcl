include_directories(./)

add_library(
  dclIO
  STATIC
  File.cpp
)

target_link_libraries(
  dclIO
  dclBasic
)
