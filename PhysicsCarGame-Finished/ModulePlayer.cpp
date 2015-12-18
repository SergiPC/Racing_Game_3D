#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle01(NULL), vehicle02(NULL)
{
	turn01 = acceleration01 = brake01 = 0.0f;
	turn02 = acceleration02 = brake02 = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	fx_goal = App->audio->LoadFx("Music/Fx_Pass_Goal_w.wav");

	// PLAYER 01 -------------------------
	laps01 = 1;
	checkpoint01 = false;
	VehicleInfo car01;
	// car01 properties ----------------------------------------
	car01.chassis_size.Set(2, 1, 4);
	car01.chassis_offset.Set(0, 1.5, 0);
	car01.chassis_color.Set(0.1f, 0.6f, 0.1f);
	car01.mass = 500.0f;
	car01.suspensionStiffness = 15.88f;
	car01.suspensionCompression = 0.83f;
	car01.suspensionDamping = 0.88f;
	car01.maxSuspensionTravelCm = 1000.0f;
	car01.frictionSlip = 1000;
	car01.maxSuspensionForce = 6000.0f;
	// Wheel properties ---------------------------------------
	float connection_height = 1.0f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 0.8f;
	// Don't change anything below this line ------------------
	float half_width = car01.chassis_size.x*0.5f;
	float half_length = car01.chassis_size.z*0.5f;
	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);
	car01.num_wheels = 4;
	car01.wheels = new Wheel[4];
	// FRONT-LEFT ------------------------
	car01.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car01.wheels[0].direction = direction;
	car01.wheels[0].axis = axis;
	car01.wheels[0].suspensionRestLength = suspensionRestLength;
	car01.wheels[0].radius = wheel_radius;
	car01.wheels[0].width = wheel_width;
	car01.wheels[0].front = true;
	car01.wheels[0].drive = true;
	car01.wheels[0].brake = false;
	car01.wheels[0].steering = true;
	// FRONT-RIGHT ------------------------
	car01.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car01.wheels[1].direction = direction;
	car01.wheels[1].axis = axis;
	car01.wheels[1].suspensionRestLength = suspensionRestLength;
	car01.wheels[1].radius = wheel_radius;
	car01.wheels[1].width = wheel_width;
	car01.wheels[1].front = true;
	car01.wheels[1].drive = true;
	car01.wheels[1].brake = false;
	car01.wheels[1].steering = true;
	// REAR-LEFT ------------------------
	car01.wheels[2].connection.Set(half_width + 1.0f * wheel_width, connection_height, -half_length + wheel_radius);
	car01.wheels[2].direction = direction;
	car01.wheels[2].axis = axis;
	car01.wheels[2].suspensionRestLength = suspensionRestLength;
	car01.wheels[2].radius = wheel_radius;
	car01.wheels[2].width = wheel_width;
	car01.wheels[2].front = false;
	car01.wheels[2].drive = false;
	car01.wheels[2].brake = true;
	car01.wheels[2].steering = false;
	// REAR-RIGHT ------------------------
	car01.wheels[3].connection.Set(-half_width - 1.0f * wheel_width, connection_height, -half_length + wheel_radius);
	car01.wheels[3].direction = direction;
	car01.wheels[3].axis = axis;
	car01.wheels[3].suspensionRestLength = suspensionRestLength;
	car01.wheels[3].radius = wheel_radius;
	car01.wheels[3].width = wheel_width;
	car01.wheels[3].front = false;
	car01.wheels[3].drive = false;
	car01.wheels[3].brake = true;
	car01.wheels[3].steering = false;
	// -----------------------------------
	vehicle01 = App->physics->AddVehicle(car01);
	// -----------------------------------
	mat4x4 tmp_mat01;
	tmp_mat01.rotate(180, { 0, 1, 0 });
	vehicle01->SetTransform(&tmp_mat01);
	vehicle01->SetPos(-5, 5, 40);
	
	
	// PLAYER 02 -------------------------
	laps02 = 1;
	checkpoint02 = false;
	car01.chassis_color.Set(0.75f, 0.55f, 0.12f);
	// -----------------------------------
	vehicle02 = App->physics->AddVehicle(car01);
	// -----------------------------------
	mat4x4 tmp_mat02;
	tmp_mat02.rotate(180, { 0, 1, 0 });
	vehicle02->SetTransform(&tmp_mat02);
	vehicle02->SetPos(5, 5, 40);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	InputPlayer01();
	InputPlayer02();

	if (App->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
		SetBegginingPos(1);
	
	if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		SetBegginingPos(2);

	return UPDATE_CONTINUE;
}

void ModulePlayer::InputPlayer01()
{
	turn01 = acceleration01 = brake01 = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		acceleration01 = MAX_ACCELERATION;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		if (turn01 < TURN_DEGREES)
			turn01 += TURN_DEGREES;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		if (turn01 > -TURN_DEGREES)
			turn01 -= TURN_DEGREES;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		acceleration01 = -MAX_ACCELERATION;

	vehicle01->ApplyEngineForce(acceleration01);
	vehicle01->Turn(turn01);
	vehicle01->Brake(brake01);

	vehicle01->Render();
}

void ModulePlayer::InputPlayer02()
{
	turn02 = acceleration02 = brake02 = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
		acceleration02 = MAX_ACCELERATION;

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		if (turn02 < TURN_DEGREES)
			turn02 += TURN_DEGREES;

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
		if (turn02 > -TURN_DEGREES)
			turn02 -= TURN_DEGREES;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
		acceleration02 = -MAX_ACCELERATION;

	vehicle02->ApplyEngineForce(acceleration02);
	vehicle02->Turn(turn02);
	vehicle02->Brake(brake02);

	vehicle02->Render();
}

vec3 ModulePlayer::GetPosition(int number)
{
	mat4x4 m = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	if (number == 1)
		vehicle01->GetTransform(&m);

	if (number == 2)
		vehicle02->GetTransform(&m);

	vec3 pos = m.translation();

	return pos;
}

void ModulePlayer::SetBegginingPos(int number)
{
	if (number == 1)
	{
		vehicle01->SetBegginingPos(180, 0, 1, 0, -5, 5, 40);
		checkpoint01 = false;
	}

	if (number == 2)
	{
		vehicle02->SetBegginingPos(180, 0, 1, 0, 5, 5, 40);
		checkpoint02 = false;
	}	
}

float ModulePlayer::GetKmh(int number)
{
	if (number == 1)
		return vehicle01->GetKmh();

	if (number == 2)
		return vehicle02->GetKmh();
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == vehicle01->pbody)
	{
		if (body1 == App->scene_intro->floor_sensor)
			SetBegginingPos(1);

		if (body1 == App->scene_intro->goal_sensor && checkpoint01 == true)
		{
			if (laps01 < MAX_LAPS)
				App->audio->PlayFx(fx_goal);

			laps01++;
			checkpoint01 = false;
		}

		if (body1 == App->scene_intro->check_sensor && checkpoint01 == false)
			checkpoint01 = true;
	}

	if (body2 == vehicle02->pbody)
	{
		if (body1 == App->scene_intro->floor_sensor)
			SetBegginingPos(2);

		if (body1 == App->scene_intro->goal_sensor && checkpoint02 == true)
		{
			if (laps02 < MAX_LAPS)
				App->audio->PlayFx(fx_goal);

			laps02++;
			checkpoint02 = false;
		}

		if (body1 == App->scene_intro->check_sensor && checkpoint02 == false)
			checkpoint02 = true;
	}
}