#include <hollowpch.h>

#include "SAT.h"
#include "ContactManifold.h"
#include "Contact.h"
#include "MeshData.h"

#include "Hollow/Components/Collider.h"
#include "Hollow/Components/Transform.h"

#define epsilon 0.0001f
namespace Hollow {

	SAT::SAT()
	{
		mContacts = new std::list<ContactManifold*>();
		mPrevContacts = new std::list<ContactManifold*>();
		CollisionFunctions[ShapeType::BOX][ShapeType::BOX] = &SAT::TestIntersection3D;
		CollisionFunctions[ShapeType::BOX][ShapeType::BALL] = &SAT::TestBoxBallIntersection;
		CollisionFunctions[ShapeType::BALL][ShapeType::BOX] = &SAT::TestBallBoxIntersection;
		CollisionFunctions[ShapeType::BALL][ShapeType::BALL] = &SAT::TestBallBallIntersection;
	}

	SAT::~SAT()
	{
		for (auto c : *mContacts)
			delete c;

		mContacts->clear();

		for (auto c : *mPrevContacts)
			delete c;

		mPrevContacts->clear();
		CollisionFunctions[ShapeType::BOX][ShapeType::BOX] = nullptr;
		CollisionFunctions[ShapeType::BOX][ShapeType::BALL] = nullptr;
		CollisionFunctions[ShapeType::BALL][ShapeType::BOX] = nullptr;
		CollisionFunctions[ShapeType::BALL][ShapeType::BALL] = nullptr;
	}

	// from http://paulbourke.net/geometry/pointlineplane/
	float LineLineIntersect(
		glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3* poA, glm::vec3* poB)
	{
		glm::vec3 p13, p43, p21, pa, pb;
		double d1343, d4321, d1321, d4343, d2121;
		double numer, denom;

		p13.x = p1.x - p3.x;
		p13.y = p1.y - p3.y;
		p13.z = p1.z - p3.z;
		p43.x = p4.x - p3.x;
		p43.y = p4.y - p3.y;
		p43.z = p4.z - p3.z;

		p21.x = p2.x - p1.x;
		p21.y = p2.y - p1.y;
		p21.z = p2.z - p1.z;

		d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
		d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
		d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
		d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
		d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

		denom = d2121 * d4343 - d4321 * d4321;
		if (abs(denom) < 0.0001)
			return 0.0f;
		numer = d1343 * d4321 - d1321 * d4343;

		double mua = numer / denom;
		double mub = (d1343 + d4321 * (mua)) / d4343;

		pa.x = static_cast<float>(p1.x + mua * p21.x);
		pa.y = static_cast<float>(p1.y + mua * p21.y);
		pa.z = static_cast<float>(p1.z + mua * p21.z);
		pb.x = static_cast<float>(p3.x + mub * p43.x);
		pb.y = static_cast<float>(p3.y + mub * p43.y);
		pb.z = static_cast<float>(p3.z + mub * p43.z);

		*poA = pa;
		*poB = pb;

		return glm::distance(pa, pb);
	}

