#include "Model.h"

#include "vboindexer.cpp"

Model::Model(const char* modelPath, const char* texturePath, Shader theShader, glm::vec3 startPosition, glm::vec3 startScale, bool hasCol)
{
    std::ifstream modelIStream;

    // Ensure ifstream objects can throw exceptions.
    // This causes crash here because the while loop is expected to run to the end and "fail".
    // modelIStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    float* textureArray;
    float* normalsArray;
    
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
                textureArray = new float[verticesPos.size()*2];
                normalsArray = new float[verticesPos.size()*3];
                break;
                // std::istringstream s(line.substr(2));
                // unsigned int a, b, c;
                // for(int i = 0; i < 3; i++)
                // {
                //     s >> a;
                //     s.ignore(1); 
                //     s >> b; 
                //     s.ignore(1);
                //     s >> c;
                //     a--;
                //     b--;
                //     c--;
                //     vertexIndices.push_back(a); uvIndices.push_back(b); normalIndices.push_back(c);
                // }
            }
        }

        while(getline(modelIStream, line))
        {
            if (line.substr(0,2) != "f ")
            {
                continue;
            }

            std::vector<std::string> currentLine = Split(line, ' ');
            std::vector<std::string> vertex1 = Split(currentLine[1], '/');
            std::vector<std::string> vertex2 = Split(currentLine[2], '/');
            std::vector<std::string> vertex3 = Split(currentLine[3], '/');

            ProcessVertex(vertex1, textureArray, normalsArray);
            ProcessVertex(vertex2, textureArray, normalsArray);
            ProcessVertex(vertex3, textureArray, normalsArray);
        }

        modelIStream.close();

        glm::vec3 maxVec;
        glm::vec3 minVec;

        for(int i = 0; i < verticesPos.size(); i++)
        {
            vertices[i].vertice = verticesPos[i];
            vertices[i].normal = glm::vec3(normalsArray[i*3],normalsArray[i*3+1],normalsArray[i*3+2]);
            vertices[i].texCoord = glm::vec2(textureArray[i*2],textureArray[i*2+1]);
        }

        for(int i = 0; i < vertexIndices.size(); i++)
        {
            // vertices.push_back(Vertex(verticesPos[i], normals[i], texCoords[i]));
            
            // Get the max and min values for generating the box collider around the model
            if(verticesPos[i].x > maxVec.x)
            {
                maxVec.x = verticesPos[i].x;
            }
            if(verticesPos[i].y > maxVec.y)
            {
                maxVec.y = verticesPos[i].y;
            }
            if(verticesPos[i].z > maxVec.z)
            {
                maxVec.z = verticesPos[i].z;
            }

            if(verticesPos[i].x < minVec.x)
            {
                minVec.x = verticesPos[i].x;
            }
            if(verticesPos[i].y < minVec.y)
            {
                minVec.y = verticesPos[i].y;
            }
            if(verticesPos[i].z < minVec.z)
            {
                minVec.z = verticesPos[i].z;
            }
        }

        if(hasCol)
        {
            float colliderVertices[24] = {
                maxVec.x, maxVec.y, maxVec.z,
                maxVec.x, maxVec.y, minVec.z,
                maxVec.x, minVec.y, maxVec.z,
                maxVec.x, minVec.y, minVec.z,
                minVec.x, maxVec.y, maxVec.z,
                minVec.x, maxVec.y, minVec.z,
                minVec.x, minVec.y, maxVec.z,
                minVec.x, minVec.y, minVec.z
            };
            collider = new Collider(colliderVertices);
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

        shader = new Shader();
        *shader = theShader;

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
    delete(shader);
    // delete(collider);
}

std::vector<std::string> Model::Split(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

void Model::ProcessVertex(std::vector<std::string> vertexData, float* textureArray, float* normalsArray)
{
    int currentVertexPointer = std::stoi(vertexData[0]) - 1;
    vertexIndices.push_back(currentVertexPointer);
    glm::vec2 currentTexture = texCoords[std::stoi(vertexData[1]) - 1];
    textureArray[currentVertexPointer*2] = currentTexture.x;
    textureArray[currentVertexPointer*2+1] = 1 - currentTexture.y;
    glm::vec3 currentNormal = normals[std::stoi(vertexData[2]) - 1];
    normalsArray[currentVertexPointer*3] = currentNormal.x;
    normalsArray[currentVertexPointer*3+1] = currentNormal.y;
    normalsArray[currentVertexPointer*3+2] = currentNormal.z;
    vertices.push_back(Vertex(verticesPos[currentVertexPointer], currentNormal, currentTexture));
}

void Model::Draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    shader->Use();
    shader->SetInt("myTexture", 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    if(needToRotate)
    {
        model = glm::rotate(model, desiredRot, glm::vec3(0.0f, 1.0f, 0.0f));
    } 
    shader->SetMat4("model", model);

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

void Model::Rotate(float newAngle)
{
    if(newAngle != lastGivenAngle)
    {
        needToRotate = true;
        desiredRot = newAngle;
    }
    lastGivenAngle = newAngle;
}