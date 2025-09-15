#ifndef CUSTOMMODULE
#define CUSTOMMODULE

#include "Common/common.h"
#include "Common/dllExport.h"

#include  "PulseEngine/ModuleLoader/IModuleInterface/IModuleInterface.h"

class PULSE_ENGINE_DLL_API Viewport : public IModuleInterface
{
    public:
        std::string GetName() const override;

        std::string GetVersion() const override;

        void Initialize() override;

        void Shutdown() override;

        void Render() override;
};

#endif