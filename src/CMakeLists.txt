project (tdef_lib)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

set (SOURCES
  ${SOURCES}
  olcEngine.cc
  )

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/coordinates
  )

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/world
  )

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/app
  )

add_library (tdef_lib SHARED
  ${SOURCES}
  )

set (NEW_FRONTIERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_link_libraries(tdef_lib
  sdl_core
  png
  X11
  GL
  pthread
  stdc++fs
  )

target_include_directories (tdef_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/app
  )
