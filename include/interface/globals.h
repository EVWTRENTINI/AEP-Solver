#pragma once
#include "raylib.h"

float raioNo = 5.0f;
Color green = {173, 204, 96, 255};
Color yellow = {241, 215, 0, 255};
Color branco = {255, 255, 255, 255};
Color darkGreen = {43, 51, 24, 255};
float loadScale = 20; // pixel/force unit
float normalScale = 20; // pixel/force unit
float shearScale = 20; // pixel/force unit
float bendingScale = 20; // pixel/(force*distance) unit

int mainMenuSize = 24;
int topMenuSize = 120;
int leftMenuSize = 100;
int rightMenuSize = 0;
int bottomMenuSize = 0;
int topPadding = mainMenuSize + topMenuSize;

int fontsize = 24;
Font fontTtf;