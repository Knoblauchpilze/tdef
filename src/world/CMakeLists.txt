
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Block.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Spawner.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Wall.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Portal.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/StepInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Locator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/World.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/mobs
  )

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/towers
  )
