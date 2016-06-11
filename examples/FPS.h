/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef __PREFR_EXAMPLES_FPS__
#define __PREFR_EXAMPLES_FPS__

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


#endif /* __PREFR_EXAMPLES_FPS__ */
