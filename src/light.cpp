#include "light.h"

Light7::Light7(Shader7* shader)
{
    shader->bind();
    shader->SetUniform3f("dirLight.position", 0.0f, 0.0f, 0.0f);
    shader->SetUniform3f("pointLight.position", 0.0f, 0.0f, 0.0f);
    shader->unbind();
}

Light7::~Light7()
{
}

void Light7::setCam(Shader7* shader, glm::vec3 position)
{
    shader->SetUniform3f("camPos", position);
}

void Light7::useSunLight(Shader7* shader, glm::vec3 position)
{
    shader->SetUniform3f("dirLight.position", position);
    shader->SetUniform1f("dirLight.ambient", 0.05f);
    shader->SetUniform1f("dirLight.diffuse", 0.45f);
    shader->SetUniform1f("dirLight.specular", 0.1f);
}

void Light7::useBulb1(Shader7* shader)
{
    shader->SetUniform3f("pointLight.position", bulbPos);
    shader->SetUniform1f("pointLight.ambient", 0.4f);
    shader->SetUniform1f("pointLight.diffuse", 0.2f);
    shader->SetUniform1f("pointLight.specular", 0.1f);
    shader->SetUniform1f("pointLight.constant", 1.0f);
    shader->SetUniform1f("pointLight.linear", 0.7f);
    shader->SetUniform1f("pointLight.quadratic", 1.8f);
}


void Light7::noBulb1(Shader7* shader)
{
    shader->SetUniform3f("pointLight.position", 0.0f, 0.0f, 0.0f);
}

void Light7::setMaterial(Shader7* shader, float material)
{
    shader->SetUniform1f("material", material);
}
