#pragma once
#include "Shape.h"

class Plane :
	public Shape
{
public:
	Plane(const Vector3f& center, const Vector3f& normal, float width, float height, const Material& material);
	Plane(const Vector3f& center, const Vector3f& normal, float width, float height);

	virtual ~Plane();
	
	virtual RayHitData getRayHitResult(const Vector3f& source, const Vector3f& vec, AmbientLight& ambient, std::list<Light*>& lights);
	virtual bool doesRayHit(const Vector3f& source, const Vector3f& vec);


	float getWidth();
	float getHeight();

protected:
	virtual Vector3f getNormal(const Vector3f& point);

	Vector3f normal;
	float width;
	float height;

};

