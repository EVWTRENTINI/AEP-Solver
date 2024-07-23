#pragma once
#include "raylib.h"

float raioNo = 5.0f;
Color green = {173, 204, 96, 255};
Color yellow = {241, 215, 0, 255};
Color branco = {255, 255, 255, 255};
Color darkGreen = {43, 51, 24, 255};
Color forceColor {212, 0, 0, 255};
Color momentColor {204, 0, 255, 255};
Color momentReactionColor {215, 125, 224, 255};

Color normalFill    {255,   0,   0, 30};
Color normalBorder  {255, 141, 141, 255};
Color shearFill     {  3,   0, 255, 30};
Color shearBorder   {182, 182, 255, 255};
Color bendingFill   {182,   0, 255, 30};
Color bendingBorder {235, 191, 255, 255};


float loadScale = 20; // pixel/force unit
float normalScale = 20; // pixel/force unit
float shearScale = 20; // pixel/force unit
float bendingScale = 20; // pixel/(force*distance) unit

int mainMenuSize = 24;
int topMenuSize = 146;
int leftMenuSize = 50;
int rightMenuSize = 0;
int bottomMenuSize = 0;
int topPadding = mainMenuSize + topMenuSize;

int myFontSize = 24;
Font fontTtf;