#pragma once
#include "Component.h"

namespace Hollow
{
	class Texture;
	class Mesh;
	enum Shapes;

	class UIImage : public Component
	{
		REGISTERCOMPONENT(UIImage);
	public:
		/// <summary>
		/// Initializes the component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the UIImage component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the UIImage component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current UIImage component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the UIImage Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;
		
	private:
		std::string mShapeType;
		
	public:
		std::string TexturePath;
		Texture* mpTexture;
		glm::vec3 mColor;
		Mesh* mpShapeData;
	};
}