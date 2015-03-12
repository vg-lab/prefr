#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include <string.h>

class FPS
{
public:
	FPS(void);

	void updateAndPrint(void);
	void printText(float _x, float _y, float _z, char * _format, ...);
	void print(void);
	~FPS(void);

	float fps;
	int frameCount;
	int baseTime;
	GLvoid *font_style;


};

