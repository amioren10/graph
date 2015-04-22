#pragma once
#include "Shape.h"
class Sphere :
	public Shape
{
public:

	Sphere(const Vector3f& center, float radius, const Material& material);
	virtual ~Sphere();

	virtual RayHitData getRayHitResult(const Vector3f& source, const Vector3f& vec);

protected:
	float radius;
};

