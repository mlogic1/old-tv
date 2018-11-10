#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string vertexShaderSrcFileName, const std::string fragmentShaderSrcFileName)
{
    std::string vertexShaderFilePath = SHADER_DIRECTORY + vertexShaderSrcFileName;
    std::string fragmentShaderFilePath = SHADER_DIRECTORY + fragmentShaderSrcFileName;

    std::ifstream vertexInStream(vertexShaderFilePath);
    std::ifstream fragmentInStream(fragmentShaderFilePath);

    std::string line;
    std::string tmp = "";

    // load vertex shader code from file
    std::cout<<"Loading vertex shader: "<<vertexShaderFilePath<<std::endl;
    if(vertexInStream.is_open()){
        while (vertexInStream.good()){
            std::getline(vertexInStream, line);
            tmp += line + "\n";
        }
    m_vertexShaderSource = tmp;
    }else{
        std::string error = "Missing file: ";
        error += vertexShaderFilePath;
        throw error;
    }
    vertexInStream.close();

    tmp = "";
    // load fragment shader code from file
    std::cout<<"Loading fragment shader: "<<fragmentShaderFilePath<<std::endl;
    if(fragmentInStream.is_open()){
        while (fragmentInStream.good()){
            std::getline(fragmentInStream, line);
            tmp += line + "\n";
        }
    m_fragmentShaderSource = tmp;
    }else{
        std::string error = "Missing file: ";
        error += fragmentShaderFilePath;
        throw error;
    }
    fragmentInStream.close();

    // create shaders
    VERTEX_SHADER = glCreateShader(GL_VERTEX_SHADER);
    FRAGMENT_SHADER = glCreateShader(GL_FRAGMENT_SHADER);

    // compile shaders
    compileVertexShader();
    compileFragmentShader();

    linkShaders();

    // clean up
    glDeleteShader(VERTEX_SHADER);
    glDeleteShader(FRAGMENT_SHADER);

}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(SHADER_ID);
}

unsigned int const ShaderProgram::GetID(){
    return SHADER_ID;
}

void ShaderProgram::UseShader(){
    glUseProgram(SHADER_ID);
}

void ShaderProgram::compileVertexShader(){
    int success = false;    // compilation status flag

    const char *vertexShaderSource = m_vertexShaderSource.c_str();
    glShaderSource(VERTEX_SHADER, 1, &vertexShaderSource, NULL);
    glCompileShader(VERTEX_SHADER);

    glGetShaderiv(VERTEX_SHADER, GL_COMPILE_STATUS, &success);
    if (!success){
        std::string error = "Failed to compile vertex shader! Log: ";
        GLchar infoLog[1024];
        glGetShaderInfoLog(VERTEX_SHADER, 1024, NULL, infoLog);
        error += infoLog;
        throw error;
    }
}

void ShaderProgram::compileFragmentShader(){
    int success = false;

    const char* fragmentShaderSource = m_fragmentShaderSource.c_str();
    glShaderSource(FRAGMENT_SHADER, 1, &fragmentShaderSource, NULL);
    glCompileShader(FRAGMENT_SHADER);

    glGetShaderiv(FRAGMENT_SHADER, GL_COMPILE_STATUS, &success);
    if (!success){
        std::string error = "Failed to compile fragment shader! Log: ";
        GLchar infoLog[1024];
        glGetShaderInfoLog(FRAGMENT_SHADER, 1024, NULL, infoLog);
        error += infoLog;
        throw error;
    }
}

void ShaderProgram::linkShaders(){
    int success = false;
    SHADER_ID = glCreateProgram();
    glAttachShader(SHADER_ID, VERTEX_SHADER);
    glAttachShader(SHADER_ID, FRAGMENT_SHADER);

    glLinkProgram(SHADER_ID);

    glGetProgramiv(SHADER_ID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(SHADER_ID, 512, NULL, infoLog);
        std::string error = "ERROR::SHADER::PROGRAM::LINKING_FAILED!\n";
        error += infoLog;
        throw error;
    }
    std::cout<<"Shader program successfully linked"<<std::endl;
}
