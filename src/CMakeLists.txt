add_executable(
  specni
  include/swephpp.hpp
  util/util.cpp
  gui/AspectsWidget.cpp
  gui/PlanetsWidget.cpp
  gui/ControlsWidget.cpp
  gui/ChartModel.cpp
  gui/MoonPhase.cpp
  gui/AspectsWidget.cpp
  gui/ChartWidget.cpp
  main.cpp)

target_link_libraries(specni ${SDL2_LIBRARIES} IMGUI OpenGL::GL dl ${swisseph_LIBRARIES})
target_include_directories(specni PUBLIC ../deps/imgui/backends)
