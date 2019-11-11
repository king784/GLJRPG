#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Shader.h"
#include "Texture.h"
#include "Collider.h"
#include "Drawable.h"

#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GameObject
{
public:
	unsigned int VBO, VAO;
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 model;
	unsigned int textureID;
	Collider* collider;
	
	GameObject() 
	{
		this->position = glm::vec3(0.0);
		this->scale = glm::vec3(1.0);
		this->model = glm::mat4(1.0f);
		Initialize();
	}

	GameObject(glm::vec3 position, glm::vec3 scale)
	{
		this->position = position;
		this->scale = scale;
		this->model = glm::mat4(1.0f);
		Initialize();
	}

	void Rotate(glm::vec3 axis, float angle)
	{
		this->model = glm::rotate(model, glm::radians(angle), axis);
	}

	virtual void AddCollider() {}

	virtual void Initialize() {}
private:
	virtual void Draw() {}
};

class ScreenQuad : public GameObject, public Drawable
{
public:
	ScreenQuad(unsigned int textureID, Shader theShader)
	{
		this->position = glm::vec3(0.0);
		this->scale = glm::vec3(1.0);
		this->model = glm::mat4(1.0);
		this->textureID = textureID;
		this->shader = new Shader();
        *shader = theShader;
		Initialize();
	}

	~ScreenQuad()
	{
		delete this->shader;
	}

	void Draw() override
	{
		shader->Use();
		shader->SetMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindVertexArray(this->VAO);
		
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	Shader* shader;
	unsigned int EBO;
	// float vertices[24] = {
	// 	// positions   // texCoords
	// 	-1.0f,  1.0f,  0.0f, 1.0f,
	// 	-1.0f, -1.0f,  0.0f, 0.0f,
	// 	 1.0f, -1.0f,  1.0f, 0.0f,

	// 	-1.0f,  1.0f,  0.0f, 1.0f,
	// 	 1.0f, -1.0f,  1.0f, 0.0f,
	// 	 1.0f,  1.0f,  1.0f, 1.0f
	// };
	float vertices[20] = 
    {
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f    // top left 
    };

	unsigned int indices[6] = 
    {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	void Initialize()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		// Unbind
		glBindVertexArray(0);

		// Plane is stationary, set start pos
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
	}
};

class Plane : public GameObject
{
public:
	Plane(glm::vec3 position, glm::vec3 scale)
	{
		this->position = position;
		this->scale = scale;
		this->model = glm::mat4(1.0f);
		Initialize();
	}
	
	void Initialize()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		// Vertex colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		// Vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);

		Texture text("ground.png");
		textureID = text.GetID();

		// Plane is stationary, set start pos
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		Rotate(glm::vec3(1.0, 0.0, 0.0), 90.0f);
	}

	void Draw(Shader& shader)
	{
		shader.SetMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	unsigned int EBO;
	float vertices[32] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
};

class Cube : public GameObject
{
public:
	Cube(glm::vec3 position, glm::vec3 scale)
	{
		this->position = position;
		this->scale = scale;
		this->path = "";
		this->filename = "";
		this->model = glm::mat4(1.0f);
		InitializeWithoutTexture();
	}
	Cube(glm::vec3 position, glm::vec3 scale, std::string path, std::string filename)
	{
		this->position = position;
		this->scale = scale;
		this->path = path;
		this->filename = filename;
		this->model = glm::mat4(1.0f);
		Initialize();
	}

	void Initialize()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(VAO);

		// set the vertex attributes (only position data for our lamp)
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		if (!path.empty() && !filename.empty())
		{
			Texture text(filename.c_str());
			textureID = text.GetID();
		}

		// Plane is stationary, set start pos
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
	}

	void InitializeWithoutTexture()
	{
		float newVertices[] = {
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
   - 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(newVertices), newVertices, GL_STATIC_DRAW);

		glBindVertexArray(VAO);

		// set the vertex attributes (only position data for our lamp)
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		// Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


		// Plane is stationary, set start pos
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
	}

	void Draw(Shader shader)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		
		shader.SetMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void DrawWithoutTexture(Shader shader)
	{
		model = glm::mat4(1.0f);
    	model = glm::translate(model, position);
    	model = glm::scale(model, scale);

		shader.SetMat4("model", model);

		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:
	std::string path, filename;
	float vertices[288] = {
		// back
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		// front
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		// left
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		// right
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		// bottom
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		// top
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
	};
};

class Skybox : public GameObject
{
public:
	Skybox(glm::vec3 position, glm::vec3 scale, std::string path)
	{
		this->position = position;
		this->scale = scale;
		this->path = path;
		this->model = glm::mat4(1.0f);
		Initialize();
	}

	void Draw()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

private:
	std::string path, filename;
	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	float vertices[108] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	void Initialize()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(VAO);

		// set the vertex attributes (only position data for our lamp)
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		// Generate cubemap texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		
		for (GLuint i = 0; i < faces.size(); i++)
		{
			std::string endPath = path + faces[i];
			unsigned char* data = stbi_load(endPath.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLenum format;
				if (nrChannels == 1)
					format = GL_RED;
				else if (nrChannels == 3)
					format = GL_RGB;
				else if (nrChannels == 4)
					format = GL_RGBA;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture load failed!!" << std::endl;
				stbi_image_free(data);
			}
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
};

#endif
