
target_sources (tdef_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiair.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Basic.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Blast.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Freezing.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Minigun.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Missile.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Multishot.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Sniper.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Splash.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Tesla.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Venom.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TowerData.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TowerFactory.cc
  )

target_include_directories (tdef_lib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )
