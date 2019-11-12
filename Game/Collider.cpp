#include "Collider.h"

Collider::Collider(const float newVertices[24])
{
    for(int i = 0; i < 24; i++)
    {
        colVertices[i] = newVertices[i];
    }
}

Collider::Collider(glm::vec3 newMin, glm::vec3 newMax)
{
    min = newMin;
    max = newMax;
}

bool Collider::CheckPoint(glm::vec3 position)
{
    //if(position > min &&)
}

void Collider::SetPosition(glm::vec3 newPosition)
{
    
}

