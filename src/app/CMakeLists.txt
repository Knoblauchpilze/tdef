
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/olcEngine.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TexturePack.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PGEApp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TDefApp.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )
