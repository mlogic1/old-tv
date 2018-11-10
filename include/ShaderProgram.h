#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>

#include <iostream>
#include <fstream>

class ShaderProgram
{
    public:
        ShaderProgram(const std::string vertexShaderSrcFileName, const std::string fragmentShaderSrcFileName);
        virtual ~ShaderProgram();

        void UseShader();

        unsigned int const GetID();

    private:
        // shader ID
        unsigned int SHADER_ID;

        unsigned int VERTEX_SHADER;
        unsigned int FRAGMENT_SHADER;

        const std::string SHADER_DIRECTORY = "shaders/";

        // vertex and fragment shader source code
        std::string m_vertexShaderSource;
        std::string m_fragmentShaderSource;

        void compileVertexShader();
        void compileFragmentShader();
        void linkShaders();
};

#endif // SHADERPROGRAM_H
