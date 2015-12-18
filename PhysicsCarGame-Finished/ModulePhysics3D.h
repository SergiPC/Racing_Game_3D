#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "Primitive.h"

#include "Bullet/src/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
struct PhysBody3D;
struct PhysVehicle3D;
struct VehicleInfo;

struct Cubylinder8
{
	Cubylinder8()
	{

	};
	~Cubylinder8();

	void Render();

	PhysBody3D*		body01;
	PhysBody3D*		body02;
	PhysBody3D*		body03;
	PhysBody3D*		body04;
	Cube			cube01;
	Cube			cube02;
	Cube			cube03;
	Cube			cube04;
};

struct Cubylinder16
{
	Cubylinder16()
	{

	};
	~Cubylinder16();

	void Render();
	void Rotate(float angle, vec3 axis);

	PhysBody3D*		body01;
	PhysBody3D*		body02;
	PhysBody3D*		body03;
	PhysBody3D*		body04;
	PhysBody3D*		body05;
	PhysBody3D*		body06;
	PhysBody3D*		body07;
	PhysBody3D*		body08;

	Cube			cube01;
	Cube			cube02;
	Cube			cube03;
	Cube			cube04;
	Cube			cube05;
	Cube			cube06;
	Cube			cube07;
	Cube			cube08;

	mat4x4			r_mat01;
	mat4x4			r_mat02;
	mat4x4			r_mat03;
	mat4x4			r_mat04;
	mat4x4			r_mat05;
	mat4x4			r_mat06;
	mat4x4			r_mat07;
	mat4x4			r_mat08;
};

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	PhysBody3D* AddBody(const Sphere& sphere, float mass = 1.0f);
	PhysBody3D* AddBody(const Cube& cube, float mass = 1.0f);
	PhysBody3D* AddBody(const Cylinder& cylinder, float mass = 1.0f);
	PhysVehicle3D* AddVehicle(const VehicleInfo& info);
	Cubylinder8* AddCubylinder8(vec3 pos, float radius, float fondue);
	Cubylinder16* AddCubylinder16(vec3 rot_a, vec3 pos, float radius, float fondue, float r, float g, float b, float offset);

	PhysBody3D* AddKinematicBody(const Cylinder& cylinder);

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);

private:

	bool debug;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	btDefaultVehicleRaycaster*			vehicle_raycaster;
	DebugDrawer*						debug_draw;

	p2List<btCollisionShape*> shapes;
	p2List<PhysBody3D*> bodies;
	p2List<btDefaultMotionState*> motions;
	p2List<btTypedConstraint*> constraints;
	p2List<PhysVehicle3D*> vehicles;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
};