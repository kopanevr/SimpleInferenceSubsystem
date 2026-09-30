add_library(CommandInterpreter STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandInterpreter.cpp)

target_include_directories(
  CommandInterpreter PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Utilities/BitField/include
)
