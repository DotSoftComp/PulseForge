#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <string>
#include <unordered_map>
#include "dllExport.h"

class Material;

class MA_DLL_API MaterialManager 
{
public:

    static Material* loadMaterial(const std::string& filePath);
    static Material* getMaterial(const std::string& materialName);
    static void unloadMaterial(const std::string& materialName);
private:
    static std::unordered_map<std::string, Material*> materials;
};


#endif