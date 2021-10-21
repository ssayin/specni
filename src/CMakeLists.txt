add_executable(
  specni
  core/swephpp.hpp
  core/MoonPhase.cpp
  core/AccidentalStates.cpp
  core/EssentialStates.cpp
  core/PlanetPairs.cpp
  core/Aspects.cpp
  gui/AspectsWidget.cpp
  gui/PlanetsWidget.cpp
  gui/ControlsWidget.cpp
  gui/ChartModel.cpp
  gui/AspectsWidget.cpp
  gui/RadixChartWidget.cpp
  util/Calendar.cpp
  main.cpp)

set_property(TARGET specni PROPERTY C_STANDARD 17)
set_property(TARGET specni PROPERTY CXX_STANDARD 20)

target_link_libraries(specni ${SDL2_LIBRARIES} IMGUI OpenGL::GL dl SWISSEPH)
target_include_directories(specni PUBLIC ../deps/imgui/backends ../deps/swisseph)