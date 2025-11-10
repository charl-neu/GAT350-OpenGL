#include "LightComponent.h"

namespace neu {
	FACTORY_REGISTER(LightComponent);
	void LightComponent::Update(float dt) {

	}

	void LightComponent::SetProgram(Program& program, const std::string& name, const glm::mat4& view) {
		glm::vec3 position = glm::vec3(view * glm::vec4(owner->transform.position, 1));
		glm::vec3 direction = glm::normalize(glm::mat3(view) * owner->transform.Forward());
		
		program.SetUniform(name + ".type", (int)u_lightType);
		program.SetUniform(name + ".position", position);
		program.SetUniform(name + ".direction", direction);
		program.SetUniform(name + ".color", color);
		program.SetUniform(name + ".intensity", intensity);
		program.SetUniform(name + ".range", range);
		program.SetUniform(name + ".innerSpotAngle", innerSpotAngle);
		program.SetUniform(name + ".outerSpotAngle", outerSpotAngle);

	}
	void LightComponent::Read(const serial_data_t& value) {
		std::string lightType;
		SERIAL_READ(value, lightType);
		if (equalsIgnoreCase(lightType, "point")) u_lightType = LightType::Point;
		else if (equalsIgnoreCase(lightType, "directional")) u_lightType = LightType::Directional;
		else if (equalsIgnoreCase(lightType, "spot")) u_lightType = LightType::Spot;

		SERIAL_READ(value, color);
		SERIAL_READ(value, intensity);
		SERIAL_READ(value, range);
		SERIAL_READ(value, innerSpotAngle);
		SERIAL_READ(value, outerSpotAngle);
	}
	void LightComponent::UpdateGui() {
		const char* types[] = { "Point", "Directional", "Spot" };
		ImGui::Combo("Type", (int*)&u_lightType, types, 3);

		ImGui::ColorEdit3("Color", glm::value_ptr(color));
		ImGui::DragFloat("Intensity", &intensity, .1f, .1f);
		if (u_lightType != LightType::Directional) {
			ImGui::DragFloat("Range", &range, .1f, .1f);
		}
		if (u_lightType == LightType::Spot) {
			ImGui::DragFloat("innerCutoff", &innerSpotAngle, 0.1f, 0.0f, outerSpotAngle);
			ImGui::DragFloat("OuterCutoff", &outerSpotAngle, 0.1f, innerSpotAngle);


		}
	}
}