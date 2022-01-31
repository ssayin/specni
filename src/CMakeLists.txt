add_executable(
  specni
  core/swephpp.hpp
  core/MoonPhase.cpp
  core/PlanetStates.cpp
  core/PlanetPairs.cpp
  core/ChartModel.cpp
  core/Horary.cpp
  gui/imgui_main.cpp
  gui/AspectsWidget.cpp
  gui/PlanetsWidget.cpp
  gui/ControlsWidget.cpp
  gui/AspectsWidget.cpp
  gui/UniWheel.cpp
  gui/BiWheel.cpp
  util/Calendar.cpp
  util/Util.cpp
  main.cpp)

set_property(TARGET specni PROPERTY C_STANDARD 17)
set_property(TARGET specni PROPERTY CXX_STANDARD 20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_link_libraries(specni ${SDL2_LIBRARIES} IMGUI OpenGL::GL dl SWISSEPH)
target_include_directories(specni PUBLIC ../deps/imgui/backends ../deps/swisseph)
