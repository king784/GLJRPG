#include "Collider.h"

Collider::Collider(const float newVertices[24])
{
    for(int i = 0; i < 24; i++)
    {
        colVertices[i] = newVertices[i];
    }
}

void Collider::SetPosition(glm::vec3 newPosition)
{
    
}