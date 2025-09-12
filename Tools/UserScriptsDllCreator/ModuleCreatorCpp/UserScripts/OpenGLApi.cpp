#include "PulseEngine/core/WindowContext/WindowContext.h"
#include "PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.h"
#include "PulseEngine/core/Graphics/IGraphicsApi.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "pulseEngine/core/Inputs/Mouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "OpenGLApi.h"

bool OpenGLAPI::InitializeApi(const char* title, int* width, int* height, PulseEngineBackend* engine)
{
    std::cout << "starting to init opengl" << std::endl;
        this->engine = engine;
    if (!glfwInit())
    {
        EDITOR_ERROR("Failed to initialize GLFW.");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->width = width;
    this->height = height;

    window = glfwCreateWindow(*width, *height, title, nullptr, nullptr);
    if (!window)
    {
        EDITOR_ERROR("Failed to create GLFW window.");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, PulseEngineBackend::FramebufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        EDITOR_ERROR("Error while initializing GLAD.");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    glfwMakeContextCurrent(window);

        // Create texture to render to
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    // Create Renderbuffer for depth and stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fboWidth, fboHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EDITOR_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::cout << "OpenGL API initialized successfully." << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void OpenGLAPI::ShutdownApi()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void OpenGLAPI::PollEvents() const
{
    std::cout << "Polling OpenGL events..." << std::endl;
    glfwSetCursorPosCallback(window, MouseInput::MouseCallback);
    glfwSetScrollCallback(window, MouseInput::ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void OpenGLAPI::SwapBuffers() const
{
    glfwSwapBuffers(window);
}

bool OpenGLAPI::ShouldClose() const
{
    std::cout << "Checking if OpenGL window should close..." << std::endl;
    if (!window)
    {
        EDITOR_ERROR("GLFW window is not initialized.");
        return true; // Consider it closed if window is null
    }
    std::cout << glfwWindowShouldClose(window) << std::endl;
    
    return glfwWindowShouldClose(window);
}

void OpenGLAPI::SetWindowSize(int width, int height) const
{
    glfwSetWindowSize(window, width, height);
}

void OpenGLAPI::SetWindowTitle(const char *title) const
{
    glfwSetWindowTitle(window, title);
}

void *OpenGLAPI::GetNativeHandle() const
{
    return static_cast<void*>(window);
}

void OpenGLAPI::StartFrame() const
{
    std::cout << "Starting OpenGL frame..." << std::endl;
    EDITOR_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, fboWidth, fboHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    )
    IN_GAME_ONLY(
        glfwGetFramebufferSize(window, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, *width, *height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
    )
}

void OpenGLAPI::EndFrame() const
{
    std::cout << "Ending OpenGL frame..." << std::endl;
    EDITOR_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Go back to default framebuffer
        glViewport(0, 0, 1920, 1080); // Reset to default screen size
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    )
}

unsigned int OpenGLAPI::CreateShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Charger et compiler les shaders
    std::cout << "Loading shader from: " << vertexPath << " and " << fragmentPath << std::endl;
    std::string vertexCode = LoadShaderCode(vertexPath);
    std::string fragmentCode = LoadShaderCode(fragmentPath);
    
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
    unsigned int shaderID = LinkProgram(vertexShader, fragmentShader);
    std::cout << "Shader program linked with ID: " << shaderID << std::endl;
    return shaderID;
}

std::string OpenGLAPI::LoadShaderCode(const std::string& path)
{
    std::ifstream shaderFile(path);
    if (!shaderFile.is_open())
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier shader : " << path << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

unsigned int OpenGLAPI::CompileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erreur de compilation du "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader:\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int OpenGLAPI::LinkProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erreur de linkage du shader program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


OpenGLAPI::~OpenGLAPI()
{
    Shutdown();
}


extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new OpenGLAPI();
}


// graphicsAPI.cpp
__declspec(dllexport) void InitializeGraphics()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}