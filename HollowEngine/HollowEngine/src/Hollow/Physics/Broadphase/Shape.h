#pragma once

#include "Hollow/Physics/NarrowPhase/MeshData.h"
#include "Utils/RayCast.h"
#include "Hollow/Components/Collider.h"

namespace Hollow {
	class Collider;

	enum ShapeType
	{
		BALL,
		BOX,
		SHAPESNUM
	};

	class Shape
	{
	public:
		Shape(ShapeType type)
		{
			mType = type;
			mpOwnerCollider = nullptr;
		}
		virtual ~Shape() {}
		virtual glm::vec3 GetHalfExtents() = 0;
		virtual bool TestRay(const Ray& r, IntersectionData& id, glm::mat3& rot, glm::vec3& extents) = 0;
		Collider* mpOwnerCollider;

	public:
		ShapeType mType;
	};

	class ShapeCircle : public Shape
	{
	public:
		bool TestRay(const Ray& r, IntersectionData& id, glm::mat3& rot, glm::vec3& extents)
		{
			return true;
		}
		ShapeCircle(float radius,glm::vec3 center) : Shape(ShapeType::BALL)
		{
			mRadius = radius;
			mCenter = center;
		}
		bool TestRay(const Ray& r, IntersectionData& id)
		{
			return false;
		}
		glm::vec3 GetHalfExtents() {
			glm::vec3 extents;
			extents = glm::vec3(mRadius, mRadius, mRadius);
			return extents;
		}

		float mRadius;
		glm::vec3 mCenter;
	};

	class ShapeAABB : public Shape
	{
	public:
		ShapeAABB(glm::vec3 min, glm::vec3 max) : Shape(ShapeType::BOX)
		{
			mMin = min;
			mMax = max;

			SetMeshData();
		}

		// when testing for tree aabb use this
		bool TestRay(const Ray& r, IntersectionData& data)
		{
			float a = 0;
			float b = std::numeric_limits<float>::infinity();

			glm::vec3 Extents = GetHalfExtents() * 2.0f;

			int i = 0;
			while (i < 6 && a <= b) {
				glm::vec3 pointOnFace = mMeshData.GetPointOnFace(i) * Extents + GetCenter();
				if (glm::dot(r.direction, mMeshData.faces[i].normal) < 0) {
					data.depth = glm::dot((pointOnFace - r.origin), mMeshData.faces[i].normal) /
						glm::dot(r.direction, mMeshData.faces[i].normal);
					if (a < data.depth) {
						a = data.depth;
					}
				}
				else if (glm::dot(r.direction, mMeshData.faces[i].normal) > 0) {
					data.depth = glm::dot((pointOnFace - r.origin), mMeshData.faces[i].normal) /
						glm::dot(r.direction, mMeshData.faces[i].normal);

					if (b > data.depth) {
						b = data.depth;
					}
				}
				else if (glm::dot(r.origin - GetCenter(), mMeshData.faces[i].normal) > 0) {
					b = a - 1;
				}

				++i;
			}

			if (a > b) {
				return false;
			}
			else if (a == 0) {
				data.depth = b;
				return true;
			}
			else {
				data.depth = a;
				return true;
			}

			return false;
		}

		// when testing for actual object use this
		bool TestRay(const Ray& r, IntersectionData& data, glm::mat3& rot, glm::vec3& extents)
		{
			float a = 0;
			float b = std::numeric_limits<float>::infinity();
			
			int i = 0;
			while (i < 6 && a <= b) {
				glm::vec3 pointOnFace = rot * (mMeshData.GetPointOnFace(i) * extents) + GetCenter();
				glm::vec3 normal = rot * mMeshData.faces[i].normal;
				if (glm::dot(r.direction, normal) < 0) {
					data.depth = glm::dot((pointOnFace - r.origin), normal) /
						glm::dot(r.direction, normal);
					if (a < data.depth) {
						a = data.depth;
					}
				}
				else if (glm::dot(r.direction, normal) > 0) {
					data.depth = glm::dot((pointOnFace - r.origin), normal) /
						glm::dot(r.direction, normal);

					if (b > data.depth) {
						b = data.depth;
					}
				}
				else if (glm::dot(r.origin - GetCenter(), normal) > 0) {
					b = a - 1;
				}

				++i;
			}

			if (a > b) {
				return false;
			}
			else if (a == 0) {
				data.object = mpOwnerCollider; // should be saving gameobject pointer instead
				data.depth = b;
				return true;
			}
			else {
				data.object = mpOwnerCollider; // should be saving gameobject pointer instead
				data.depth = a;
				return true;
			}

			return false;
		}
		//This is the AABB of the rotated boxes!!!!!!!!!!!!!!!(this is not the exact scales- use mptr scale)
		glm::vec3 GetHalfExtents() {
			glm::vec3 extents;
			extents = glm::vec3((mMax.x - mMin.x) * 0.5f, (mMax.y - mMin.y) * 0.5f, (mMax.z - mMin.z) * 0.5f);
			return extents;
		}

		glm::mat4& DebugModelMatrix() {
			mDebugMatrix = glm::translate(glm::mat4(1.0f), GetCenter());
			mDebugMatrix = glm::scale(mDebugMatrix, GetHalfExtents() * 2.0f);
			return mDebugMatrix;
		}

