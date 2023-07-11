#include "Sun.h"

Sun::Sun(const char* model_path) : riseY(-550.0f), increment(0.05f)
{
	sun = new Model7(model_path);
	sd = new Shader7("shaders/onlyColor.shader");
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, riseY, -2000000.0f));
	model = glm::scale(model, glm::vec3(500.0f, 500.0f, 500.0f));
	pos = glm::vec3(0.0f);
}

Sun::~Sun()
{
	if (sd) delete sd;
	if (sun) delete sun;
}

void Sun::render()
{
	sd->bind();
	sd->SetUniformMat4f("model", model);
	sd->SetUniformMat4f("projection", projection);
	sd->SetUniformMat4f("view", view);
	sd->SetUniform3f("Color", glm::vec3(1.0f, 0.9f, 0.3f));
	sun->render();
	sd->unbind();
}

glm::vec3 Sun::getPos()
{
	return pos;
}

void Sun::getViewProjection(glm::mat4 view, glm::mat4 projection)
{
	this->view = view;
	this->projection = projection;
}

void Sun::rise()
{
	if (riseY <= 600.0f) {
		riseY += increment;
		pos = glm::vec3(0.0f, riseY, -2000.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, riseY, -2000.0f));
		model = glm::scale(model, glm::vec3(500.0f, 500.0f, 500.0f));
	}
}

