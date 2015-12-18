#pragma once

#include "PhysBody3D.h"
#include "Primitive.h"

class AirRide3D : public PhysBody3D
{
public:
	AirRide3D(btRigidBody* body);
	~AirRide3D();

	void ApplyForce(vec3 force, vec3 pos);

private:
	PhysBody3D* body;
};