#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet\src\btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis.color.Set(info.chassis_color.x, info.chassis_color.y, info.chassis_color.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	// Wings -----------------------------
	Cube chassis_wings(4, 0.5f, 2);
	chassis_wings.color.Set(info.chassis_color.x + 0.2f, info.chassis_color.y + 0.2f, info.chassis_color.z + 0.2f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_wings.transform);
	btVector3 offset1(0, info.chassis_offset.y - 0.5f, -1.0f);
	offset1 = offset1.rotate(q.getAxis(), q.getAngle());

	chassis_wings.transform.M[12] += offset1.getX();
	chassis_wings.transform.M[13] += offset1.getY();
	chassis_wings.transform.M[14] += offset1.getZ();

	// Prow ------------------------------
	Cube chassis_prow(1, 0.5f, 1.5f);
	chassis_prow.color.Set(info.chassis_color.x + 0.2f, info.chassis_color.y + 0.2f, info.chassis_color.z + 0.2f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_prow.transform);
	btVector3 offset2(0, info.chassis_offset.y - 0.5f, 2.75f);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	chassis_prow.transform.M[12] += offset2.getX();
	chassis_prow.transform.M[13] += offset2.getY();
	chassis_prow.transform.M[14] += offset2.getZ();

	// Cabin ----------------------------
	Sphere chassis_cabin(1.0f);
	chassis_cabin.color.Set(info.chassis_color.x + 0.4f, info.chassis_color.y + 0.4f, info.chassis_color.z + 0.4f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_cabin.transform);
	btVector3 offset3(0, info.chassis_offset.y + 0.5f, 0.0f);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	chassis_cabin.transform.M[12] += offset3.getX();
	chassis_cabin.transform.M[13] += offset3.getY();
	chassis_cabin.transform.M[14] += offset3.getZ();

	// Wing2 -----------------------------
	Cube chassis_wing2(0.5f, 1.0f, 0.75f);
	chassis_wing2.color.Set(info.chassis_color.x + 0.2f, info.chassis_color.y + 0.2f, info.chassis_color.z + 0.2f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_wing2.transform);
	btVector3 offset4(0, info.chassis_offset.y + 1.0f, -1.75f);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	chassis_wing2.transform.M[12] += offset4.getX();
	chassis_wing2.transform.M[13] += offset4.getY();
	chassis_wing2.transform.M[14] += offset4.getZ();

	chassis.Render();
	chassis_wings.Render();
	chassis_prow.Render();
	chassis_cabin.Render();
	chassis_wing2.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}