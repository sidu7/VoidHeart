#pragma once

namespace Hollow
{
	enum Shapes;
	class Mesh;
	class VertexArray;

	struct DebugRenderData
	{
		std::vector<Mesh*> mpMeshes;
		glm::mat4 mpModel;
		int mDrawCommand;
		Shapes mType;
		glm::vec3 mColor;

		DebugRenderData(std::vector<Mesh*> mesh, glm::mat4 model, int drawCommand, Shapes shape, glm::vec3 color)
			: mpMeshes(mesh), mpModel(model), mDrawCommand(drawCommand), mType(shape), mColor(color) { }
	};

	struct DebugPathData
	{
		VertexArray* mCurveVAO;
		unsigned int mCurvePointsCount;
	};
}