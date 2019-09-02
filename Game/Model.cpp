#include "Model.h"

Model::Model(const char* modelPath)
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
                vertices.push_back(v);
            }
            // Read texture coordinates
            else if(line.substr(0,3) == "vt ")
            {
                std::istringstream s(line.substr(3));
                glm::vec2 v; 
                s >> v.x;
                s >> v.y;
                texCoods.push_back(v);
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
                    vertexIndices.push_back(a); uvIndices.push_back(b); normalIndices.push_back(c);
                }
            }
        }
        modelIStream.close();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int),
            &vertexIndices[0], GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(3 * sizeof(glm::vec3)));
        // Vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(6 * sizeof(glm::vec3)));

        glBindVertexArray(0);
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "Model file not successfully read!" << e.what() << std::endl;
    }
}

void Model::Draw(Shader& shader)
{
    // glBindTexture(GL_TEXTURE_2D, textures.id);
    // glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}