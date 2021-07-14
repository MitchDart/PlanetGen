#include "ui.h"

ui::ui(GLFWwindow* _window, std::vector<std::shared_ptr<windowable>> _windowable) {
    window = _window;
    windowables = _windowable;
}

void ui::on_create() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ui::on_draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug tools", &showDebug, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        for (auto windowable : windowables) 
        {  
            if (ImGui::BeginMenu(windowable->window_name()))
            {
                windowable->on_draw_ui();
                ImGui::EndMenu();
            }
        }
        if(ImGui::BeginMenu("ImGui"))
        {
            if(ImGui::Button("Show demo")) {
                showDemo = !showDemo;
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }
    ImGui::End();

    if (showDemo) {
        ImGui::ShowDemoWindow(&showDemo);
    }

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ui::on_destroy() {}

void ui::on_update(double delta) {}