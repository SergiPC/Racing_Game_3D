#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet\src\btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

// ---------------------------------------------------------
void PhysBody3D::SetAngularSpeed(float s_x, float s_y, float s_z)
{
	body->setLinearVelocity(btVector3(s_x, s_y, s_z));
}

// ---------------------------------------------------------
void PhysBody3D::SetLinearSpeed(float s_x, float s_y, float s_z)
{
	body->setAngularVelocity(btVector3(s_x, s_y, s_z));
}

// ---------------------------------------------------------
void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if(this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if(is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() &~ btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void PhysBody3D::SetBegginingPos(float angle, float axis_x, float axis_y, float axis_z, float pos_x, float pos_y, float pos_z)
{
	mat4x4 tmp_mat;
	tmp_mat.rotate(angle, {axis_x, axis_y, axis_z});
	this->SetTransform(&tmp_mat);

	this->SetAngularSpeed(0, 0, 0);
	this->SetLinearSpeed(0, 0, 0);
	this->SetPos(pos_x, pos_y, pos_z);
}

// ---------------------------------------------------------
bool PhysBody3D::IsSensor() const
{
	return is_sensor;
}

// ---------------------------------------------------------
void PhysBody3D::ApplyForce(float force_x, float force_y, float force_z, float axis_x, float axis_y, float axis_z)
{
	body->applyForce({force_x, force_y, force_z}, {axis_x, axis_y, axis_z});
}