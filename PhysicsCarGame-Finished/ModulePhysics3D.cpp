#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Primitive.h"

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/bin/BulletDynamics_vs2010_debug.lib")
	#pragma comment (lib, "Bullet/bin/BulletCollision_vs2010_debug.lib")
	#pragma comment (lib, "Bullet/bin/LinearMath_vs2010_debug.lib")
#else
	#pragma comment (lib, "Bullet/bin/BulletDynamics_vs2010.lib")
	#pragma comment (lib, "Bullet/bin/BulletCollision_vs2010.lib")
	#pragma comment (lib, "Bullet/bin/LinearMath_vs2010.lib")
#endif

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = false;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if(pbodyA && pbodyB)
			{
				p2List_item<Module*>* item = pbodyA->collision_listeners.getFirst();
				while(item)
				{
					item->data->OnCollision(pbodyA, pbodyB);
					item = item->next;
				}

				item = pbodyB->collision_listeners.getFirst();
				while(item)
				{
					item->data->OnCollision(pbodyB, pbodyA);
					item = item->next;
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();

		// Render vehicles
		p2List_item<PhysVehicle3D*>* item = vehicles.getFirst();
		while(item)
		{
			item->data->Render();
			item = item->next;
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for(p2List_item<btTypedConstraint*>* item = constraints.getFirst(); item; item = item->next)
	{
		world->removeConstraint(item->data);
		delete item->data;
	}
	
	constraints.clear();

	for(p2List_item<btDefaultMotionState*>* item = motions.getFirst(); item; item = item->next)
		delete item->data;

	motions.clear();

	for(p2List_item<btCollisionShape*>* item = shapes.getFirst(); item; item = item->next)
		delete item->data;

	shapes.clear();

	for(p2List_item<PhysBody3D*>* item = bodies.getFirst(); item; item = item->next)
		delete item->data;

	bodies.clear();

	for(p2List_item<PhysVehicle3D*>* item = vehicles.getFirst(); item; item = item->next)
		delete item->data;

	vehicles.clear();

	delete vehicle_raycaster;
	delete world;

	return true;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
{
	btCollisionShape* colShape = new btSphereShape(sphere.radius);
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&sphere.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cube.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius, 0.0f));
	shapes.add(colShape);
	
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysVehicle3D* ModulePhysics3D::AddVehicle(const VehicleInfo& info)
{
	btCompoundShape* comShape = new btCompoundShape();
	shapes.add(comShape);

	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x*0.5f, info.chassis_size.y*0.5f, info.chassis_size.z*0.5f));
	shapes.add(colShape);
	
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));

	comShape->addChildShape(trans, colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	comShape->calculateLocalInertia(info.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setActivationState(DISABLE_DEACTIVATION);

	world->addRigidBody(body);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info.frictionSlip;
	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info.suspensionCompression;
	tuning.m_suspensionDamping = info.suspensionDamping;
	tuning.m_suspensionStiffness = info.suspensionStiffness;

	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, body, vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for(int i = 0; i < info.num_wheels; ++i)
	{
		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
	}
	// ---------------------

	PhysVehicle3D* pvehicle = new PhysVehicle3D(body, vehicle, info);
	world->addVehicle(vehicle);
	vehicles.add(pvehicle);

	pvehicle->pbody = new PhysBody3D(body);

	return pvehicle;
}

// ---------------------------------------------------------
Cubylinder8* ModulePhysics3D::AddCubylinder8(vec3 pos, float radius, float fondue)
{
	Cubylinder8* cub = new Cubylinder8();

	float side = radius * 2 * 0.4142;

	cub->cube01.size = { radius * 2, fondue, side };
	cub->cube01.SetPos(pos.x, pos.y, pos.z);
	cub->cube02.size = { radius * 2, fondue, side };
	cub->cube02.SetPos(pos.x, pos.y, pos.z);
	cub->cube02.SetRotation(45, { 0, 1, 0 });
	cub->cube03.size = { radius * 2, fondue, side };
	cub->cube03.SetPos(pos.x, pos.y, pos.z);
	cub->cube03.SetRotation(90, { 0, 1, 0 });
	cub->cube04.size = { radius * 2, fondue, side };
	cub->cube04.SetPos(pos.x, pos.y, pos.z);
	cub->cube04.SetRotation(135, { 0, 1, 0 });

	cub->body01 = AddBody(cub->cube01, 0);
	cub->body02 = AddBody(cub->cube02, 0);
	cub->body03 = AddBody(cub->cube03, 0);
	cub->body04 = AddBody(cub->cube04, 0);

	return cub;
}

// ---------------------------------------------------------
Cubylinder16* ModulePhysics3D::AddCubylinder16(vec3 rot_a, vec3 pos, float radius, float fondue, float r, float g, float b, float offset)
{
	Cubylinder16* cub = new Cubylinder16();

	float side = radius * 2 * 0.1989;

	if (rot_a.x == 0 && rot_a.y == 1 && rot_a.z == 0)
	{
		cub->cube01.size = { radius * 2 + radius*offset, fondue + fondue*0.10f, side };
		cub->cube02.size = { radius * 2, fondue, side };
		cub->cube03.size = { radius * 2 + radius*offset, fondue + fondue*0.10f, side };
		cub->cube04.size = { radius * 2, fondue, side };
		cub->cube05.size = { radius * 2 + radius*offset, fondue + fondue*0.10f, side };
		cub->cube06.size = { radius * 2, fondue, side };
		cub->cube07.size = { radius * 2 + radius*offset, fondue + fondue*0.10f, side };
		cub->cube08.size = { radius * 2, fondue, side };
	}

	if (rot_a.x == 1 && rot_a.y == 0 && rot_a.z == 0)
	{
		cub->cube01.size = { fondue + fondue*0.10f, radius * 2 + radius*offset, side };
		cub->cube02.size = { fondue, radius * 2, side };
		cub->cube03.size = { fondue + fondue*0.10f, radius * 2 + radius*offset, side };
		cub->cube04.size = { fondue, radius * 2, side };
		cub->cube05.size = { fondue + fondue*0.10f, radius * 2 + radius*offset, side };
		cub->cube06.size = { fondue, radius * 2, side };
		cub->cube07.size = { fondue + fondue*0.10f, radius * 2 + radius*offset, side };
		cub->cube08.size = { fondue, radius * 2, side };
	}

	cub->cube01.SetPos(pos.x, pos.y, pos.z);
	cub->cube01.color.Set(r, g, b);
	cub->r_mat01 = cub->cube01.transform;

	cub->cube02.SetPos(pos.x, pos.y, pos.z);
	cub->cube02.color.Set(r, g, b);
	cub->cube02.SetRotation(22.5, rot_a);
	cub->r_mat02 = cub->cube02.transform;

	cub->cube03.SetPos(pos.x, pos.y, pos.z);
	cub->cube03.color.Set(r, g, b);
	cub->cube03.SetRotation(45, rot_a);
	cub->r_mat03 = cub->cube03.transform;

	cub->cube04.SetPos(pos.x, pos.y, pos.z);
	cub->cube04.color.Set(r, g, b);
	cub->cube04.SetRotation(67.5, rot_a);
	cub->r_mat04 = cub->cube04.transform;

	cub->cube05.SetPos(pos.x, pos.y, pos.z);
	cub->cube05.color.Set(r, g, b);
	cub->cube05.SetRotation(90, rot_a);
	cub->r_mat05 = cub->cube05.transform;

	cub->cube06.SetPos(pos.x, pos.y, pos.z);
	cub->cube06.color.Set(r, g, b);
	cub->cube06.SetRotation(112.5, rot_a);
	cub->r_mat06 = cub->cube06.transform;

	cub->cube07.SetPos(pos.x, pos.y, pos.z);
	cub->cube07.color.Set(r, g, b);
	cub->cube07.SetRotation(135, rot_a);
	cub->r_mat07 = cub->cube07.transform;

	cub->cube08.SetPos(pos.x, pos.y, pos.z);
	cub->cube08.color.Set(r, g, b);
	cub->cube08.SetRotation(157.5, rot_a);
	cub->r_mat08 = cub->cube08.transform;

	cub->body01 = AddBody(cub->cube01, 0);
	cub->body02 = AddBody(cub->cube02, 0);
	cub->body03 = AddBody(cub->cube03, 0);
	cub->body04 = AddBody(cub->cube04, 0);
	cub->body05 = AddBody(cub->cube05, 0);
	cub->body06 = AddBody(cub->cube06, 0);
	cub->body07 = AddBody(cub->cube07, 0);
	cub->body08 = AddBody(cub->cube08, 0);

	return cub;
}

PhysBody3D* ModulePhysics3D::AddKinematicBody(const Cylinder& cylinder)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius, 0.0f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.add(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.add(hinge);
	hinge->setDbgDrawSize(2.0f);
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}

// Cubylinder struct --------------------------------
void Cubylinder8::Render()
{
	cube01.Render();
	cube02.Render();
	cube03.Render();
	cube04.Render();
}

void Cubylinder16::Render()
{
	cube01.Render();
	cube02.Render();
	cube03.Render();
	cube04.Render();
	cube05.Render();
	cube06.Render();
	cube07.Render();
	cube08.Render();
}

void Cubylinder16::Rotate(float angle, vec3 axis)
{
	r_mat01.rotate(angle, axis);
	body01->SetTransform(r_mat01.M);
	body01->GetTransform(&cube01.transform);
	
	r_mat02.rotate(angle + 22.5, axis);
	body02->SetTransform(r_mat02.M);
	body02->GetTransform(&cube02.transform);
	
	r_mat03.rotate(angle + 45, axis);
	body03->SetTransform(r_mat03.M);
	body03->GetTransform(&cube03.transform);

	r_mat04.rotate(angle + 67.5, axis);
	body04->SetTransform(r_mat04.M);
	body04->GetTransform(&cube04.transform);

	r_mat05.rotate(angle + 90, axis);
	body05->SetTransform(r_mat05.M);
	body05->GetTransform(&cube05.transform);

	r_mat06.rotate(angle + 112.5, axis);
	body06->SetTransform(r_mat06.M);
	body06->GetTransform(&cube06.transform);

	r_mat07.rotate(angle + 135, axis);
	body07->SetTransform(r_mat07.M);
	body07->GetTransform(&cube07.transform);

	r_mat08.rotate(angle + 157.5, axis);
	body08->SetTransform(r_mat08.M);
	body08->GetTransform(&cube08.transform);
}