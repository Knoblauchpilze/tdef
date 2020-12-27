
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/CoordinateFrame.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )
