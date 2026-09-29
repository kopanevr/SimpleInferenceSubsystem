add_library(SubsystemManager SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/SubsystemManager.cpp)

target_include_directories(
  SubsystemManager PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Subsystems/Logger/include
  ${CMAKE_SOURCE_DIR}/src/Subsystems/Inference/include
)

target_link_libraries(
  SubsystemManager PRIVATE
  Logger
)

target_link_libraries(
  SubsystemManager PUBLIC
  Inference
)