		bool Collides(ShapeAABB* other) {

			if ((mMin.x >= other->mMax.x || mMax.x <= other->mMin.x) ||
				(mMin.y >= other->mMax.y || mMax.y <= other->mMin.y) ||
				(mMin.z >= other->mMax.z || mMax.z <= other->mMin.z)
				)
			{
				return false;
			}
			
			return true;
			
		}

		// set the bounds of this AABB to encompass the argument AABBs
		void Merge(ShapeAABB& first, ShapeAABB& second) {
			mMin.x = std::min(first.mMin.x, second.mMin.x);
			mMin.y = std::min(first.mMin.y, second.mMin.y);
			mMin.z = std::min(first.mMin.z, second.mMin.z);
			
			mMax.x = std::max(first.mMax.x, second.mMax.x);
			mMax.y = std::max(first.mMax.y, second.mMax.y);
			mMax.z = std::max(first.mMax.z, second.mMax.z);
		}

		void fatten(Shape* shape, glm::vec3 fatMargin)
		{
			if (shape->mType == BOX)
			{
				this->mMin = static_cast<ShapeAABB*>(shape)->mMin
					- fatMargin;
				this->mMax = static_cast<ShapeAABB*>(shape)->mMax
					+ fatMargin;
			}
			else if (shape->mType == BALL)
			{
				this->mMin = (static_cast<ShapeCircle*>(shape)->mCenter - glm::vec3(static_cast<ShapeCircle*>(shape)->mRadius)) - fatMargin;
				this->mMax = (static_cast<ShapeCircle*>(shape)->mCenter + glm::vec3(static_cast<ShapeCircle*>(shape)->mRadius)) + fatMargin;
			}
		}

		bool Contains(Collider* col) {
			if (col->mpShape->mType == BOX)
			{
				ShapeAABB* other = static_cast<ShapeAABB*>(col->mpShape);
				if (other->mMin.x < mMin.x || other->mMin.y < mMin.y || other->mMin.z < mMin.z) {
					return false;
				}
				if (other->mMax.x > mMax.x || other->mMax.y > mMax.y || other->mMax.z > mMax.z) {
					return false;
				}
			}
			else if (col->mpShape->mType == BALL)
			{
				ShapeCircle* other = static_cast<ShapeCircle*>(col->mpShape);
				glm::vec3 min, max;
				min = other->mCenter - other->mRadius;
				max = other->mCenter + other->mRadius;
				if (min.x < mMin.x || min.y < mMin.y || min.z < mMin.z) {
					return false;
				}
				if (max.x > mMax.x || max.y > mMax.y || max.z > mMax.z) {
					return false;
				}
			}
			return true;
		}

		glm::vec3 GetCenter() {
			return glm::vec3((mMax.x + mMin.x) * 0.5f, (mMax.y + mMin.y) * 0.5f, (mMax.z + mMin.z) * 0.5f);
		}

		float GetSurfaceArea() {
			glm::vec3 extents = GetHalfExtents();
			return 8.0f * (extents.x * extents.y + extents.y * extents.z + extents.z * extents.x);
		}

		void SetMeshData()
		{
			// generate vertices
			mMeshData.AddVertex(glm::vec3(-0.5f, -0.5f, 0.5f));
			mMeshData.AddVertex(glm::vec3(0.5f, -0.5f, 0.5f));
			mMeshData.AddVertex(glm::vec3(0.5f, 0.5f, 0.5f));
			mMeshData.AddVertex(glm::vec3(-0.5f, 0.5f, 0.5f));
			mMeshData.AddVertex(glm::vec3(-0.5f, -0.5f, -0.5f));
			mMeshData.AddVertex(glm::vec3(0.5f, -0.5f, -0.5f));
			mMeshData.AddVertex(glm::vec3(0.5f, 0.5f, -0.5f));
			mMeshData.AddVertex(glm::vec3(-0.5f, 0.5f, -0.5f));

			// generate (quadrilateral) faces
			std::vector<int> faceVerts;
			faceVerts.clear();
			// front
			faceVerts.push_back(0);
			faceVerts.push_back(1);
			faceVerts.push_back(2);
			faceVerts.push_back(3);
			mMeshData.AddFace(faceVerts);

			faceVerts.clear();
			// back
			faceVerts.push_back(7);
			faceVerts.push_back(6);
			faceVerts.push_back(5);
			faceVerts.push_back(4);
			mMeshData.AddFace(faceVerts);

			faceVerts.clear();
			// top
			faceVerts.push_back(3);
			faceVerts.push_back(2);
			faceVerts.push_back(6);
			faceVerts.push_back(7);
			mMeshData.AddFace(faceVerts);
			
			faceVerts.clear();
			// bottom
			faceVerts.push_back(1);
			faceVerts.push_back(0);
			faceVerts.push_back(4);
			faceVerts.push_back(5);
			mMeshData.AddFace(faceVerts);
			
			faceVerts.clear();
			// right
			faceVerts.push_back(2);
			faceVerts.push_back(1);
			faceVerts.push_back(5);
			faceVerts.push_back(6);
			mMeshData.AddFace(faceVerts);
			
			faceVerts.clear();
			// left
			faceVerts.push_back(0);
			faceVerts.push_back(3);
			faceVerts.push_back(7);
			faceVerts.push_back(4);
			mMeshData.AddFace(faceVerts);
		}

		glm::vec3 mMin, mMax;
		glm::mat4 mDebugMatrix;
		MeshData mMeshData;
	};

}