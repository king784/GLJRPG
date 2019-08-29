#include "Model.h"

Model::Model(const char* modelPath)
{
    std::ifstream modelIStream;
    int linenum = 0;

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
            linenum++;
            std::cout << linenum << std::endl;
            if(line.substr(0,2) == "v ")
            {
                std::istringstream s(line.substr(2));
                glm::vec3 v; 
                s >> v.x;
                s >> v.y;
                s >> v.z;
                vertices.push_back(v);
            }
            else if(line.substr(0,3) == "vt ")
            {
                std::istringstream s(line.substr(3));
                glm::vec2 v; 
                s >> v.x;
                s >> v.y;
                texCoods.push_back(v);
            }
            else if(line.substr(0,3) == "vn ")
            {
                std::istringstream s(line.substr(3));
                glm::vec3 v; 
                s >> v.x;
                s >> v.y;
                s >> v.z;
                normals.push_back(v);
            }
            // THIS IS NEXT!
            else if (line.substr(0,2) == "f ")
            {
                std::istringstream s(line.substr(2));
                GLushort a,b,c;
                s >> a; s >> b; s >> c;
                a--; b--; c--;
                indices.push_back(a); indices.push_back(b); indices.push_back(c);
            }
        }
        modelIStream.close();

        for(int i = 0; i < indices.size()-1; i++)
        {
            std::cout << indices[i] << "," << indices[i] << "," << indices[i]<< std::endl;
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "Model file not successfully read!" << e.what() << std::endl;
    }
}