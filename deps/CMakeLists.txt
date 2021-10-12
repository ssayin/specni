set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(SWISSEPH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/swisseph)

add_library(IMGUI STATIC)
add_library(SWISSEPH STATIC)

target_sources(
  IMGUI
  PRIVATE ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp
          ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp
          ${IMGUI_DIR}/imgui.cpp
  PRIVATE ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
          ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp)

target_include_directories(
  IMGUI
  PUBLIC ${IMGUI_DIR}
  PUBLIC ${IMGUI_DIR}/backends
  PUBLIC ${SDL2_DIR}/include)

target_sources(
        SWISSEPH
        PRIVATE ${SWISSEPH_DIR}/sweph.c
        ${SWISSEPH_DIR}/swephlib.c
        ${SWISSEPH_DIR}/swehouse.c
        ${SWISSEPH_DIR}/swedate.c
        ${SWISSEPH_DIR}/swecl.c
        ${SWISSEPH_DIR}/swevents.c
        ${SWISSEPH_DIR}/sweasp.c
        ${SWISSEPH_DIR}/swehel.c
        ${SWISSEPH_DIR}/sweephe4.c
        ${SWISSEPH_DIR}/swemplan.c
        ${SWISSEPH_DIR}/swemmoon.c
        ${SWISSEPH_DIR}/swejpl.c
)

target_include_directories(
        SWISSEPH
        PUBLIC ${SWISSEPH_DIR}
)
