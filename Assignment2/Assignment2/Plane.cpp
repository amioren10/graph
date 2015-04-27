#include "Plane.h"


Plane::Plane(const Vector3f& center, const Vector3f& normal, float width, float height, const Material& material) :Shape(center, material), normal(normal), width(width), height(height)
{
	this->normal.normalize();
	calculateCorners();
}

Plane::Plane(const Vector3f& center, const Vector3f& normal, float width, float height) : Shape(center), normal(normal), width(width), height(height)
{
	this->normal.normalize();
	calculateCorners();
}

Plane::~Plane()
{
}

float Plane::getWidth()
{
	return width;
}

float Plane::getHeight()
{
	return height;
}

RayHitData Plane::getRayHitResult(const Vector3f& source, const Vector3f& vec, AmbientLight& ambient, list<Light*>& lights, list<Shape*>& shapes, int recursiveLevel)
{
	RayHitData rhd;
	//calculate the point of impact in the infinite plane
	Vector3f& q0 = this->center;
	const Vector3f& p0 = source;
	this->getNormal(source, vec*(-1));	//to flip normal if needed

	Vector3f p = p0 + (vec * Vector3f::dotProduct(this->normal, (q0 - p0) / Vector3f::dotProduct(this->normal, vec)));
	//todo: check what happen if the plane is parallel 

	bool isOnWhite = isPointOnWhiteSquare(p);

	if (isOnWhite){
		rhd.intensity = { 0.05, 0.05, 0.05 };
	}
	
	int arr[8] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	for (int i = 0; i < 8; i+=2){
		Vector3f V1 = corners[arr[i]] - source;
		Vector3f V2 = corners[arr[i + 1]] - source;
		Vector3f triangleNormal = Vector3f::crossProduct(V1, V2);
		triangleNormal.normalize();
		if (Vector3f::dotProduct(p - p0, triangleNormal) < 0){
			rhd.isHit = false;
			return rhd;
		}
		
	}
	
	//rhd.
	rhd.isHit = true;
	rhd.pointOfHit = p;
	rhd.distance = (p - p0).getLength();
	rhd.intensity = Shape::calculateIntensity(p, vec, ambient, lights, shapes);
	
	if (isOnWhite){
		float newBlue = rhd.intensity.blue + 0.05;
		float newGreen = rhd.intensity.green + 0.05;
		float newRed = rhd.intensity.red + 0.05;

		rhd.intensity.blue = (newBlue > 1 ? 1 : newBlue);
		rhd.intensity.green = (newGreen > 1 ? 1 : newGreen);
		rhd.intensity.red = (newRed > 1 ? 1 : newRed);

	}
	return rhd;
}

Vector3f Plane::getNormal(const Vector3f& point, const Vector3f& incomingVector)
{
	if (Vector3f::dotProduct(normal, incomingVector) < 0)	this->normal *= -1;
	return this->normal;
}

float Plane::rayHitDistance(const Vector3f& source, const Vector3f& vec)
{
	//todo: simplifiy .. this is a copy paste..
	RayHitData rhd;
	//calculate the point of impact in the infinite plane
	Vector3f& q0 = this->center;
	const Vector3f& p0 = source;

	int factor = 1;
	if (Vector3f::dotProduct(vec, this->normal) >= 0)	factor = -1;

	this->getNormal(source, vec *-1);	//to flip normal if needed
	Vector3f p = p0 + vec * Vector3f::dotProduct(this->normal, (q0 - p0) / Vector3f::dotProduct(this->normal, vec));
	//todo: check what happen if the plane is parallel 

	rhd.isHit = false;
	int arr[8] = { 0, 1, 1, 2, 2, 3, 3, 0 };


	for (int i = 0; i < 8; i += 2){
		Vector3f V1 = corners[arr[i]] - source;
		Vector3f V2 = corners[arr[i + 1]] - source;
		Vector3f triangleNormal = Vector3f::crossProduct(V1, V2);
		
		triangleNormal = triangleNormal * factor;

		triangleNormal.normalize();
		if (Vector3f::dotProduct(p - p0, triangleNormal) < 0){
			rhd.isHit = false;
			return 0;
		}

	}

	//if (!rhd.isHit) return 0;

	rhd.distance = (p - p0).getLength();

	return rhd.distance;
}

void Plane::calculateCorners()
{
	Vector3f horizontalDirection;
	Vector3f verticalDirection;
	normal.getTwoOrthogonals(horizontalDirection, verticalDirection);
	horizontalDirection.normalize();
	verticalDirection.normalize();
	horizontalDirection *= width / 2;
	verticalDirection *= height / 2;

	corners[0] = center + (horizontalDirection + verticalDirection);
	corners[1] = center + ( (horizontalDirection*(-1)) + verticalDirection);
	corners[2] = center + ((horizontalDirection*(-1)) + (verticalDirection*(-1)));
	corners[3] = center + (horizontalDirection + (verticalDirection*(-1)) );
}

#define SQUARE_SIZE 0.2

bool Plane::isPointOnWhiteSquare(const Vector3f& point)
{
	Vector3f directionA = corners[0] - corners[1];
	Vector3f directionB = corners[1] - corners[2];
	directionA.normalize();
	directionB.normalize();
	Vector3f& cornerToPoint = point - corners[0];
	float widthToPoint = Vector3f::dotProduct(cornerToPoint, directionA);
	float heightToPoint = Vector3f::dotProduct(cornerToPoint, directionB);

	int pointCol = widthToPoint / SQUARE_SIZE;
	int pointRow = heightToPoint / SQUARE_SIZE;

	return ((pointCol + pointRow) % 2) == 0;
}
