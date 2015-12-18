#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetAngularSpeed(float s_x, float s_y, float s_z);
	void SetLinearSpeed(float s_x, float s_y, float s_z);
	void SetAsSensor(bool is_sensor);
	void SetBegginingPos(float angle, float axis_x, float axis_y, float axis_z, float pos_x, float pos_y, float pos_z);
	bool IsSensor() const;
	void ApplyForce(float force_x, float force_y, float force_z, float axis_x, float axis_y, float axis_z);

private:
	btRigidBody* body = nullptr;
	bool is_sensor = false;

public:
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__