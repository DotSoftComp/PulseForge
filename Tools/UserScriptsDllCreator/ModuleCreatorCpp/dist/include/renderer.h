#ifndef RENDERER_H
#define RENDERER_H

#include <glfw3.h>
#include "shader.h"

class Renderer {
public:
    Renderer();
    void clear();
    void render(Shader& shader);

private:
    // Ajoute ici des membres comme les buffers, les VAO/VBO pour l'affichage
    unsigned int VAO, VBO;
};

#endif // RENDERER_H
