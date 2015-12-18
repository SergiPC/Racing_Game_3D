#include "AirRide3D.h"

AirRide3D::AirRide3D(btRigidBody* _body) : PhysBody3D(_body)
{
	PhysBody3D* physb = new PhysBody3D(_body);
	body = physb;
}

AirRide3D::~AirRide3D()
{
}

void AirRide3D::ApplyForce(vec3 force, vec3 pos)
{
	body->ApplyForce(force.x, force.y, force.z, pos.x, pos.y, pos.z);
}