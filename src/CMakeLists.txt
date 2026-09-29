add_executable(Application Application.cpp main.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/CommandInterpreter)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Subsystems/Logger)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Subsystems/Inference)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Subsystems/SubsystemManager)

target_include_directories(
  Application PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationContext/include
  ${CMAKE_CURRENT_SOURCE_DIR}/CommandInterpreter/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Subsystems/SubsystemManager/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/BitField/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/TimerManager/include
)

target_link_libraries(
  Application PRIVATE
  CommandInterpreter
  SubsystemManager
)

add_custom_command(
  TARGET Application
  POST_BUILD
  COMMAND size Application --format=gnu
  COMMENT "Размеры секций:"
)
