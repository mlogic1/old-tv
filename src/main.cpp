#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ShaderProgram.h"
#include "Rectangle.h"

#ifdef DEBUGMODE
    const bool DEBUG_MODE = true;
#else
    const bool DEBUG_MODE = false;
#endif // DEBUGTEST


using namespace std;

/* starting config consts */
const char* WINDOW_NAME = "Old TV";
const unsigned int RESOLUTION_W = 1280;
const unsigned int RESOLUTION_H = 800;

/* noise vertices array */
vector<float> linesVertices;

/* GLFW/GLAD functions */
int initGlad(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }else
    {
        return 1;
    }
}

void initGlfw(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow *initWindow(){
    GLFWwindow *window = glfwCreateWindow(RESOLUTION_W, RESOLUTION_H, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onWindowResized);
    glfwSetKeyCallback(window, onKey);
    return window;
}

/* context */
GLFWwindow *window = nullptr;

/* rectangles */
Rect *tvRect;
Rect *noiseRect;

/* timers */
float deltaTime, currentFrame, lastFrame;
int frameCount = 0;
float tickTime = 0.0f;
float fps = 0.0f;

int main(int argc, char* argv[]){
    initGlfw();
    window = initWindow();
    if (window == NULL)
        return -1;
    if (initGlad() == -1)
        return -1;

    ShaderProgram shaderTV("vertexshader.vs", "fragmentShaderTV.fs");
    ShaderProgram shaderNoise("vertexshader.vs", "fragmentShaderNoise.fs");


    // rectangle data
    std::vector<float> vertices = {
        // position          // texture     // stride = 5 floats
         0.8f, 0.8f,  0.0f,   1.0f, 1.0f,
         0.8f, -0.8f, 0.0f,   1.0f, 0.0f,
        -0.8f, -0.8f, 0.0f,   0.0f, 0.0f,
        -0.8f, 0.8f,  0.0f,   0.0f, 1.0f
    };

    // rectangle indices
    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3
    };

    try{
        tvRect = new Rect(vertices, 5 * sizeof(float), GL_TRIANGLES, indices, "tv.png");
    }catch(std::string error){
        std::cout<<"Rect error: "<<error<<std::endl;
        return -1;
    }

    for(float i= 0.68f; i>=-0.55;i-=0.01f){
            for(float j=-0.70f;j<=0.45;j+=0.01f){
                linesVertices.push_back(j);
                linesVertices.push_back(i);
                linesVertices.push_back(1.0f);
        }
    }

    try{
        noiseRect = new Rect(linesVertices, 3 * sizeof(float), GL_LINE_STRIP);
    }catch(std::string error){
        std::cout<<error<<std::endl;
        return -1;
    }

    // make lines a little thicker
    glLineWidth(2.4f);

    // lock framerate to monitor refresh rate
    glfwSwapInterval(1);

    while(!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frameCount++;

        if ( currentFrame - tickTime >= 1.0){
            fps = frameCount;
            frameCount = 0;
            tickTime = currentFrame;
        }

        if (DEBUG_MODE){
            // replaces window title with delta time and fps
            char deltaStr[64];
            sprintf(deltaStr, "Delta: %f - FPS: %f", deltaTime, fps);
            glfwSetWindowTitle(window, deltaStr);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // noise rectangle and logic
        shaderNoise.UseShader();
        float rand_seed = sin(currentFrame) / 2.0f + 0.5f;
        int location  = glGetUniformLocation(shaderNoise.GetID(), "seed");
        glUniform1f(location, rand_seed);
        noiseRect->updateAndRender(deltaTime);

        // TV graphics render
        shaderTV.UseShader();
        tvRect->updateAndRender(deltaTime);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // clean up
    delete tvRect;
    delete noiseRect;
    glfwTerminate();

    return 0;
}
