#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Collider
{
public:
    Collider(const float newVertices[24]);
    Collider(glm::vec3 newMin, glm::vec3 newMax);

    bool CheckPoint(glm::vec3 position);

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
   glm::vec3 min;
   glm::vec3 max;
};

#endif