	// from Orange Book!! To take into account the thickness of the plane
	std::vector<glm::vec3> ClipPolygon(const std::vector<glm::vec3>& polygon, glm::vec3 normal, glm::vec3 pointOnPlane, float thickness = 0.0f) {
		std::vector<glm::vec3> clipped;

		glm::vec3 ver1 = polygon.back();

		float dist1 = glm::dot(normal, ver1 - pointOnPlane);

		for (int i = 0; i < polygon.size(); ++i) {
			glm::vec3 ver2 = polygon[i];
			float dist2 = glm::dot(normal, ver2 - pointOnPlane);
			if (dist1 < 0.0f - thickness && dist2 > 0.0f)
			{
				float frac = dist1 / (dist1 - dist2);
				glm::vec3 intersectionPoint = ver1 + frac * (ver2 - ver1);
				clipped.push_back(intersectionPoint);
			}
			else if (dist1 < 0.0f - thickness && ((dist2 <= 0.0f) && (dist2 >= 0.0f - thickness)))
			{
				clipped.push_back(ver2);
			}
			else if (dist1 > 0.0f && dist2 < 0.0f - thickness)
			{
				float frac = dist1 / (dist1 - dist2);
				glm::vec3 intersectionPoint = ver1 + frac * (ver2 - ver1);
				clipped.push_back(intersectionPoint);
				clipped.push_back(ver2);
			}
			else if (((dist1 <= 0.0f) && (dist1 >= 0.0f - thickness)) && dist2 < 0.0f - thickness)
			{
				clipped.push_back(ver1);
				clipped.push_back(ver2);
			}
			else if (dist1 < 0.0f - thickness && dist2 < 0.0f - thickness)
			{
				clipped.push_back(ver2);
			}
			else if (((dist1 <= 0.0f) && (dist1 >= 0.0f - thickness)) && ((dist2 <= 0.0f) && (dist2 >= 0.0f - thickness)))
			{
				clipped.push_back(ver2);
			}
			ver1 = ver2;
			dist1 = dist2;
		}

		return clipped;
	}

