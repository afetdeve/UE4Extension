// main.cpp
#include <cstdio>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imnodes.h" // imnodes header
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static void glfw_error_callback(int error, const char *description)
{
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    // ---- GLFW init ----
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "GLFW başlatılamadı!\n";
        return -1;
    }

#if __APPLE__
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Visual Script Editor", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Pencere oluşturulamadı!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // ---- GLAD ----
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD yüklenemedi!\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // ---- ImGui ----
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ---- ImNodes ----
    ImNodes::CreateContext();

    // UI state (EKSİK OLANLAR)
    bool show_imgui_demo = true;
    ImVec4 clear_color = ImVec4(0.10f, 0.12f, 0.15f, 1.00f);

    // ---- Main loop ----
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Toolbox
        ImGui::Begin("Toolbox");
        ImGui::Checkbox("Show ImGui Demo", &show_imgui_demo);
        ImGui::ColorEdit3("Clear Color", (float *)&clear_color);
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::End();

        if (show_imgui_demo)
            ImGui::ShowDemoWindow(&show_imgui_demo);

        // ---- Node Editor penceresi ----
        ImGui::Begin("Node Editor"); // <- eksikti
        ImNodes::BeginNodeEditor();

        // Node A
        ImNodes::BeginNode(1);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Input Node");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(2);
        ImGui::Text("In");
        ImNodes::EndInputAttribute();
        ImNodes::EndNode();

        // Node B
        ImNodes::BeginNode(3);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Output Node");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginOutputAttribute(4);
        ImGui::Indent(40.0f);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();
        ImNodes::EndNode();

        // Link
        ImNodes::Link(100, 4, 2);

        ImNodes::EndNodeEditor();
        ImGui::End(); // Node Editor penceresini kapat

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImNodes::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
