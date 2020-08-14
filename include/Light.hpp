#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Model.hpp"

enum LightEnum
{
	LIGHT_POSITIONAL,
	LIGHT_DIRECTIONAL,
	LIGHT_SPOTLIGHT,
};

class Light : Model
{
public:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation;
	float ambientFactor;
	bool isDirectional;
	bool isSpotlight;
	glm::vec3 spotlightDirection;
	float cutOff;
	float outerCutOff;
	bool isActive;
	Light(
		std::string path,
		glm::vec3 pos = glm::vec4(0.0f),
		glm::vec3 color = glm::vec3(1.0f),
		LightEnum lightType = LIGHT_POSITIONAL,
		float ambientFactor = 0.1f,
		glm::vec3 attenuation = glm::vec3(1.0f, 0.045f, 0.0075f),
		glm::vec3 spotlightDirection = glm::vec3(0.0f),
		float cutOff = 0.0f,
		float outerCutOff = 0.0f)
		: Model(path)
	{
		this->position = glm::vec3(pos);
		this->color = color;
		this->ambientFactor = ambientFactor;
		this->attenuation = attenuation;

		this->spotlightDirection = glm::vec3(0.0f);
		this->cutOff = 0.0f;
		this->outerCutOff = 0.0f;
		isActive = true;
		switch (lightType)
		{
		case LIGHT_POSITIONAL:
			isDirectional = false;
			isSpotlight = false;
			break;
		case LIGHT_DIRECTIONAL:
			isDirectional = true;
			isSpotlight = false;
			this->attenuation = glm::vec3(1.0f);
			break;
		case LIGHT_SPOTLIGHT:
			isDirectional = false;
			isSpotlight = true;
			this->spotlightDirection = spotlightDirection;
			this->cutOff = cutOff;
			this->outerCutOff = outerCutOff;
			break;
		}
	}

	void Draw(Shader &shader)
	{
		Model::Draw(shader);
	}

	void setUniforms(Shader &shader, unsigned int index = 0)
	{
		const std::string indexStr = std::to_string(index);
		shader.use();

		if (isActive)
		{
			shader.set("lights[" + indexStr + "].position", position);
			shader.set("lights[" + indexStr + "].ambient", ambientFactor * color);
			shader.set("lights[" + indexStr + "].diffuse", color);
			shader.set("lights[" + indexStr + "].specular", color);
			shader.set("lights[" + indexStr + "].attenuation", attenuation);
			shader.set("lights[" + indexStr + "].spotlightDirection", spotlightDirection);
			shader.set("lights[" + indexStr + "].cut_off", cutOff);
			shader.set("lights[" + indexStr + "].outer_cut_off", outerCutOff);
			shader.set("lights[" + indexStr + "].isDirectional", isDirectional);
			shader.set("lights[" + indexStr + "].isSpotlight", isSpotlight);
		}
		else
		{
			shader.set("lights[" + indexStr + "].ambient", glm::vec3(0.0f));
			shader.set("lights[" + indexStr + "].diffuse", glm::vec3(0.0f));
			shader.set("lights[" + indexStr + "].specular", glm::vec3(0.0f));
			shader.set("lights[" + indexStr + "].attenuation", glm::vec3(0.0f));
		}
	}
};