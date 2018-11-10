#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glad/glad.h>

#include <iostream>
#include <algorithm>
#include <vector>

class Rect
{
    public:
        Rect(std::vector<float>vertices, unsigned const int stride, const int DRAW_MODE, std::vector<unsigned int> indices = {}, std::string textureFileName = "");
        ~Rect();

        void updateAndRender(float deltaTime);

    private:

        // vertex array object
        unsigned int VAO;
        // vertex buffer object
        unsigned int VBO;
        // element buffer object
        unsigned int EBO;

        // draw mode
        const int GL_DRAW_MODE;
        unsigned int pointsToDraw = 0;   // can be either indice count or number of vertexes

        // vertex data
        float *vertices;
        bool hasIndices = false;
        unsigned int *indices;

        // texture
        bool hasTexture = false;
        unsigned int texture;
        const std::string TEXTURE_DIRECTORY = "textures/";
        void loadTexture2D(std::string textureFileName);
};

#endif // RECTANGLE_H

