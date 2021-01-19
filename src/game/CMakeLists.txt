
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/MobData.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/MobFactory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SpawnerFactory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )

add_subdirectory (
  ${CMAKE_CURRENT_SOURCE_DIR}/towers
  )
