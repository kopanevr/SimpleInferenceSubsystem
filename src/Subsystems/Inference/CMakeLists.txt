add_library(Inference SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/Inference.cpp)

target_include_directories(
  Inference PUBLIC
  ${CMAKE_SOURCE_DIR}/thirdParty/onnxruntime-linux-x64-1.30.0/include
)

target_include_directories(
  Inference PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/Subsystems/Subsystem/include
  ${CMAKE_SOURCE_DIR}/src/Subsystems/Logger/include
)

target_link_libraries(Inference PRIVATE ${CMAKE_SOURCE_DIR}/thirdParty/onnxruntime-linux-x64-1.30.0/lib/libonnxruntime.so)

target_link_libraries(Inference PRIVATE Logger)

if (USER_OPTION_SHOW_MODEL_INFO)
  target_compile_definitions(Inference PRIVATE USER_OPTION_SHOW_MODEL_INFO)
endif()
