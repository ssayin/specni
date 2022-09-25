add_executable(
  specni
  Chart.cpp
  main.cpp
  Wheel.cpp
  GuiMain.cpp
)

set_property(TARGET specni PROPERTY CXX_STANDARD 20)
set_property(TARGET specni PROPERTY CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_link_libraries(specni ${SDL2_LIBRARIES} IMGUI OpenGL::GL dl SWISSEPH fmt::fmt)

target_include_directories(specni PUBLIC ../deps/imgui/backends ../deps/swisseph ../include)
