#include "PostProcessComponent.h"
#include "Engine.h"

namespace neu {
	FACTORY_REGISTER(PostProcessComponent)

	void PostProcessComponent::Update(float dt)	{
	}

	void PostProcessComponent::Apply(Program& program) {
		program.SetUniform("u_parameters", (uint32_t)parameters);
		program.SetUniform("u_colorTint", colorTint);
		program.SetUniform("u_time", GetEngine().GetTime().GetTime()); 
		program.SetUniform("u_blend", blend);
	}

	void PostProcessComponent::Read(const serial_data_t& value) {
		Object::Read(value);
	}

	void PostProcessComponent::UpdateGui() {
		uint32_t iparameters = (uint32_t)parameters;

		bool parameter = iparameters & (uint32_t)Parameters::GrayScale;
		if (ImGui::Checkbox("Gray Scale", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::GrayScale;
			else iparameters &= ~(uint32_t)Parameters::GrayScale;
		}

		parameter = iparameters & (uint32_t)Parameters::ColorTint;
		if (ImGui::Checkbox("Color Tint", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ColorTint;
			else iparameters &= ~(uint32_t)Parameters::ColorTint;
		}

		parameter = iparameters & (uint32_t)Parameters::Inverse;
		if (ImGui::Checkbox("Inverse", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Inverse;
			else iparameters &= ~(uint32_t)Parameters::Inverse;
		}

		parameter = iparameters & (uint32_t)Parameters::Grain;
		if (ImGui::Checkbox("Grain", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Grain;
			else iparameters &= ~(uint32_t)Parameters::Grain;
		}

		parameter = iparameters & (uint32_t)Parameters::ScanLines;
		if (ImGui::Checkbox("Scan Lines", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ScanLines;
			else iparameters &= ~(uint32_t)Parameters::ScanLines;
		}

		parameter = iparameters & (uint32_t)Parameters::Vignette;
		if (ImGui::Checkbox("Vignette", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Vignette;
			else iparameters &= ~(uint32_t)Parameters::Vignette;
		}


		parameters = (Parameters)iparameters;

		ImGui::ColorEdit3("Color Tint", glm::value_ptr(colorTint));
		ImGui::SliderFloat("Blend", &blend, 0, 1); 

	}
}
