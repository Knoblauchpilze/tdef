
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Path.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AStar.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Mob.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )
