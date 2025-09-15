#include "AccountViewer.h"
#include "PulseEngineEditor/InterfaceEditor/Account/Account.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.h"

std::string AccountViewer::GetName() const
{
    return std::string("Account viewer");
}

std::string AccountViewer::GetVersion() const
{
    return std::string("v0.1.0");
}

void AccountViewer::Initialize()
{
    acc = PulseEngineInstance->GetAccountLoggedIn();
}

void AccountViewer::Shutdown()
{
}

void AccountViewer::Render()
{
    PulseInterfaceAPI::OpenWindow("Account viewer");

    PulseInterfaceAPI::BeginChild("account window child left", PulseEngine::Vector2(PulseInterfaceAPI::GetActualWindowSize().x * 0.5f - 20.0f, PulseInterfaceAPI::GetActualWindowSize().y ), true);

    PulseInterfaceAPI::EndChild();

    PulseInterfaceAPI::SameLine();

    PulseInterfaceAPI::BeginChild("account window child right", PulseEngine::Vector2(PulseInterfaceAPI::GetActualWindowSize().x * 0.5f -20.0f, PulseInterfaceAPI::GetActualWindowSize().y ), true);
    PulseInterfaceAPI::WriteText(std::string("Connected has : ") + acc->GetName());
    PulseInterfaceAPI::WriteText(acc->GetEmail());
    PulseInterfaceAPI::EndChild();

    PulseInterfaceAPI::CloseWindow();   
}

extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new AccountViewer();
}