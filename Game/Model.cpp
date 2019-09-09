#include "Model.h"

Model::Model(const char* modelPath, const char* texturePath, glm::vec3 startPosition, glm::vec3 startScale)
{
    std::ifstream modelIStream;

    // Ensure ifstream objects can throw exceptions.
    // This causes crash here because the while loop is expected to run to the end and "fail".
    // modelIStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try 
    {
        // Open files.
        modelIStream.open(modelPath);
        std::string line;
        while(getline(modelIStream, line))
        {
            // Read vertices
            if(line.substr(0,2) == "v ")
            {
                std::istringstream s(line.substr(2));
                glm::vec3 v; 
                s >> v.x;
                s >> v.y;
                s >> v.z;
                verticesPos.push_back(v);
            }
            // Read texture coordinates
            else if(line.substr(0,3) == "vt ")
            {
                std::istringstream s(line.substr(3));
                glm::vec2 v; 
                s >> v.x;
                s >> v.y;
                texCoords.push_back(v);
            }
            // Read vertex normals
            else if(line.substr(0,3) == "vn ")
            {
                std::istringstream s(line.substr(3));
                glm::vec3 v; 
                s >> v.x;
                s >> v.y;
                s >> v.z;
                normals.push_back(v);
            }
            // Read face elements
            // Elements are formatted this way in the obj file:
            // 1/8/6 4/20/6 6/15/6
            // That is why we need to read a number, then discard the slash. 
            // Also we need to substact 1 from the numbers, because indices are
            // range[1, 8] in obj files but OpenGL expects it to be range[0, 7].
            else if (line.substr(0,2) == "f ")
            {
                std::istringstream s(line.substr(2));
                unsigned int a, b, c;
                for(int i = 0; i < 3; i++)
                {
                    s >> a;
                    s.ignore(1); 
                    s >> b; 
                    s.ignore(1);
                    s >> c;
                    a--;
                    b--;
                    c--;
                    vertexIndices.push_back(a); uvIndices.push_back(b); normalIndices.push_back(c);
                }
            }
        }
        modelIStream.close();

        for(int i = 0; i < vertexIndices.size(); i++)
        {
            vertices.push_back(Vertex(verticesPos[i], normals[i], texCoords[i]));
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0], GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, &normals[0], GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, texCoods.size() * sizeof(float) * 2, &texCoods[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int),
            &vertexIndices[0], GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // Vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindVertexArray(0);

        texture = new Texture(texturePath);

        position = startPosition;
        scale = startScale;
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "Model file not successfully read!" << e.what() << std::endl;
    }
}

Model::~Model()
{
    delete(texture);
}

void Model::Draw(Shader& shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    shader.Use();
    shader.SetInt("myTexture", 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    shader.SetMat4("model", model);

    glBindVertexArray(VAO);

    // Draw mesh
    int size;  
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);  
    glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::SetPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

void Model::Move(glm::vec3 direction, float speed)
{
    position += glm::normalize(direction) * speed;
}