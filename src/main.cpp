// Dear ImGui: standalone example application for GLFW + OpenGL 3, using
//
// programmable pipeline (GLFW is a cross-platform general purpose library for
// handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation,
// etc.) If you are new to Dear ImGui, read documentation from the docs/ folder
// + read the top of imgui.cpp. Read online:
// https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to
// maximize ease of testing and compatibility with old VS compilers. To link
// with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project
// should not be affected, as you are likely to link with a newer binary of GLFW
// that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include "chartgfx.hpp"
#include "chartsettings.hpp"
#include "controlswidget.hpp"
#include "imvecext.hpp"
#include "planetswidget.hpp"
#include "swephpp.hpp"
#include "util.hpp"
#include <vector>

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char **) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(
      1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Our state
  bool show_demo_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ n a string
  // literal you need to write a double backslash \\ !
  io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  ImFont *zodiacs_font =
      io.Fonts->AddFontFromFileTTF("../misc/fonts/zodiac_s.ttf", 18.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);
  specni::ChartSettings settings(zodiacs_font);
  specni::ChartModel model;
  specni::ControlsWidget controls(&model);
  specni::PlanetsWidget planets(&model);
  specni::ChartWidget chart(settings, &model);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application. Generally you may always pass all inputs
    // to dear imgui, and hide them from your application based on those two
    // flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    controls.Show();
    planets.Show();
    chart.Show();
    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to created a named window.
    //{
    // static float f = 0.0f;
    //      static int counter = 0;
    //
    //      ImGui::Begin("Hello, world!"); // Create a window called "Hello,
    //      world!"
    //
    //      static float sz = 36.0f;
    //      static float thickness = 3.0f;
    //      static int ngon_sides = 6;
    //      static bool circle_segments_override = false;
    //      static int circle_segments_override_v = 12;
    //      static bool curve_segments_override = false;
    //      static int curve_segments_override_v = 8;
    //      static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
    //      // and append into it.
    //
    //      ImDrawList *draw_list = ImGui::GetWindowDrawList();
    //      const ImVec2 p = ImGui::GetCursorScreenPos();
    //      const ImU32 col = ImColor(colf);
    //      const float spacing = 10.0f;
    //      const ImDrawFlags corners_tl_br =
    //          ImDrawFlags_RoundCornersTopLeft |
    //          ImDrawFlags_RoundCornersBottomRight;
    //      const float rounding = sz / 5.0f;
    //      const int circle_segments =
    //          circle_segments_override ? circle_segments_override_v : 0;
    //      const int curve_segments =
    //          curve_segments_override ? curve_segments_override_v : 0;
    //      float x = p.x + 4.0f;
    //      float y = p.y + 4.0f;
    //
    // ImVec2 window_pos = ImGui::GetWindowPos();
    //            ImVec2 window_size = ImGui::GetWindowSize();
    //            ImVec2 window_center = ImVec2(window_pos.x + window_size.x *
    //            0.5f, window_pos.y + window_size.y * 0.5f);
    //
    //        draw_list->AddCircle(window_center, window_size.y * 0.45f, col,
    //        circle_segments, 1);
    //
    //      for (int n = 0; n < 2; n++) {
    //        // First line uses a thickness of 1.0f, second line uses the
    //        // configurable thickness
    //        float th = (n == 0) ? 1.0f : thickness;
    //        draw_list->AddNgon(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz *
    //        0.5f, col,
    //                           ngon_sides, th);
    //        x += sz + spacing; // N-gon
    //        draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz *
    //        0.5f,
    //                             col, circle_segments, th);
    //        x += sz + spacing; // Circle
    //        draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col,
    //        0.0f,
    //                           ImDrawFlags_None, th);
    //        x += sz + spacing; // Square
    //        draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col,
    //        rounding,
    //                           ImDrawFlags_None, th);
    //        x += sz + spacing; // Square with all rounded corners
    //        draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col,
    //        rounding,
    //                           corners_tl_br, th);
    //        x += sz + spacing; // Square with two rounded corners
    //        draw_list->AddTriangle(ImVec2(x + sz * 0.5f, y),
    //                               ImVec2(x + sz, y + sz - 0.5f),
    //                               ImVec2(x, y + sz - 0.5f), col, th);
    //        x += sz + spacing; // Triangle
    //        // draw_list->AddTriangle(ImVec2(x+sz*0.2f,y), ImVec2(x,
    //        y+sz-0.5f),
    //        // ImVec2(x+sz*0.4f, y+sz-0.5f), col, th);x+= sz*0.4f + spacing;
    //        // Thin
    //        // triangle
    //        draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col, th);
    //        x += sz + spacing; // Horizontal line (note: drawing a filled
    //        rectangle
    //                           // will be faster!)
    //        draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col, th);
    //        x += spacing; // Vertical line (note: drawing a filled rectangle
    //        will be
    //                      // faster!)
    //        draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col, th);
    //        x += sz + spacing; // Diagonal line
    //
    //        // Quadratic Bezier Curve (3 control points)
    //        ImVec2 cp3[3] = {ImVec2(x, y + sz * 0.6f),
    //                         ImVec2(x + sz * 0.5f, y - sz * 0.4f),
    //                         ImVec2(x + sz, y + sz)};
    //        draw_list->AddBezierQuadratic(cp3[0], cp3[1], cp3[2], col, th,
    //                                      curve_segments);
    //        x += sz + spacing;
    //
    //        // Cubic Bezier Curve (4 control points)
    //        ImVec2 cp4[4] = {ImVec2(x, y), ImVec2(x + sz * 1.3f, y + sz *
    //        0.3f),
    //                         ImVec2(x + sz - sz * 1.3f, y + sz - sz * 0.3f),
    //                         ImVec2(x + sz, y + sz)};
    //        draw_list->AddBezierCubic(cp4[0], cp4[1], cp4[2], cp4[3], col, th,
    //                                  curve_segments);
    //
    //        x = p.x + 4;
    //        y += sz + spacing;
    //      }
    //      draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz *
    //      0.5f,
    //                               col, ngon_sides);
    //      x += sz + spacing; // N-gon
    //      draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f),
    //                                 sz * 0.5f, col, circle_segments);
    //      x += sz + spacing; // Circle
    //      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);
    //      x += sz + spacing; // Square
    //      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col,
    //                               10.0f);
    //      x += sz + spacing; // Square with all rounded corners
    //      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz),
    //      col, 10.0f,
    //                               corners_tl_br);
    //      x += sz + spacing; // Square with two rounded corners
    //      draw_list->AddTriangleFilled(ImVec2(x + sz * 0.5f, y),
    //                                   ImVec2(x + sz, y + sz - 0.5f),
    //                                   ImVec2(x, y + sz - 0.5f), col);
    //      x += sz + spacing; // Triangle
    //      // draw_list->AddTriangleFilled(ImVec2(x+sz*0.2f,y), ImVec2(x,
    //      y+sz-0.5f),
    //      // ImVec2(x+sz*0.4f, y+sz-0.5f), col); x += sz*0.4f + spacing; //
    //      Thin
    //      // triangle
    //      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y +
    //      thickness),
    //                               col);
    //      x += sz + spacing; // Horizontal line (faster than AddLine, but only
    //                         // handle integer thickness)
    //      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y +
    //      sz),
    //                               col);
    //      x += spacing * 2.0f; // Vertical line (faster than AddLine, but only
    //                           // handle integer thickness)
    //      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 1, y + 1), col);
    //      x += sz; // Pixel (faster than AddLine)
    //      draw_list->AddRectFilledMultiColor(
    //          ImVec2(x, y), ImVec2(x + sz, y + sz), IM_COL32(0, 0, 0, 255),
    //          IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255),
    //          IM_COL32(0, 255, 0, 255));
    //
    // ImGui::Text("This is some useful text."); // Display some text (you can
    // use a format strings too)
    // ImGui::Checkbox(
    //  "Demo Window",
    //&show_demo_window); // Edit bools storing our window open/close state

    // widgets return true when edited/activated)

    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
    //               1000.0f / ImGui::GetIO().Framerate,
    //               ImGui::GetIO().Framerate);
    // ImGui::End();
    //}

    // ImGui::GetBackgroundDrawList()->AddCircle(
    //  ImVec2(600.0, 400.0), 400.0, IM_COL32(255, 0, 0, 200), 0, 10 + 4);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
