#pragma once
#include "Component.h"

#include "Hollow/Graphics/Vertex.h"

namespace Hollow {

	class VertexArray;
	class VertexBuffer;
	class ElementArrayBuffer;

	class HOLLOW_API Shape : public Component
	{
		REGISTERCOMPONENT(Shape, "Shape");
	public:
		void Init();

		void Clear() {}

		virtual ~Shape();
		void MakeVAO();
		
		void DebugDisplay();

	public:
		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mIndices;

		std::string mShapeType;
		VertexBuffer* mpVBO;
		VertexArray* mpVAO;
		ElementArrayBuffer* mpEBO;

	private:
		void CreateBuffers();
		void CreateShape(const std::string& shapeType);
		void CreateSphere();
		void CreateTeapot();
	};
}