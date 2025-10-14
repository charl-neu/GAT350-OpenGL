#include "Mesh.h"
#include "Renderer.h"

namespace neu {
	bool Mesh::Load(const std::string& filename) {

		std::string buffer;
		if (!file::ReadTextFile(filename, buffer)) {
			LOG_ERROR("Could not read file: {}", filename);
			return false;
		}

		std::stringstream stream(buffer);

		// read color
		stream >> m_color;

		// read points
		vec2 point;
		while (stream >> point) {
			m_points.push_back(point);
		}

		if (!stream.eof()) {
			LOG_ERROR("Could not parse file: {}", filename);
			return false;
		}

		return true;
	}





	/// <summary>
	/// Calculates and updates the model's radius as the maximum distance from the origin among all points.
	/// </summary>
	void Mesh::CalculateRadius()
	{
		m_radius = 0;
		for (auto& point : m_points) {
			float length = point.Length();
			if (length > m_radius) m_radius = length;
		}
	}
}