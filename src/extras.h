#pragma once
#ifndef EXTRAS_H
#define EXTRAS_H
#include "../wolf/wolf.h"

struct Vertex
{
	GLfloat x,y,z;
	GLubyte r,g,b,a;
};

struct Plane{
	float a, b, c, d;
	Plane() : a(0.0f), b(0.0f), c(0.0f), d(0.0f) {}

	glm::vec3 getNormal(){return glm::vec3(a,b,c);}

	float getDistance(const glm::vec3& point) const{return (a*point.x + b*point.y + c*point.z + d);}
};

enum class BoundingType{
	SPHERE,
	AABB,
	OBB
};

#endif