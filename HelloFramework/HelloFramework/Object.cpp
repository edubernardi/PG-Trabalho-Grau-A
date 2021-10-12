#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::initialize()
{
	GLuint VBO, EBO;

	float vertices[] = {
		// positions          // colors          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	transform = glm::mat4(1);
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	angle = 0;
	shader = NULL;
	texID = 0;
}

void Object::update()
{
	shader->Use();

	transform = glm::mat4(1);
	setTranslation(position, false);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	shader->setMat4("model", glm::value_ptr(transform));

}

void Object::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::vec3 Object::getTopRightVertex()
{
	return glm::vec3(position.x + (scale.x / 2), position.y + (scale.y / 2), 1);
}

glm::vec3 Object::getBottomLeftVertex()
{
	return glm::vec3(position.x - (scale.x / 2), position.y - (scale.y / 2), 1);
}

void Object::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) {
		transform = glm::mat4(1);
	}
	else {
		transform = glm::rotate(transform, angle, axis);
	}
}

void Object::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) {
		transform = glm::mat4(1);
	}
	else {
		transform = glm::translate(transform, displacements);
	}
}

void Object::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) {
		transform = glm::mat4(1);
	}
	else {
		transform = glm::scale(transform, scaleFactors);
	}
}
