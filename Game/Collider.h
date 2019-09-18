#ifndef COLLIDER_H
#define COLLIDER_H

class Collider
{
public:
    Collider();

private:
    float newVertices[24] = 
    {
	0.5f, 0.5f, 0.5f,  
	0.5f, 0.5f, -0.5f, 
	0.5f,  -0.5f, 0.5f,  
	0.5f,  -0.5f, -0.5f,  
   -0.5f, 0.5f, 0.5f,  
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  -0.5f
   };
};

#endif