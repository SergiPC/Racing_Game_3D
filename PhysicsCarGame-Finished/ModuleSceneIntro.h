#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;
struct Cubylinder8;
struct Cubylinder16;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void WinGame();

public:
	uint		fx_win;

	// TITLE
	int			laps01, laps02, wins01, wins02;
	Timer		ms_timer;
	Timer		s_timer;
	Timer		m_timer;
	Uint32		time_ms;
	Uint32		time_s;
	Uint32		time_m;

	// DELAY
	Timer		real_timer;
	Uint32		delay;
	bool		delay_on;

	// GOAL
	Cylinder	goal_bar01;
	Cylinder	goal_bar02;
	Cube		goal_flag;
	Cube		goal_sensor_c;
	PhysBody3D*	goal_sensor;

	// GOAL CHECKPOINT
	Cube		checkpoint;
	PhysBody3D*	check_sensor;

	// ROAD CUBES
	Cube		c01;
	Cube		c02;
	Cube		c03;
	Cube		c04;
	Cube		c05;
	Cube		c06;
	Cube		c07;
	Cube		c08;
	Cube		c09;
	Cube		c10;

	// WALL CUBES
	Cube		w01;
	Cube		w02;
	Cube		w03;
	Cube		w04;
	Cube		w05;
	Cube		w05_1;
	Cube		w05_2;
	Cube		w06;
	Cube		w07;
	Cube		w07_1;
	Cube		w07_2;
	Cube		w08;
	Cube		w09;
	Cube		w10;
	Cube		w11;
	Cube		w12;
	Cube		w13;
	Cube		w14;
	Cube		w15;
	Cube		w16;
	Cube		w17;
	Cube		w18;
	Cube		w18_1;
	Cube		w18_2;
	Cube		w19;
	Cube		w19_1;
	Cube		w19_2;
	Cube		w20;
	Cube		w20_1;
	Cube		w20_2;

	// FLOOR
	Cube		floor;

	// CYLINDER ENGINES
	Cylinder	y01;
	mat4x4		rmat_y01;

	// CLINDERS
	p2DynArray<Cubylinder16*> clinder_array;

	// COLLIDERS ENGINES
	Cube		coll_eng01;
	Cube		coll_eng02;
	PhysBody3D* sensor_eng01;
	PhysBody3D* sensor_eng02;

	// STAGE COLUMNS
	Cylinder	column01;
	Cylinder	column02;

	// PHYSBODY SENSORS
	PhysBody3D*	sensor_c01;
	PhysBody3D*	sensor_c02;
	PhysBody3D*	sensor_c03;
	PhysBody3D*	sensor_c04;
	PhysBody3D*	sensor_c05;
	PhysBody3D* sensor_c06;
	PhysBody3D* sensor_c07;
	PhysBody3D*	sensor_c08;
	PhysBody3D* sensor_c09;
	PhysBody3D* sensor_c10;

	PhysBody3D* sensor_w01;
	PhysBody3D* sensor_w02;
	PhysBody3D* sensor_w03;
	PhysBody3D* sensor_w04;
	PhysBody3D* sensor_w05;
	PhysBody3D* sensor_w06;
	PhysBody3D* sensor_w07;
	PhysBody3D* sensor_w08;
	PhysBody3D* sensor_w09;
	PhysBody3D* sensor_w10;
	PhysBody3D* sensor_w11;
	PhysBody3D* sensor_w12;
	PhysBody3D* sensor_w13;
	PhysBody3D* sensor_w14;
	PhysBody3D* sensor_w15;
	PhysBody3D* sensor_w16;
	PhysBody3D* sensor_w17;
	PhysBody3D* sensor_w18;
	PhysBody3D* sensor_w19;
	PhysBody3D* sensor_w20;

	PhysBody3D* floor_sensor;

	float		r_ang;
};
