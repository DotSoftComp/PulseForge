#ifndef ENTITYEDITOR_H
#define ENTITYEDITOR_H

#include "Common/common.h"
#include "Common/dllExport.h"

#include  "PulseEngine/ModuleLoader/IModuleInterface/IModuleInterface.h"

class Mesh;

class EntityEditor : public IModuleInterface
{
    public:
        std::string GetName() const override;
        std::string GetVersion() const override;
        void Initialize() override;
        void Shutdown() override;
        void Render() override;

        void ManageCamera();

        void EntityMeshesManager();

        void AddMeshToEntity();

        void EntityScriptManager();

        void AddScriptToEntity();

        void AddScriptOneNTITY();

        void NewFileClicked(const ClickedFileData& data);
        
        Camera* cam;
        Entity* selectedEntity;
        Mesh* selectedData = nullptr;
        Shader* forRender;
        PulseEngine::Vector2 previewData;
        float distCam = 5.0f;
};

#endif