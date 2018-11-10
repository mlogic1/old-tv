#include "Rectangle.h"

#include "stb_image.h"

Rect::Rect(std::vector<float>vertices, unsigned const int stride, const int DRAW_MODE, std::vector<unsigned int> indices, std::string textureFileName):GL_DRAW_MODE(DRAW_MODE)
{
    unsigned const int verticesCount = vertices.size();
    unsigned const int indicesCount = indices.size();
    unsigned const int verticesByteSize = verticesCount * sizeof(float);
    unsigned const int indicesByteSize = indicesCount * sizeof(float);

    if(verticesCount == 0){
        std::string error = "Warning! Instantiating a rectangle with 0 vertices!";
        throw error;
    }

    // initialize vertices
    this->vertices = new float[verticesCount];

    // set up number of vertices to draw
    this->pointsToDraw = verticesCount;

    // populate vertices
    int index = 0;
    for_each(vertices.begin(), vertices.end(), [this, &index](float vertex){
        this->vertices[index] = vertex;
        index++;
    });

    if (indicesCount>0){
        this->hasIndices = true;
        this->indices = new unsigned int[indicesCount];
        // set up number of indices to draw
        this->pointsToDraw = indicesCount;
        index = 0;
        for_each(indices.begin(), indices.end(), [this, &index](unsigned int indice){
            this->indices[index] = indice;
            index++;
        });
    }


    // check if rectangle should display a texture
    if (textureFileName != "")
        this->hasTexture = true;

    // initialize vertex array object and vertex array buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    if(hasIndices){
        glGenBuffers(1, &EBO);
    }

    // buffer data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if(hasIndices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, verticesByteSize, this->vertices, GL_STATIC_DRAW);

    if(hasIndices)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesByteSize, this->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    if(hasTexture){
        // load texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        loadTexture2D(textureFileName);
    }

    // cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Rect::~Rect()
{
    delete[] vertices;
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Rect::loadTexture2D(std::string textureFileName){
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    textureFileName = TEXTURE_DIRECTORY + textureFileName;
    const char* cTextureFileName = textureFileName.c_str();
    int width, height, nOfChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(cTextureFileName, &width, &height, &nOfChannels, 0);

    if(!data){
        std::string error = "Unable to load image: ";
        error += textureFileName;
        throw error;
    }

    // texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // enables transparency on png texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // clean up
    stbi_image_free(data);
}

void Rect::updateAndRender(float deltaTime){
    glBindVertexArray(this->VAO);

    if(this->hasTexture){
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }else{
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if(this->hasIndices){
        glDrawElements(this->GL_DRAW_MODE, this->pointsToDraw, GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(this->GL_DRAW_MODE, 0, this->pointsToDraw);
    }

}
