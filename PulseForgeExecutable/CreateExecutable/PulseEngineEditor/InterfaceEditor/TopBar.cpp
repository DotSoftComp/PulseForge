#include "TopBar.h"
#include "imgui/imgui.h"

void TopBar::UpdateBar()
{    
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) { /* Handle New action */ }
            if (ImGui::MenuItem("Open")) { /* Handle Open action */ }
            if (ImGui::MenuItem("Save")) { /* Handle Save action */ }
            if (ImGui::MenuItem("Exit")) { /* Handle Exit action */ }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo")) { /* Handle Undo action */ }
            if (ImGui::MenuItem("Redo")) { /* Handle Redo action */ }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) { /* Handle About action */ }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
