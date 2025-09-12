/**
 * @file TopBar.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "TopBar.h"

#include <cstdlib>

// #include "json.hpp"

#include "imgui/imgui.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceEditor.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PulseEngine/core/Lights/PointLight/PointLight.h"
#include "PulseEngine/core/Lights/DirectionalLight/DirectionalLight.h"
#include "PulseEngine/core/FileManager/FileManager.h"
#include "PulseEngine/core/coroutine/CoroutineManager.h"
#include "PulseEngineEditor/InterfaceEditor/BuildGameCoroutine.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include <windows.h>
#include <commdlg.h>


void ResetWorkingDirectoryToExe()
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::current_path(exeDir);
}


/**
 * @brief Update the top bar of the editor interface. It's actually the render of the bar in ImGui
 * 
 * @param[in] engine pointer to the actual backend, needed to perform modification on entities, lighting, etcetc
 * @param[in] editor the interface, because the topbar isn't part of the interface module, but need to rely on it sometimes.
 */
void TopBar::UpdateBar(PulseEngineBackend* engine, InterfaceEditor* editor)
{
    static bool requestOpenNewMapPopup = false;
    static char newMapName[128] = "";

    // === MENU BAR ===
    if (ImGui::BeginMainMenuBar())
    {
        // === FILE MENU ===

        if(ImGui::BeginMenu("Map"))
        {
            if (ImGui::MenuItem("New map"))
            {
                requestOpenNewMapPopup = true;
                strcpy(newMapName, "");
            }
            if (ImGui::MenuItem("Save"))
            {
                SceneLoader::SaveSceneToFile(engine->actualMapName, engine);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Build"))
        {

            if (ImGui::MenuItem("Build game"))
            {     
                ///
                BuildGameToWindow(engine, editor);
            }
            if(ImGui::MenuItem("Compile user scripts"))
            {
                ScriptsLoader::FreeDll(); 
                CompileUserScripts(editor, "CustomScripts.dll");
                ScriptsLoader::LoadDLL();
                
            }

            ImGui::EndMenu();
        }

        // === EDIT MENU ===
        if (ImGui::BeginMenu("Add"))
        {
            
            if (ImGui::BeginMenu("Primitive"))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    int counter = 0;
                    std::string baseName = "Cube";
                    std::string finalName = baseName;
                
                    // Find a unique name
                    bool nameExists = true;
                    while (nameExists)
                    {
                        nameExists = false;
                        for (const auto& ent : engine->entities)
                        {
                            if (ent->GetName() == finalName)
                            {
                                nameExists = true;
                                counter++;
                                finalName = baseName + " (" + std::to_string(counter) + ")";
                                break;
                            }
                        }
                    }

                    GuidReader::InsertIntoCollection("Entities/primitiveCube.pEntity");
                
                    // Create entity with unique name
                    Entity* cube = new Entity(
                        finalName,
                        PulseEngine::Vector3(0.0f),
                        Primitive::Cube(),
                        MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat")
                    );
                    cube->SetGuid(GenerateGUIDFromPath("Entities/primitiveCube.pEntity"));
                    cube->SetMuid(GenerateGUIDFromPathAndMap(finalName, engine->actualMapPath));
                    engine->entities.push_back(cube);
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Point light"))
            {
                engine->lights.push_back(new PointLight(
                    PulseEngine::Vector3(0.0f, 5.0f, 0.0f),
                    PulseEngine::Color(1.0f, 1.0f, 1.0f),
                    5.0f,
                    5.0f,
                    50.0f
                ));

            }
            if (ImGui::MenuItem("Directional light"))
            {
                engine->lights.push_back(new DirectionalLight(
                    1.0f,
                    50.0f,
                    PulseEngine::Vector3(0.0f,0.0f,0.0f),
                    PulseEngine::Vector3(0.0f, 0.0f, 0.0f),
                    PulseEngine::Color(1.0f, 1.0f, 1.0f),
                    1.0f,
                    10.0f
                ));

            }            

            if(ImGui::MenuItem("Import"))
            {
                char filePath[MAX_PATH] = "";

                OPENFILENAME ofn = {};
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = nullptr;
                ofn.lpstrFile = filePath;
                ofn.nMaxFile = sizeof(filePath);
                ofn.lpstrFilter = "3D Models\0*.obj;*.fbx;*.glb\0All\0*.*\0";
                ofn.nFilterIndex = 1;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn))
                {
                    // ResetWorkingDirectoryToExe();
                    std::cout << "Selected file: " << filePath << std::endl;
                    std::string name = std::string(filePath).substr(std::string(filePath).find_last_of("/\\") + 1);
                    std::string fileStr = editor->currentDir.string() + "/" + name + ".pmesh"; 

                    std::string prefix = "PulseEngineEditor/";
                    std::string guidPath;
                    if (fileStr.find(prefix) == 0)
                    {
                        guidPath = fileStr.substr(prefix.length());
                    }
                    else
                    {
                        guidPath = fileStr; 
                    }

                    std::cout << "GUID Path: " << guidPath << std::endl;
                    std::cout << "Name : " << name << std::endl;
                    std::cout << "fileStr : " << fileStr << std::endl;

                    std::filesystem::current_path(FileManager::workingDirectory);

                    GuidReader::InsertIntoCollection(guidPath);

                    std::ofstream guidFile(fileStr);
                    guidFile.close();
                }
            }
            ImGui::EndMenu();
        }

        // === VIEW MENU ===
        if (ImGui::BeginMenu("View"))
        {

            for (auto& win : editor->windowStates)
            {
                if (ImGui::MenuItem(win.first.c_str()))
                {
                    win.second = !win.second;
                }
            }

            ImGui::EndMenu();
        }
        // === HELP MENU ===
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) { /* TODO: About popup */ }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // === NEW MAP POPUP ===
    if (requestOpenNewMapPopup)
    {
        ImGui::OpenPopup("NewMapPopup");
        requestOpenNewMapPopup = false;
    }

    if (ImGui::BeginPopupModal("NewMapPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter new map name:");
        ImGui::InputText("##mapname", newMapName, IM_ARRAYSIZE(newMapName));

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::string mapPath = "Scenes/";
            mapPath += newMapName;

            engine->actualMapPath = mapPath;
            engine->actualMapName = newMapName;

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TopBar::BuildGameToWindow(PulseEngineBackend *engine, InterfaceEditor* editor)
{
    std::cout << "=== Building game to window ===" << std::endl;

    // Create the build coroutine
    std::unique_ptr<Coroutine> buildCoroutine = std::make_unique<BuildGameCoroutine>();

    // Safely cast raw pointer from base Coroutine* to derived BuildGameCoroutine*
    BuildGameCoroutine* co = dynamic_cast<BuildGameCoroutine*>(buildCoroutine.get());
    if (co)
    {
        co->engine = engine;
        co->editor = editor;
        co->topbar = this;
    }
    else
    {
        // Handle error: cast failed
        std::cerr << "Failed to cast Coroutine* to BuildGameCoroutine*\n";
    }


    // Add the coroutine to the editor
    engine->coroutineManager->Add(std::move(buildCoroutine));
}

void TopBar::CompileUserScripts(InterfaceEditor * editor, std::string output )
{
    std::cout << "Compiling...\n";
                    //Lets work on the custom files scripts now
                std::string compiler = "g++";
                std::string stdVersion = "-std=c++17";
                std::string defines = "-DBUILDING_DLL -DWINDOW_PULSE_EXPORT";
                std::string flags = "-shared -Wall -g -mconsole " + defines;
                std::string includeDirs = R"(-IUserScripts -Idist\src\PulseEngine\CustomScripts -Idist\include -Idist\src -Idist/src/dllexport -Ldist)";
                std::string libs = "-lPulseEngine";

                // Gather source files
                std::string sources;
                for (const auto& entry : std::filesystem::directory_iterator("UserScripts"))
                {
                    if (entry.path().extension() == ".cpp")
                    {
                        sources += entry.path().string() + " ";
                    }
                }
            
                // Final compilation command
                std::string compileCommand = compiler + " " + stdVersion +
                                             " -o " + output + " " + sources +
                                             includeDirs + " " + libs + " " + flags;
            
                std::cout << "Compiling with command:\n" << compileCommand << "\n";
            
                int result = system(compileCommand.c_str());

                editor->ChangePorgressIn("Building Game", 1.0f);
                if (result != 0)
                {
                    std::cerr << "Compilation failed.\n";
                }
                else
                {
                    std::cout << "DLL generated: " << output << "\n";
                }
}

void TopBar::GenerateExecutableForWindow(PulseEngineBackend * engine)
{
    std::cout << "=== Creating the executable for window ===" << std::endl;

    nlohmann::json_abi_v3_12_0::json engineConfig = FileManager::OpenEngineConfigFile(engine);
    std::string gameName = engineConfig["GameData"]["Name"].get<std::string>();
    std::string gameVersion = engineConfig["GameData"]["version"].get<std::string>();

    std::string defineGameName = "-DGAME_NAME=\\\"" + gameName + "\\\"";
    std::string defineGameVersion = "-DGAME_VERSION=\\\"" + gameVersion + "\\\"";

    std::string compileCommand =
        "g++ -Idist/src -Idist/include dist/main.cpp "
        "-Ldist/Build -lPulseEngine "
        "-LC:/path/to/glfw/lib -lglfw3 -lgdi32 -lopengl32 "
        "-DWINDOW_PULSE_EXPORT " +
        defineGameName + " " + defineGameVersion + " "
                                                   "-o Build/Game.exe";
    std::cout << "Compile command: " << compileCommand << std::endl;
    system(compileCommand.c_str());

    std::string renameCmd = "rename \"Build\\game.exe\" \"" + gameName + ".exe\"";
    system(renameCmd.c_str());
}

void TopBar::CopyDllForWindow()
{
    std::cout << "=== Copying PulseEngine.dll For window ===" << std::endl;
    system("xcopy dist\\PulseEngine.dll \"Build\" /Y");
}

void TopBar::CopyAssetForWindow()
{
    std::cout << "=== Copying assets For window ===" << std::endl;
    system("xcopy PulseEngineEditor \"Build/assets\" /E /I /Y");
}

void TopBar::GenerateWindowsDirectory()
{
    system("echo === Generating folders for Window ===");
    system("if not exist Build mkdir Build");
    system("if not exist \"Build/assets\" mkdir \"Build/assets\"");
    system("if not exist \"Build/Logs\" mkdir \"Build/Logs\"");
}