	// SAT Gregorius style
	bool SAT::TestIntersection3D(Collider* col1, Collider* col2) {
		FaceQuery faceQueryA = FaceIntersectionQuery(col1, col2);
		if (faceQueryA.separation > 0.0f) return false;

		FaceQuery faceQueryB = FaceIntersectionQuery(col2, col1);
		if (faceQueryB.separation > 0.0f) return false;

		EdgeQuery edgeQuery = EdgeIntersectionQuery(col1, col2);
		if (edgeQuery.separation > 0.0f) return false;

		bool refFaceOnA = false;
		bool refFaceOnB = false;
		Collider* refCollider = nullptr;
		Collider* inciCollider = nullptr;
		int refIndex = -1;

		ContactManifold* manifold = new ContactManifold();

		if (std::min(fabs(faceQueryA.separation), fabs(faceQueryB.separation)) < fabs(edgeQuery.separation) + epsilon) {
			// Find Axis of Minimum Penetration
			if (fabs(faceQueryA.separation) < fabs(faceQueryB.separation) + epsilon) {
				// faceQueryA face index is the reference face
				refFaceOnA = true;
				refCollider = col1;
				inciCollider = col2;
				refIndex = faceQueryA.faceIndex;
			}
			else if (fabs(faceQueryB.separation) < fabs(faceQueryA.separation) + epsilon) {
				refFaceOnB = true;
				refCollider = col2;
				inciCollider = col1;
				refIndex = faceQueryB.faceIndex;
			}
		}

		// if Minimum Axis of Penetration is Face Normal
		if (refFaceOnA || refFaceOnB) {
			glm::vec3 refFaceNormal = glm::vec3(0.0f);
			MeshData& incidentMeshData = static_cast<ShapeAABB*>(inciCollider->mpShape)->mMeshData;
			MeshData& referenceMeshData = static_cast<ShapeAABB*>(refCollider->mpShape)->mMeshData;

			refFaceNormal = refCollider->mpBody->mRotationMatrix * referenceMeshData.faces[refIndex].normal;

			// Find incident face
			int incidentIndex = -1;
			float minProjection = std::numeric_limits<float>::infinity();

			for (int i = 0; i < incidentMeshData.faces.size(); ++i) {
				glm::vec3 normalWorld = inciCollider->mpBody->mRotationMatrix * incidentMeshData.faces[i].normal;

				// find most anti-parallel face on the body
				float projection = glm::dot(refFaceNormal, normalWorld);
				if (minProjection > projection) {
					minProjection = projection;
					incidentIndex = i;
				}
			}

			// Get Incident Face Vertices
			std::vector<glm::vec3> incidentPoly = incidentMeshData.GetFacePolygon(incidentIndex);
			for (auto& v : incidentPoly) {
				v = inciCollider->mpBody->mRotationMatrix * (v * inciCollider->mpTr->mScale)
					+ inciCollider->mpBody->mPosition;
			}

			std::vector<glm::vec3>& clippedPoly = incidentPoly;
			int startingEdge = referenceMeshData.faces[refIndex].edge;

			// clip against adjacent faces to the reference face
			for (int i = 0; i < 4; ++i) {
				int twin = referenceMeshData.edges[startingEdge].twin;

				glm::vec3 pointOnFace = referenceMeshData.GetPointOnFace(referenceMeshData.edges[twin].face);
				glm::vec3 faceNormal = referenceMeshData.faces[referenceMeshData.edges[twin].face].normal;

				pointOnFace = refCollider->mpBody->mRotationMatrix * (pointOnFace * refCollider->mpTr->mScale)
					+ refCollider->mpTr->mPosition;
				faceNormal = refCollider->mpBody->mRotationMatrix * faceNormal;

				if (!clippedPoly.empty())
					clippedPoly = ClipPolygon(clippedPoly, faceNormal, pointOnFace);

				startingEdge = referenceMeshData.edges[startingEdge].next;
			}

			// clip against reference face
			glm::vec3 pointOnRef = refCollider->mpBody->mRotationMatrix * (referenceMeshData.GetPointOnFace(refIndex)
				* refCollider->mpTr->mScale)
				+ refCollider->mpBody->mPosition;

			if (!clippedPoly.empty())
				clippedPoly = ClipPolygon(clippedPoly, refFaceNormal, pointOnRef, 0.005f); // This should be accessed from the physics system i.e how much slop we are giving

			Contact deepest;
			deepest.penetrationDepth = std::numeric_limits<float>::max();

			std::vector<Contact> tempContacts;

			// save penetration depth for all points from the reference face
			for (auto& point : clippedPoly) {
				float depth = glm::dot(refFaceNormal, point - pointOnRef);
				if (depth <= 0.0f) {
					Contact c;
					c.penetrationDepth = depth;
					c.point = point;

					tempContacts.emplace_back(c);

					if (deepest.penetrationDepth > depth)
					{
						deepest = c;
					}
				}
			}

			manifold->contactPoints.emplace_back(deepest);

			if (tempContacts.size() <= 4)
			{
				manifold->contactPoints.swap(tempContacts);
			}
			else
			{
				// Select 3 more points out of all available
				Contact furthest, triangle, fourth;
				float maxDistance = -std::numeric_limits<float>::max();
				for (auto& point : tempContacts)
				{
					float distSq = glm::distance2(point.point, deepest.point);
					if (distSq > maxDistance)
					{
						maxDistance = distSq;
						furthest = point;
					}
				}
				manifold->contactPoints.emplace_back(furthest);

				// find largest area triangle
				float maxArea = -std::numeric_limits<float>::max();
				int winding = 0;
				for (auto& point : tempContacts)
				{
					glm::vec3 side1 = manifold->contactPoints[0].point - point.point;
					glm::vec3 side2 = manifold->contactPoints[1].point - point.point;

					float area = 0.5f * glm::dot(glm::cross(side1, side2), refFaceNormal);
					if (abs(area) > maxArea)
					{
						if (area < 0) winding = -1; else winding = 1;
						maxArea = abs(area);
						triangle = point;
					}
				}
				manifold->contactPoints.emplace_back(triangle);

				maxArea = -std::numeric_limits<float>::max();
				glm::vec3 side1(0);
				glm::vec3 side2(0);
				int A = 0, B = 1, C = 2;
				if (winding < 0)
				{
					A = 2; B = 0; C = 1;
				}
				float totalArea = 0.0f, area;
				for (auto& point : tempContacts)
				{
					totalArea = 0.0f;

					side1 = manifold->contactPoints[A].point - point.point;
					side2 = manifold->contactPoints[B].point - point.point;

					area = 0.5f * glm::dot(glm::cross(side1, side2), refFaceNormal);
					if (area < 0)
						totalArea += area;

					side1 = manifold->contactPoints[B].point - point.point;
					side2 = manifold->contactPoints[C].point - point.point;

					area = 0.5f * glm::dot(glm::cross(side1, side2), refFaceNormal);
					if (area < 0)
						totalArea += area;

					side1 = manifold->contactPoints[C].point - point.point;
					side2 = manifold->contactPoints[A].point - point.point;

					area = 0.5f * glm::dot(glm::cross(side1, side2), refFaceNormal);
					if (area < 0)
						totalArea += area;

					if (abs(totalArea) > maxArea)
					{
						maxArea = abs(totalArea);
						fourth = point;
					}
				}
				manifold->contactPoints.emplace_back(fourth);

			}


			// project points onto reference face to calculate rA and rB
			size_t size = manifold->contactPoints.size();
			for (int i = 0; i < size; ++i) {
				glm::vec3 projected = manifold->contactPoints[i].point - refFaceNormal * manifold->contactPoints[i].penetrationDepth;

				manifold->contactPoints[i].rA = projected - refCollider->mpBody->mPosition;
				manifold->contactPoints[i].rB = manifold->contactPoints[i].point - inciCollider->mpBody->mPosition;
			}

			// save contact normal
			manifold->collisionNormal = refFaceNormal;

			// for consistent normal orientation
			if (glm::dot(manifold->collisionNormal, inciCollider->mpBody->mPosition - refCollider->mpBody->mPosition) < 0.0f) {
				manifold->collisionNormal = -manifold->collisionNormal;
			}

			manifold->bodyA = refCollider->mpBody;
			manifold->bodyB = inciCollider->mpBody;
			manifold->frictionConstant = (refCollider->mFriction + inciCollider->mFriction) / 2.0f;
			// push back the contacts into the list of manifolds
			mContacts->push_back(manifold);
		}
		else if (edgeQuery.edgeA != -1) {
			MeshData& md1 = static_cast<ShapeAABB*>(col1->mpShape)->mMeshData;
			MeshData& md2 = static_cast<ShapeAABB*>(col2->mpShape)->mMeshData;

			glm::mat3& Ra = col1->mpBody->mRotationMatrix;
			glm::mat3& Rb = col2->mpBody->mRotationMatrix;

			glm::vec3 Ea = col1->mpTr->mScale;
			glm::vec3 Eb = col2->mpTr->mScale;

			// edge case
			glm::vec3 pA1 = md1.vertices[md1.edges[md1.edges[edgeQuery.edgeA].prev].toVertex].point;
			glm::vec3 pA2 = md1.vertices[md1.edges[edgeQuery.edgeA].toVertex].point;

			pA1 = Ra * (pA1 * Ea) + col1->mpBody->mPosition;
			pA2 = Ra * (pA2 * Ea) + col1->mpBody->mPosition;

			glm::vec3 pB1 = md2.vertices[md2.edges[md2.edges[edgeQuery.edgeB].prev].toVertex].point;
			glm::vec3 pB2 = md2.vertices[md2.edges[edgeQuery.edgeB].toVertex].point;

			pB1 = Rb * (pB1 * Eb) + col2->mpBody->mPosition;
			pB2 = Rb * (pB2 * Eb) + col2->mpBody->mPosition;

			// find the point betweem the edges
			glm::vec3 pointOnA, pointOnB;
			float depth = LineLineIntersect(pA1, pA2, pB1, pB2, &pointOnA, &pointOnB);

			glm::vec3 edgeA = pA2 - pA1;
			glm::vec3 edgeB = pB2 - pB1;

			Contact c;
			c.point = 0.5f * (pointOnA + pointOnB);
			c.penetrationDepth = -depth;

			c.rA = pointOnA - col1->mpBody->mPosition;
			c.rB = pointOnB - col2->mpBody->mPosition;

			manifold->collisionNormal = glm::cross(edgeA, edgeB);

			// for consistent normal orientation
			if (glm::dot(manifold->collisionNormal, col2->mpBody->mPosition - col1->mpBody->mPosition) < 0.0f) {
				manifold->collisionNormal = -manifold->collisionNormal;
			}

			//c->rA = poC - col1->mpBody->mPosition;
			//c->rB = poC - col2->mpBody->mPosition;

			//manifold->collisionNormal = glm::cross(col1->mpBody->mRotationMatrix * col1->meshData.GetEdgeDirection(edgeQuery.edgeA),
				//col2->mpBody->mRotationMatrix * col2->meshData.GetEdgeDirection(edgeQuery.edgeB));

			manifold->bodyA = col1->mpBody;
			manifold->bodyB = col2->mpBody;
			manifold->frictionConstant = (col1->mFriction + col2->mFriction) / 2.0f;
			manifold->contactPoints.emplace_back(c);

			mContacts->push_back(manifold);
		}

		// setting up mass matrix inv here
		manifold->SetupGroundConstraint();

		return true;
	}
	//From Christer ericsons book
	glm::vec3 ClosestPtPointOBB(glm::vec3& centerOfBall, glm::vec3& centerOfBox,std::vector<glm::vec3>& faceNormal,std::vector<float>& extent)
	{
		glm::vec3 d = centerOfBall - centerOfBox;
		// Start result at center of box; make steps from there
		glm::vec3 q = centerOfBox;
		// For each OBB axis...
		for (int i = 0; i < 3; i++) {
			// ...project d onto that axis to get the distance
			// along the axis of d from the box center
			float dist = glm::dot(d, faceNormal[i]);
			// If distance farther than the box extents, clamp to the box
			if (dist > extent[i]) dist = extent[i];
			if (dist < -extent[i]) dist = -extent[i];
			// Step that distance along the axis to get world coordinate
			q += dist * faceNormal[i];
		}
		return q;
	}


