
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Menu.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SimpleAction.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SimpleMenu.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/GameMenu.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )
