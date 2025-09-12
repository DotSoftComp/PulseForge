#include "CustomScript.h"
#include "Common/common.h"


#include "shader.h"
#include "camera.h"
#include "PulseEngine/core/PulseEngineBackend.h" 
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/MaterialManager.h"

#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"

#ifdef ENGINE_EDITOR
    #include "PulseEngineEditor/InterfaceEditor/InterfaceEditor.h"
#endif

#include <vector>
#include <cmath>



int main()
{
    CustomScript cust;
    cust.OnStart();
    PulseEngineBackend* engine = new PulseEngineBackend();
    if (engine->Initialize() != 0)
    {
        std::cerr << "Engine failed to initialize" << std::endl;
        return -1;
    }
    
    EDITOR_ONLY(
        InterfaceEditor* editor = new InterfaceEditor(engine);
    )
    

    // === Boucle de rendu ===
    while (engine->IsRunning())
    {
        engine->PollEvents();
        engine->Update();

        cust.OnUpdate();
        engine->RenderShadow();
        engine->Render();
        EDITOR_ONLY(
            editor->Render(engine);
        )

        glfwSwapBuffers(engine->GetWindowContext()->GetGLFWWindow());
        glfwPollEvents();
    }
    engine->Shutdown();

    EDITOR_ONLY(
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    )
    

    return 0;
}