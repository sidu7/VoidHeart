#pragma once
#include "Component.h"

namespace Hollow
{
	class Mesh;
	class Shader;
	enum Shapes;

	class Model : public Component
	{
		REGISTERCOMPONENT(Model);
	public:
		/// <summary>
		/// Initializes the component.
		/// </summary>
		HOLLOW_API void Init() override;
		/// <summary>
		/// Clears the component.
		/// </summary>
		HOLLOW_API void Clear() override;
		/// <summary>
		/// Serializes the specified object.
		/// </summary>
		/// <param name="object">The object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object object) override;
		/// <summary>
		/// DeSerialize the current component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		/// <summary>
		/// To set the Debug Display for the Components.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	private:
		std::string mShapeType;
		std::string mModelPath;

	public:
		std::vector<Mesh*> mMeshes;
		Shapes mShape;
		bool mCastShadow;
		bool mModelHasOffset;
	};
}