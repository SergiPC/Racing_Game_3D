#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1500.0f
#define TURN_DEGREES 25.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	float GetKmh(int number);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	vec3 GetPosition(int number);
	void SetBegginingPos(int number);

	void InputPlayer01();
	void InputPlayer02();

public:
	uint			fx_goal;

	PhysVehicle3D*	vehicle01;
	float			turn01;
	float			acceleration01;
	float			brake01;
	int				laps01;
	bool			checkpoint01;

	PhysVehicle3D*	vehicle02;
	float			turn02;
	float			acceleration02;
	float			brake02;
	int				laps02;
	bool			checkpoint02;
};