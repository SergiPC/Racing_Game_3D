#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration ------------------------------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "3D Physics Playground"

// Colors --------------------------------------
#define COLOR_KINKY_GREY 0.6784, 0.4509, 0.4509
#define COLOR_RED_GREY 0.4392, 0.2588, 0.2588
#define COLOR_GREY 0.5254, 0.5254, 0.5254
#define COLOR_METALLIC_YELLOW 0.6352, 0.6196, 0.4588
#define COLOR_METALLIC_GREEN 0.6196, 0.6667, 0.6274
#define COLOR_METALLIC_RED 0.6196, 0.5607, 0.5254
#define COLOR_BACKGROUND_PURPLE 0.2157, 0.1960, 0.2353
#define COLOR_BACKGROUND_BLUE 0.1745, 0.1745, 0.2529
#define COLOR_BACKGROUND_RED 0.3529, 0.2745, 0.2745
#define COLOR_BACKGROUND_YELLOW 0.3529, 0.3529, 0.2745

// Max Laps ------------------------------------
#define MAX_LAPS 1