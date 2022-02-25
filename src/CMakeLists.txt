add_executable(
  specni
  core/swephpp/Context.cpp
  core/swephpp/House.cpp
  core/swephpp/Ut.cpp
  core/swephpp/Planet.cpp
  core/ChartModel.cpp

  main.cpp)

set_property(TARGET specni PROPERTY C_STANDARD 17)
set_property(TARGET specni PROPERTY CXX_STANDARD 20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_link_libraries(specni ${SDL2_LIBRARIES} IMGUI OpenGL::GL dl SWISSEPH)
target_include_directories(specni PUBLIC ../deps/imgui/backends ../deps/swisseph)
