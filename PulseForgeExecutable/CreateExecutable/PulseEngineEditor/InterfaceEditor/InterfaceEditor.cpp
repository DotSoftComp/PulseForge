
#include "InterfaceEditor.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include "PulseEngineEditor/InterfaceEditor/TopBar.h"
#include "PulseEngine/core/Entity/Entity.h"
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

InterfaceEditor::InterfaceEditor(PulseEngineBackend* engine)
{    
    topbar = new TopBar();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Optionnel pour accéder à io

    // 2. Config options (ex: enable keyboard controls, docking, etc.)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Navigation clavier

    // 3. Style
    ImGui::StyleColorsDark(); // ou Light()

    // 4. Initialiser les backends
    ImGui_ImplGlfw_InitForOpenGL(engine->GetWindowContext()->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    // Custom ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 3.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 5.0f;
    style.ChildRounding = 3.0f;
    
    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;
    
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
    
    style.Colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.55f, 0.55f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.11f, 0.12f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.09f, 0.94f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.35f, 0.35f, 0.39f, 0.60f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.21f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.26f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.30f, 0.31f, 0.33f, 1.00f);
    
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.10f, 0.11f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.10f, 0.10f, 0.10f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    
    style.Colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.30f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.33f, 0.35f, 0.38f, 1.00f);
    
    style.Colors[ImGuiCol_Header]                = ImVec4(0.25f, 0.26f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.30f, 0.32f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.35f, 0.37f, 0.40f, 1.00f);
    
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.35f, 0.35f, 0.39f, 0.60f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.48f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);
    
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    
    style.Colors[ImGuiCol_Tab]                   = ImVec4(0.18f, 0.19f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.24f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    
}

void InterfaceEditor::Render(PulseEngineBackend *engine)
{        
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Define the main viewport dimensions
    ImVec2 viewportSize = ImVec2(1024, 720);
    ImVec2 windowSize = ImVec2(1920, 1080);

    // Calculate the position of the viewport to center it in the window
    ImVec2 viewportPos = ImVec2(
        (windowSize.x - viewportSize.x) / 2.0f,
        (windowSize.y - viewportSize.y) / 2.0f
    );

    // Render frames around the viewport
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, viewportPos.y));
    ImGui::Begin("Top Frame");
    ImGui::Text("Top Frame");
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, viewportPos.y));
    ImGui::SetNextWindowSize(ImVec2(viewportPos.x, viewportSize.y));
    GenerateSceneDataWindow(engine);

    ImGui::SetNextWindowPos(ImVec2(viewportPos.x + viewportSize.x, viewportPos.y));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x - (viewportPos.x + viewportSize.x), viewportSize.y));
    EntityAnalyzerWindow();

    ImGui::SetNextWindowPos(ImVec2(0, viewportPos.y + viewportSize.y));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y - (viewportPos.y + viewportSize.y)));
    ImGui::Begin("Bottom Frame");
    ImGui::Text("Bottom Frame");
    ImGui::End();

    ImGui::Begin("Viewport");

    ImGui::Image((ImTextureID)(intptr_t)engine->fboTexture, ImVec2(engine->fboWidth, engine->fboHeight), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    
    topbar->UpdateBar();
    // Rendu de la frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InterfaceEditor::EntityAnalyzerWindow()
{
    ImGui::Begin("Entity analyzer");

    if (selectedEntity)
    {
        ImGui::Text("Selected Entity: %s", selectedEntity->GetName().c_str());
        ImGui::Separator();

        glm::vec3 position = selectedEntity->GetPosition();
        glm::vec3 rotation = selectedEntity->GetRotation();
        glm::vec3 scale = selectedEntity->GetScale();
        if (ImGui::TreeNode("Transform"))
        {
            // Position
            ImGui::Text("Position:");
            if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                selectedEntity->SetPosition(position);
            }

            // Rotation
            ImGui::Text("Rotation:");
            if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                selectedEntity->SetRotation(rotation);
            }

            // Scale
            ImGui::Text("Scale:");
            if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                selectedEntity->SetScale(scale);
            }

            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::Text("No entity selected.");
    }

    ImGui::End();
}

void InterfaceEditor::GenerateSceneDataWindow(PulseEngineBackend *engine)
{
    ImGui::Begin("Scene Manager");

    ImGui::Text("Scene Management");
    ImGui::Separator();

    if (ImGui::Button("Clear Scene", ImVec2(-1, 0))) // Full-width button
    {
        selectedEntity = nullptr;
        engine->ClearScene();
    }

    ImGui::Spacing();

    // Get all files from the Scenes directory
    std::vector<std::string> sceneFiles = SceneLoader::GetSceneFiles("PulseEngineEditor/Scenes");

    for (const auto &fileName : sceneFiles)
    {
        if (ImGui::Button(fileName.c_str(), ImVec2(-1, 0))) // Full-width button
        {
            SceneLoader::LoadScene(fileName, engine);
        }
        ImGui::Spacing();
    }

    ImGui::Spacing();

    static bool openPopup = false;
    static char sceneName[128] = ""; // Buffer for scene name input

    // Save Scene button
    if (ImGui::Button("Save Scene", ImVec2(-1, 0))) // Full-width button
    {
        openPopup = true;
        ImGui::OpenPopup("Save Scene");
    }

    // Popup
    if (ImGui::BeginPopupModal("Save Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter scene name:");
        ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName));

        ImGui::Spacing();
        if (ImGui::Button("Save", ImVec2(120, 0)))
        {
            SceneLoader::SaveSceneToFile(sceneName, engine);
            ImGui::CloseCurrentPopup();
            openPopup = false;
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            openPopup = false;
        }

        ImGui::EndPopup();
    }

    ImGui::Spacing();

    ImGui::Text("Scene Entities:");
    // Create a scrollable zone for displaying entities
    ImGui::BeginChild("Scene Entities", ImVec2(0, -1), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (size_t i = 0; i < engine->entities.size(); i++)
    {
        if(!engine->entities[i]) continue;
        std::string entityLabel = "Entity: " + engine->entities[i]->GetName() + "_"  + std::to_string(i) +  "##" + std::to_string(i);
        std::string deleteButtonLabel = "X##" + std::to_string(i);

        if (ImGui::Button(entityLabel.c_str()))
        {
            selectedEntity = engine->entities[i];
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::GetStyle().FramePadding.x * 3 - 5); // Move cursor to the right
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // Red background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Brighter red on hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f)); // Darker red when active
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f)); // Smaller padding for square button

        if (ImGui::Button(deleteButtonLabel.c_str()))
        {
            if (selectedEntity == engine->entities[i])
            {
                selectedEntity = nullptr;
            }
            engine->DeleteEntity(engine->entities[i]);
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::Separator();
    }

    ImGui::EndChild();

    ImGui::End();
}
