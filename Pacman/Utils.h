#pragma once
#include <iostream>
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <filesystem>
#include <conio.h>
#include <queue>
#include "Color.h"

using namespace std;

// function definition -- requires windows.h
void gotoxy(int x, int y);
void clearInput();
void setTextColor(Color colorToSet);
void hideCursor();
