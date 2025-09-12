#pragma once

#include "PulseEngine/CustomScripts/IScripts.h"
#include "Common/dllExport.h"

class PULSE_ENGINE_DLL_API scriptt : public IScript
{
    public:
        scriptt()
        {
            // if you need to expose variables, do it here with these macro : 
            // AddExposedVariable(EXPOSE_VAR(speed, INT));
            // REGISTER_VAR(speed); both are needed to make the variable exposed in the editor
        }
        void OnStart() override;
        void OnUpdate() override;
        void OnRender() override;
        const char* GetName() const override;
};

