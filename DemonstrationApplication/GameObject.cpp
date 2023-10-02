#include "GameObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

void GameObject::Create(const Mesh& mesh, GLuint texID) {
	m_mesh = mesh;
	m_texID = texID;

	m_modelMat = glm::mat4(1.f);
}

void GameObject::Create(const Mesh& mesh, string TexPath, bool Wrap) {
	m_mesh = mesh;

	m_modelMat = glm::mat4(1.f);
	unsigned char* imageData = nullptr;
	m_texID = 0;

	if (TexPath != "None") {

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true); // To match OpenGL's texture coordinate system
		unsigned char* imageData = stbi_load(TexPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);


		glGenTextures(1, &m_texID);

		// Bind texture
		glBindTexture(GL_TEXTURE_2D, m_texID);

		if (imageData) {
			cout << "Loaded: " << TexPath << endl;
			// Upload the image data to the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

			// Set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			if (Wrap) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			// Unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);

			// Free the loaded image data
			stbi_image_free(imageData);
		}
		else {
			// Error handling if loading the image failed
			std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
			m_texID = 0;
		}
	}

}

void GameObject::setModelMatrix(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
	glm::mat4 modelMatrix(1.0);

	// Apply translation
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translate);
	modelMatrix = translationMatrix * modelMatrix;

	// Apply rotation
	glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ * modelMatrix;

	// Apply scaling
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	modelMatrix = scaleMatrix * modelMatrix;

	m_modelMat = modelMatrix;
}

glm::mat4 GameObject::ModelMatrix() {
	return m_modelMat;
}

void GameObject::Render(Shader* shader) {
	shader->SetUniform("model", m_modelMat);
	glBindTexture(GL_TEXTURE_2D, m_texID);

	m_mesh.Render();
}