	bool SAT::TestBallBoxIntersection(Collider* ball, Collider* box)
	{
		ShapeAABB* boxShape = static_cast<ShapeAABB*>(box->mpShape);
		ShapeCircle* ballShape = static_cast<ShapeCircle*>(ball->mpShape);
		MeshData& md1 = boxShape->mMeshData;
		glm::vec3 contactNormal(0.0f);
		glm::mat3& boxRotationMatrix = box->mpBody->mRotationMatrix;

		glm::vec3 boxposition = box->mpBody->mPosition;
		
		std::vector<glm::vec3> basisVectors;
		std::vector<float> extents;
		extents.push_back(box->mpTr->mScale.x/2.0f);
		extents.push_back(box->mpTr->mScale.y/2.0f);
		extents.push_back(box->mpTr->mScale.z/2.0f);
		basisVectors.push_back((boxRotationMatrix) * glm::vec3(1.0, 0.0, 0.0));
		basisVectors.push_back((boxRotationMatrix) * glm::vec3(0.0, 1.0, 0.0));
		basisVectors.push_back((boxRotationMatrix) * glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 pointOnBoxClosestToBall = ClosestPtPointOBB(ball->mpBody->mPosition, boxposition, basisVectors, extents);

		float distanceToCenter = glm::distance(pointOnBoxClosestToBall, ball->mpBody->mPosition);

		if (distanceToCenter > ballShape->mRadius )
			return false;

		ContactManifold* manifold = new ContactManifold();
		Contact c;

		c.rA = pointOnBoxClosestToBall - boxposition;
		manifold->collisionNormal = glm::normalize(ball->mpBody->mPosition - pointOnBoxClosestToBall);
		c.rB = (-manifold->collisionNormal) * ballShape->mRadius;
		c.penetrationDepth = distanceToCenter - ballShape->mRadius;
		c.point = pointOnBoxClosestToBall;
		manifold->bodyA = box->mpBody;
		manifold->bodyB = ball->mpBody;
		manifold->frictionConstant = (box->mFriction + ball->mFriction) / 2.0f;
		manifold->contactPoints.push_back(c);
		mContacts->push_back(manifold);
		manifold->SetupGroundConstraint();
		return true;
	}

	bool SAT::TestBoxBallIntersection(Collider* box, Collider* ball)
	{
		return TestBallBoxIntersection(ball, box);
	}

	bool SAT::TestBallBallIntersection(Collider* ball1, Collider* ball2)
	{
		ShapeCircle* ballShape1 = static_cast<ShapeCircle*>(ball1->mpShape);
		ShapeCircle* ballShape2 = static_cast<ShapeCircle*>(ball2->mpShape);
		float distanceBetweenCenter = glm::distance(ball1->mpBody->mPosition, ball2->mpBody->mPosition);
		float penetration = distanceBetweenCenter - ballShape1->mRadius - ballShape2->mRadius;
		if (penetration > 0.0f)
			return false;
		glm::vec3 contactNormal = (ball2->mpBody->mPosition - ball1->mpBody->mPosition)/ distanceBetweenCenter;
		glm::vec3 ball1Ra = (contactNormal)* ballShape1->mRadius;
		glm::vec3 ball2Rb = (-contactNormal) * ballShape2->mRadius;
		ContactManifold* manifold = new ContactManifold();
		Contact c;

		c.penetrationDepth = penetration;
		//c.point = ball1Ra;
		c.rA = ball1Ra;
		c.rB = ball2Rb;
		manifold->collisionNormal = contactNormal;

		manifold->bodyA = ball1->mpBody;
		manifold->bodyB = ball2->mpBody;
		manifold->frictionConstant = (ball1->mFriction + ball2->mFriction) / 2.0f;
		manifold->contactPoints.push_back(c);
		mContacts->push_back(manifold);
		manifold->SetupGroundConstraint();
		return true;
	}

	void SAT::ResetContacts()
	{
		for (auto c : *mPrevContacts)
			delete c;

		mPrevContacts->clear();

		for (auto c : *mContacts)
			delete c;

		mContacts->clear();
	}

	void SAT::CopyContacts()
	{
		for (auto c : *mPrevContacts)
			delete c;

		mPrevContacts->clear();

		mPrevContacts = mContacts;

		mContacts = new std::list<ContactManifold*>();
	}

	bool SAT::CheckCollsionAndGenerateContact(Collider* col1, Collider* col2)
	{
		if (col1 != NULL && col1 != NULL)
			return (this->*CollisionFunctions[col1->mpShape->mType][col2->mpShape->mType])(col1, col2);
		else
			return false;
	}


	FaceQuery SAT::FaceIntersectionQuery(Collider* col1, Collider* col2) {
		glm::mat3& Ra = col1->mpBody->mRotationMatrix;
		glm::mat3& RbTrans = glm::transpose(col2->mpBody->mRotationMatrix);

		glm::vec3 Ea = col1->mpTr->mScale;
		glm::vec3 Eb = col2->mpTr->mScale;

		// rotation matrix to convert from A's local to B's local
		//glm::mat3 C = Rb * glm::transpose(Ra);
		glm::mat3 C = RbTrans * Ra;

		FaceQuery fq;
		fq.faceIndex = -1;
		fq.separation = -std::numeric_limits<float>::infinity();

		MeshData& md1 = static_cast<ShapeAABB*>(col1->mpShape)->mMeshData;
		MeshData& md2 = static_cast<ShapeAABB*>(col2->mpShape)->mMeshData;

		glm::vec3 centerA = RbTrans * (col1->mpBody->mPosition - col2->mpBody->mPosition);

		for (int i = 0; i < md1.faces.size(); ++i) {
			glm::vec3 normalInBSpace = C * md1.faces[i].normal;

			glm::vec3 facePointinBSpace = C * (md1.GetPointOnFace(i) * Ea) + centerA;

			glm::vec3 supportPoint = Eb * md2.GetSupport(-normalInBSpace);

			float s = glm::dot(normalInBSpace, supportPoint - facePointinBSpace);
			if (s > fq.separation) {
				fq.separation = s;
				fq.faceIndex = i;
			}
		}

		return fq;
	}

	// Gauss Map Optimisation
	bool isMinkowskiFace(const glm::vec3& A, const glm::vec3& B, const glm::vec3& B_x_A, const glm::vec3& C, const glm::vec3& D, const glm::vec3& D_x_C)
	{
		// Test if arcs AB and CD intersect on the unit sphere 
		float CBA = glm::dot(C, B_x_A);
		float DBA = glm::dot(D, B_x_A);
		float ADC = glm::dot(A, D_x_C);
		float BDC = glm::dot(B, D_x_C);

		return CBA * DBA < 0.0f && ADC * BDC < 0.0f && CBA * BDC > 0.0f;
	}


	float Project(glm::vec3 p1, glm::vec3 edge1, glm::vec3 p2, glm::vec3 edge2, glm::vec3 center) {
		const float kTol = 0.005f;
		glm::vec3 edge1Xedge2 = glm::cross(edge1, edge2);
		float L = glm::length(edge1Xedge2);
		if (L < kTol * sqrtf(glm::length2(edge1) * glm::length2(edge2))) {
			return -std::numeric_limits<float>::infinity();
		}

		glm::vec3 norm = edge1Xedge2 / L;
		if (glm::dot(norm, p1 - center) < 0.0f) {
			norm = -norm;
		}

		return glm::dot(norm, p2 - p1);
	}

	EdgeQuery SAT::EdgeIntersectionQuery(Collider* col1, Collider* col2) {
		glm::mat3& Ra = col1->mpBody->mRotationMatrix;
		glm::mat3& RbTrans = glm::transpose(col2->mpBody->mRotationMatrix);

		glm::vec3 Ea = col1->mpTr->mScale;
		glm::vec3 Eb = col2->mpTr->mScale;

		// rotation matrix to convert from A's local to B's local
		glm::mat3 C = RbTrans * Ra;

		glm::vec3 centerA = RbTrans * (col1->mpBody->mPosition - col2->mpBody->mPosition);

		EdgeQuery eq;
		eq.edgeA = -1;
		eq.edgeB = -1;
		eq.separation = -std::numeric_limits<float>::infinity();

		MeshData& md1 = static_cast<ShapeAABB*>(col1->mpShape)->mMeshData;
		MeshData& md2 = static_cast<ShapeAABB*>(col2->mpShape)->mMeshData;

		for (int i = 0; i < md1.edges.size(); i += 2) {
			glm::vec3 edge1Dir = C * (Ea * md1.GetEdgeDirection(i));
			glm::vec3 p1 = C * (md1.vertices[md1.edges[md1.edges[i].prev].toVertex].point * Ea) + centerA;

			glm::vec3 u1 = C * md1.faces[md1.edges[i].face].normal;
			glm::vec3 v1 = C * md1.faces[md1.edges[i + 1].face].normal;

			assert(i + 1 == md1.edges[i].twin);

			for (int j = 0; j < md2.edges.size(); j += 2) {
				glm::vec3 edge2Dir = Eb * md2.GetEdgeDirection(j);
				glm::vec3 p2 = Eb * md2.vertices[md2.edges[md2.edges[j].prev].toVertex].point;

				glm::vec3 u2 = md2.faces[md2.edges[j].face].normal;
				glm::vec3 v2 = md2.faces[md2.edges[j + 1].face].normal;

				// check arcs intersection on gauss map to cull edge checks
				if (isMinkowskiFace(u1, v1, -edge1Dir, -u2, -v2, -edge2Dir)) {
					float s = Project(p1, edge1Dir, p2, edge2Dir, centerA);
					if (s > eq.separation) {
						eq.separation = s;
						eq.edgeA = i;
						eq.edgeB = j;
					}
				}
			}
		}

		return eq;
	}
}
