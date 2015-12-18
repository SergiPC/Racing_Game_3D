#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	r_ang = 0;
	wins01 = 0;
	wins02 = 0;
	time_ms = 0;
	time_s = 0;
	time_m = 0;
	real_timer.Stop();
	delay_on = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(30.0f, 90.0f, 30.0f));

	fx_win = App->audio->LoadFx("Music/Fx_Checklist_w.wav");
	App->audio->PlayMusic("Music/Music_Metal2_w.ogg", 1.0f);


	// GOAL
	// -----------------------------------
	float posgoal_x = 0.0f;
	float posgoal_y = 13.0f;
	float posgoal_z = 35.5f;
	// Cylinder 01 -----------------------
	goal_bar01.height = 17.0f;
	goal_bar01.radius = 0.5f;
	goal_bar01.color.Set(COLOR_GREY);
	goal_bar01.SetPos(posgoal_x - 12.0f, posgoal_y, posgoal_z);
	goal_bar01.SetRotation(90, { 0, 0, 1 });
	// Cylinder 02 -----------------------
	goal_bar02.height = 17.0f;
	goal_bar02.radius = 0.5f;
	goal_bar02.color.Set(COLOR_GREY);
	goal_bar02.SetPos(posgoal_x + 12.0f, posgoal_y, posgoal_z);
	goal_bar02.SetRotation(90, { 0, 0, 1 });
	// Cube 01 ---------------------------
	goal_flag.size = vec3(26, 5, 0.1);
	goal_flag.SetPos(posgoal_x, posgoal_y + 10.0f, posgoal_z);
	goal_flag.color.Set(COLOR_GREY);
	// Cube Sensor Goal ------------------
	goal_sensor_c.size = vec3(22, 14, 0.1);
	goal_sensor_c.SetPos(posgoal_x, posgoal_y, posgoal_z);
	// Sensor Goal -----------------------
	goal_sensor = App->physics->AddBody(goal_sensor_c, 0.0f);
	goal_sensor->SetAsSensor(true);
	goal_sensor->collision_listeners.add(App->player);


	// GOAL CHECKPOINT
	// Cube Sensor -----------------------
	checkpoint.size = vec3(0.1, 9, 18);
	checkpoint.SetPos(-53, 13, 60);
	// Sensor Check ----------------------
	check_sensor = App->physics->AddBody(checkpoint, 0.0f);
	check_sensor->SetAsSensor(true);
	check_sensor->collision_listeners.add(App->player);


	// ROAD CUBES
	// ----------------------------------
	c01.size = vec3(25, 5, 70);
	c01.color.Set(COLOR_GREY);
	c01.SetPos(0.0f, 2.5f, 35.0f);
	sensor_c01 = App->physics->AddBody(c01, 0.0f);
	// ----------------------------------
	c02.size = vec3(25, 5, 55);
	c02.color.Set(COLOR_GREY);
	c02.SetPos(-26.0f, 2.5f, -12.0f);
	c02.SetRotation(65, { 0, 1, 0 });
	sensor_c02 = App->physics->AddBody(c02, 0.0f);
	// ----------------------------------
	c03.size = vec3(25, 5, 55);
	c03.color.Set(COLOR_GREY);
	c03.SetPos(-72, 2.5f, -12.0f);
	c03.SetRotation(115, { 0, 1, 0 });
	sensor_c03 = App->physics->AddBody(c03, 0.0f);
	// ----------------------------------
	c04.size = vec3(25, 5, 50);
	c04.color.Set(COLOR_GREY);
	c04.SetPos(-98.0f, 2.5f, 25.0f);
	sensor_c04 = App->physics->AddBody(c04, 0.0f);
	// ----------------------------------
	c05.size = vec3(20, 5, 37);
	c05.color.Set(COLOR_GREY);
	c05.SetPos(-67.0f, 2.5f, 40.0f);
	c05.SetRotation(90, { 0, 1, 0 });
	sensor_c05 = App->physics->AddBody(c05, 0.0f);
	// ----------------------------------
	c06.size = vec3(20, 5, 30);
	c06.color.Set(COLOR_GREY);
	c06.SetPos(-38.0f, 2.5f, 37.0f);
	c06.SetRotation(-20, { 1, 0, 0 });
	sensor_c06 = App->physics->AddBody(c06, 0.0f);
	// ----------------------------------
	c07.size = vec3(20, 5, 20);
	c07.color.Set(COLOR_GREY);
	c07.SetPos(-38.0f, 7.5f, 60.0f);
	sensor_c07 = App->physics->AddBody(c07, 0.0f);
	// ----------------------------------
	c08.size = vec3(20, 5, 37);
	c08.color.Set(COLOR_GREY);
	c08.SetPos(-67.0f, 2.5f, 60.0f);
	c08.SetRotation(90, { 0, 1, 0 });
	sensor_c08 = App->physics->AddBody(c08, 0.0f);
	// ----------------------------------
	c09.size = vec3(25, 5, 40);
	c09.color.Set(COLOR_GREY);
	c09.SetPos(-98.0f, 2.5f, 70.0f);
	sensor_c09 = App->physics->AddBody(c09, 0.0f);
	// ----------------------------------
	c10.size = vec3(20, 5, 98);
	c10.color.Set(COLOR_GREY);
	c10.SetPos(-36.5f, 2.5f, 80.0f);
	c10.SetRotation(90, { 0, 1, 0 });
	sensor_c10 = App->physics->AddBody(c10, 0.0f);


	// WALL CUBES
	// ----------------------------------
	w01.size = vec3(1, 10, 90);
	w01.color.Set(COLOR_RED_GREY);
	w01.SetPos(12.5f, 5.0f, 45.0f);
	sensor_w01 = App->physics->AddBody(w01, 0.0f);
	// ----------------------------------
	w02.size = vec3(1, 10, 62);
	w02.color.Set(COLOR_RED_GREY);
	w02.SetPos(-12.5f, 5.0f, 39.0f);
	sensor_w02 = App->physics->AddBody(w02, 0.0f);
	// ----------------------------------
	w03.size = vec3(1, 10, 55);
	w03.color.Set(COLOR_RED_GREY);
	w03.SetPos(-20.0f, 5.0f, -23.5f);
	w03.SetRotation(65, { 0, 1, 0 });
	sensor_w03 = App->physics->AddBody(w03, 0.0f);
	// ----------------------------------
	w04.size = vec3(1, 10, 40);
	w04.color.Set(COLOR_RED_GREY);
	w04.SetPos(-30.5f, 5.0f, 0.0f);
	w04.SetRotation(65, { 0, 1, 0 });
	sensor_w04 = App->physics->AddBody(w04, 0.0f);
	// ----------------------------------
	w05.size = vec3(1, 10, 40);
	w05.SetPos(-67.5f, 5.0f, 0.0f);
	w05.SetRotation(115, { 0, 1, 0 });
	w05_1.size = vec3(1, 0.5f, 40);
	w05_1.color.Set(COLOR_RED_GREY);
	w05_1.SetPos(-67.5f, 9.75f, 0.0f);
	w05_1.SetRotation(115, { 0, 1, 0 });
	w05_2.size = vec3(1, 0.5f, 40);
	w05_2.color.Set(COLOR_RED_GREY);
	w05_2.SetPos(-67.5f, 7.5f, 0.0f);
	w05_2.SetRotation(115, { 0, 1, 0 });
	sensor_w05 = App->physics->AddBody(w05, 0.0f);
	// ----------------------------------
	w06.size = vec3(1, 10, 55);
	w06.color.Set(COLOR_RED_GREY);
	w06.SetPos(-78.0f, 5.0f, -23.5f);
	w06.SetRotation(115, { 0, 1, 0 });
	sensor_w06 = App->physics->AddBody(w06, 0.0f);
	// ----------------------------------
	w07.size = vec3(1, 10, 22);
	w07.SetPos(-85.5f, 5.0f, 19.0f);
	w07_1.size = vec3(1, 0.5f, 22);
	w07_1.color.Set(COLOR_RED_GREY);
	w07_1.SetPos(-85.5f, 9.75f, 19.0f);
	w07_2.size = vec3(1, 0.5f, 22);
	w07_2.color.Set(COLOR_RED_GREY);
	w07_2.SetPos(-85.5f, 7.5f, 19.0f);
	sensor_w07 = App->physics->AddBody(w07, 0.0f);
	// ----------------------------------
	w08.size = vec3(1, 10, 90);
	w08.color.Set(COLOR_RED_GREY);
	w08.SetPos(-110.5f, 5.0f, 45.0f);
	sensor_w08 = App->physics->AddBody(w08, 0.0f);
	// ----------------------------------
	w09.size = vec3(1, 10, 63);
	w09.color.Set(COLOR_RED_GREY);
	w09.SetPos(-79.5f, 5.0f, 50.0f);
	w09.SetRotation(90, { 0, 1, 0 });
	sensor_w09 = App->physics->AddBody(w09, 0.0f);
	// ----------------------------------
	w10.size = vec3(1, 10, 30);
	w10.color.Set(COLOR_RED_GREY);
	w10.SetPos(-48.5f, 5.0f, 35.0f);
	sensor_w10 = App->physics->AddBody(w10, 0.0f);
	// ----------------------------------
	w11.size = vec3(1, 10, 13);
	w11.color.Set(COLOR_RED_GREY);
	w11.SetPos(-79.5f, 5.0f, 30.0f);
	w11.SetRotation(90, { 0, 1, 0 });
	sensor_w11 = App->physics->AddBody(w11, 0.0f);
	// ----------------------------------
	w12.size = vec3(1, 15, 35);
	w12.color.Set(COLOR_RED_GREY);
	w12.SetPos(-28.0f, 7.5f, 52.5f);
	sensor_w12 = App->physics->AddBody(w12, 0.0f);
	// ----------------------------------
	w13.size = vec3(1, 15, 76);
	w13.color.Set(COLOR_RED_GREY);
	w13.SetPos(-50.0f, 7.5f, 70.0f);
	w13.SetRotation(90, { 0, 1, 0 });
	sensor_w13 = App->physics->AddBody(w13, 0.0f);
	// ----------------------------------
	w14.size = vec3(1, 10, 124);
	w14.color.Set(COLOR_RED_GREY);
	w14.SetPos(-49.0f, 5.0f, 90.0f);
	w14.SetRotation(90, { 0, 1, 0 });
	sensor_w14 = App->physics->AddBody(w14, 0.0f);
	// ----------------------------------
	w15.size = vec3(1, 10, 15);
	w15.color.Set(COLOR_RED_GREY);
	w15.SetPos(8.5f, 5.0f, -6.0f);
	w15.SetRotation(33, { 0, 1, 0 });
	sensor_w15 = App->physics->AddBody(w15, 0.0f);
	// ----------------------------------
	w16.size = vec3(1, 10, 15);
	w16.color.Set(COLOR_RED_GREY);
	w16.SetPos(-106.5f, 5.0f, -6.0f);
	w16.SetRotation(-33, { 0, 1, 0 });
	sensor_w16 = App->physics->AddBody(w16, 0.0f);
	// ----------------------------------
	w17.size = vec3(1, 10, 9);
	w17.color.Set(COLOR_RED_GREY);
	w17.SetPos(-49.0f, 5.0f, -35.0f);
	w17.SetRotation(-90, { 0, 1, 0 });
	sensor_w17 = App->physics->AddBody(w17, 0.0f);
	// ----------------------------------
	w18.size = vec3(1, 10, 22);
	w18.SetPos(-72.5f, 5.0f, 19.5f);
	w18_1.size = vec3(1, 0.5f, 22);
	w18_1.color.Set(COLOR_RED_GREY);
	w18_1.SetPos(-72.5f, 9.75f, 19.5f);
	w18_2.size = vec3(1, 0.5f, 22);
	w18_2.color.Set(COLOR_RED_GREY);
	w18_2.SetPos(-72.5f, 7.5f, 19.5f);
	sensor_w18 = App->physics->AddBody(w18, 0.0f);
	// ----------------------------------
	w19.size = vec3(1, 10, 34);
	w19.SetPos(-57.5f, 5.0f, 1.5f);
	w19.SetRotation(115, { 0, 1, 0 });
	w19_1.size = vec3(1, 0.5f, 34);
	w19_1.color.Set(COLOR_RED_GREY);
	w19_1.SetPos(-57.5f, 9.75f, 1.5f);
	w19_1.SetRotation(115, { 0, 1, 0 });
	w19_2.size = vec3(1, 0.5f, 34);
	w19_2.color.Set(COLOR_RED_GREY);
	w19_2.SetPos(-57.5f, 7.5f, 1.5f);
	w19_2.SetRotation(115, { 0, 1, 0 });
	sensor_w19 = App->physics->AddBody(w19, 0.0f);
	// ----------------------------------
	w20.size = vec3(1, 10, 35);
	w20.SetPos(-28.0f, 5.0f, 18.5f);
	w20_1.size = vec3(1, 0.5f, 35);
	w20_1.color.Set(COLOR_RED_GREY);
	w20_1.SetPos(-28.0f, 9.75f, 18.5f);
	w20_2.size = vec3(1, 0.5f, 35);
	w20_2.color.Set(COLOR_RED_GREY);
	w20_2.SetPos(-28.0f, 7.5f, 18.5f);
	sensor_w20 = App->physics->AddBody(w20, 0.0f);


	// FLOOR
	// ----------------------------------
	floor.size = vec3(1000, 1, 1000);
	floor.SetPos(0.0f, 0.0f, 0.0f);
	floor_sensor = App->physics->AddBody(floor, 0.0f);
	floor_sensor->SetAsSensor(true);
	floor_sensor->collision_listeners.add(App->player);


	// CYLINDER ENGINES
	// ----------------------------------
	y01.height = 70.0f;
	y01.radius = 2.0f;
	y01.color.Set(COLOR_GREY);
	y01.SetPos(-10.0f, -6, 60.0f);


	// CLINDERS
	// ----------------------------------
	clinder_array.PushBack(App->physics->AddCubylinder16({ 0, 1, 0 }, { 0, 2.5f, 0 }, 12.5, 5.5, COLOR_METALLIC_YELLOW, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 0, 1, 0 }, { -49, 2.5f, -22.75f }, 12.5, 5.5, COLOR_METALLIC_GREEN, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 0, 1, 0 }, { -98.0f, 2.5f, 0 }, 12.5, 5.5, COLOR_METALLIC_RED, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 0, 1, 0 }, { -49, 2.5f, 20.0f }, 25, 5.5, COLOR_METALLIC_GREEN, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 1, 0, 0 }, { -53.0f, 0, 60.0f }, 10, 10, COLOR_METALLIC_YELLOW, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 1, 0, 0 }, { -63.0f, -1.5f, 60.0f }, 10, 10, COLOR_METALLIC_GREEN, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 1, 0, 0 }, { -73.0f, -3, 60.0f }, 10, 10, COLOR_METALLIC_RED, 0.15f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 1, 0, 0 }, { -8.33f, -6, 60.0f }, 13, 8.33f, COLOR_METALLIC_RED, 0.05f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 1, 0, 0 }, { 0.0f, -6, 60.0f }, 14.5, 8.33f, COLOR_METALLIC_GREEN, 0.05f));
	clinder_array.PushBack(App->physics->AddCubylinder16({ 1, 0, 0 }, { 8.33f, -6, 60.0f }, 13, 8.33f, COLOR_METALLIC_YELLOW, 0.05f));


	// COLLIDERS ENGINES
	// -----------------------------------
	coll_eng01.size = vec3(10, 5, 20);
	coll_eng01.SetPos(-53, 5, 60);
	sensor_eng01 = App->physics->AddBody(coll_eng01, 0.0f);
	// -----------------------------------
	coll_eng02.size = vec3(10, 5, 20);
	coll_eng02.SetPos(-63, 4, 60);
	sensor_eng02 = App->physics->AddBody(coll_eng02, 0.0f);


	// STAGE COLUMNS
	// ----------------------------------
	column01.height = 40.0f;
	column01.radius = 12.0f;
	column01.color.Set(COLOR_GREY);
	column01.SetPos(0, -21, 0);
	column01.SetRotation(90, { 0, 0, 1 });
	// ----------------------------------
	column02.height = 40.0f;
	column02.radius = 12.0f;
	column02.color.Set(COLOR_GREY);
	column02.SetPos(0, -21, 80);
	column02.SetRotation(90, { 0, 0, 1 });
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	r_ang += 0.5f;

	// GOAL RENDER ----------------------
	goal_bar01.Render();
	goal_bar02.Render();
	goal_flag.Render();
	goal_bar01.Render();
	goal_bar02.Render();
	goal_flag.Render();

	// ROAD RENDER ----------------------
	c01.Render();
	c02.Render();
	c03.Render();
	c04.Render();
	c05.Render();
	c06.Render();
	c07.Render();
	c08.Render();
	c09.Render();
	c10.Render();

	// WALLS RENDER ----------------------
	w01.Render();
	w02.Render();
	w03.Render();
	w04.Render();
	w05_1.Render();
	w05_2.Render();
	w06.Render();
	w07_1.Render();
	w07_2.Render();
	w08.Render();
	w09.Render();
	w10.Render();
	w11.Render();
	w12.Render();
	w13.Render();
	w14.Render();
	w15.Render();
	w16.Render();
	w17.Render();
	w18_1.Render();
	w18_2.Render();
	w19_1.Render();
	w19_2.Render();
	w20_1.Render();
	w20_2.Render();

	// CYLINDER RENDER -------------------
	y01.Render();

	// STAGE ----------------------------
	column01.Render();
	column02.Render();

	// CLINDERS UPDATE -------------------
	for (int i = 0; i < clinder_array.Count(); i++)
	{
		if (i < 4)
		{
			if (i % 2 == 0)
				clinder_array[i]->Rotate(r_ang, { 0, 1, 0 });
			else
				clinder_array[i]->Rotate(-r_ang, { 0, 1, 0 });
		}

		if (i >= 4)
		{
			if (i % 2 == 0)
				clinder_array[i]->Rotate(r_ang, { 1, 0, 0 });
			else
				clinder_array[i]->Rotate(-r_ang, { 1, 0, 0 });
		}

		clinder_array[i]->Render();
	}

	// SET CAMERA POS --------------------
	vec3 p1 = App->player->GetPosition(1);
	vec3 p2 = App->player->GetPosition(2);
	App->camera->LookAtMidPoint(p1, p2);

	// SET WINDOW TITLE ------------------
	laps01 = App->player->laps01;
	laps02 = App->player->laps02;
	time_ms = ms_timer.Read();
	time_s = s_timer.Read()/1000;
	time_m = m_timer.Read()/(1000*60);
	
	if (time_ms >= 1000)
		ms_timer.Start();

	if (time_s >= 60)
		s_timer.Start();

	char title[500];
	sprintf_s(title, "[[ Player 1    Vel: %.1f Km/h  Lap: %d/%d  Wins: %d]]    Time: %u:%u:%u     [[ Player 2    Vel: %.1f Km/h  Lap: %d/%d  Wins: %d]]",
		App->player->GetKmh(1), laps01, MAX_LAPS, wins01, time_m, time_s, time_ms, App->player->GetKmh(2), laps02, MAX_LAPS, wins02);
	App->window->SetTitle(title);

	// SET DELAY TIMER WIN ---------------
	if (laps01 > MAX_LAPS && delay_on == false || laps02 > MAX_LAPS && delay_on == false)
	{
		if (laps01 > MAX_LAPS)
			wins01++;

		if (laps02 > MAX_LAPS)
			wins02++;

		App->audio->PlayFx(fx_win);
		real_timer.Start();
		delay_on = true;
	}

	delay = real_timer.Read();

	if (delay > 4000)
		WinGame();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::WinGame()
{
	App->player->SetBegginingPos(1);
	App->player->laps01 = 1;
	App->player->SetBegginingPos(2);
	App->player->laps02 = 1;

	ms_timer.Start();
	s_timer.Start();
	m_timer.Start();

	// Delay to 0
	real_timer.Start();
	real_timer.Stop();
}


