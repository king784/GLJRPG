#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Collider
{
public:
    Collider(const float newVertices[24]);

    void SetPosition(glm::vec3 newPosition);

private:
    float colVertices[24] = 
    {
	1.0f, 1.0f, 1.0f,  
	1.0f, 1.0f, -1.0f, 
	1.0f,  -1.0f, 1.0f,  
	1.0f,  -1.0f, -1.0f,  
   -1.0f, 1.0f, 1.0f,  
   -1.0f,  1.0f, -1.0f,
   -1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  -1.0f
   };
};

#endif