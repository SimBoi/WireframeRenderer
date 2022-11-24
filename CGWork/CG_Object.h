#ifndef CG_OBJECT
#define CG_OBJECT

#include "CG_Matrix.h"
#include <list>
#include <windows.h>

#define FAR_PLANE 0
#define NEAR_PLANE 1
#define TOP_PLANE 2
#define BOTTOM_PLANE 3
#define RIGHT_PLANE 4
#define LEFT_PLANE 5

namespace CG
{
	extern double rotation_sensitivity;
	extern double translation_sensitivity;
	extern double scale_sensitivity;

	void setDefaultSensitivity();

	vec4 HomogeneousToEuclidean(vec4& coords);

	class Vertex
	{
	public:
		vec4 localPosition;
		vec4 normal;

		Vertex(vec4& pos = vec4(), vec4& normal = vec4());
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
		vec4 normal;
	};

	class Object
	{
	public:
		mat4 mTransform;
		mat4 wTransform;
		std::list<Face> faces;
		COLORREF color;
		double minX, maxX, minY, maxY, minZ, maxZ; // used for bounding box calculation
		bool hasShape = false; // true if the object has at least one vertex, this value should be manually set
		Face boundingBox[6];
		CG::Object* parent = NULL;
		std::list<Object> children;

		vec4 mPosition() const;
		vec4 wPosition() const;
		void Translate(vec4& amount);
		void LocalTranslate(vec4& amount);
		void RotateX(double angle);
		void RotateY(double angle);
		void RotateZ(double angle);
		void LocalRotateX(double angle);
		void LocalRotateY(double angle);
		void LocalRotateZ(double angle);
		void Scale(vec4& amount);
		void LocalScale(vec4& amount);
		void CalcBoundingBox();
		void ReCalcBoundingBox(const Object& alteredChild); // if a child transforms relative to the parent, the parent should recalculate its bounding box

	private:
		void GenerateBoundingBoxArray();
	};

	class Camera
	{
	public:
		mat4 cTransform;
		mat4 cInverse;
		mat4 projection;
		Plane clipPlanes[6];

		void LookAt(vec4& eye, vec4& at, vec4& up);
		void Ortho(double left, double right, double bottom, double top, double zNear, double zFar);
		void Perspective(double fovY, double aspectRatio, double zNear, double zFar);
		static mat4 ToScreenSpace(double width, double height);
		bool IsInsideFrustum(vec4& p) const;
	};
}

#endif