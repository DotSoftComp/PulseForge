#include "Common/common.h"
#include "MaterialManager.h"
#include "Material.h"
#include <fstream>
#include "json.hpp"
#include "PulseEngine/core/GUID/GuidReader.h"

std::unordered_map<std::string, Material*> MaterialManager::materials;


Material* MaterialManager::loadMaterial(const std::string &filePath)
{

    // integrate "shared asset system" here
    // -> hash content into a .mpak file during the building in game engine
    // -> use the hashed filePath to load the material

    #ifdef ENGINE_EDITOR
    //here we will use the std::ifstream system, because of engine
    #else
    //here, we are using the shared asset system to get the hashed and turn it into a readable json
    #endif
    std::ifstream file(filePath);
    nlohmann::json jsonData;
    file >> jsonData;

    
    if(materials[jsonData["name"]]) return materials[jsonData["name"]];
    Material* material = new Material("new material", new Shader(std::string(ASSET_PATH) + "shaders/basic.vert", std::string(ASSET_PATH) + "shaders/basic.frag"));
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open material file: " + filePath);
    }


    if(jsonData.contains("name"))
    {
        material->SetName(jsonData["name"]);
    }
    if(jsonData.contains("color"))
    {
        if (jsonData["color"].is_array() && jsonData["color"].size() == 3)
        {
            material->color = PulseEngine::Vector3(
                jsonData["color"][0].get<float>(),
                jsonData["color"][1].get<float>(),
                jsonData["color"][2].get<float>()
            );
        }
        else
        {
            throw std::runtime_error("Invalid format for 'color' in material file: " + filePath);
        }
    }
    if(jsonData.contains("specular"))
    {
        material->specular = jsonData["specular"].get<float>();
    }
    if(jsonData.contains("roughness"))
    {
        material->roughness = jsonData["roughness"].get<float>();
    }

    if(jsonData.contains("albedo"))
    {
        Texture* albedoTexture = new Texture(jsonData["albedo"].get<std::string>());
        material->SetTexture("albedo", std::shared_ptr<Texture>(albedoTexture));
    }

    materials[material->GetName()] = material;
    file.close();
    GuidReader::InsertIntoCollection(filePath);
    return material;
}

Material *MaterialManager::getMaterial(const std::string &materialName)
{
    if(materials[materialName]) return materials[materialName];
    return nullptr;
}
