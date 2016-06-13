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
#include "FPS.h"


FPS::FPS(void)
{
	frameCount=0;
	baseTime=0;
	font_style=GLUT_BITMAP_TIMES_ROMAN_24;
}


FPS::~FPS(void)
{
}

void FPS::updateAndPrint(void)
{
//	char s[50];
	frameCount++;
	int time=glutGet(GLUT_ELAPSED_TIME);

	if(time-baseTime > 1000){
		fps=frameCount*1000.0/(time-baseTime);;
		baseTime=time;
		frameCount=0;
	}
}

void FPS::print(void)
{
	printf("FPS: %4.2f",fps);
}

void FPS::printText(float /*_x*/, float /*_y*/, float /*_z*/, char * format, ...)
{
	int len;
	int i;

	len=(int)strlen(format);
	glColor3f(1.0,0,0);
	glRasterPos2i(0,0);
	for(i=0; i<len; i++){
		glutBitmapCharacter(font_style,format[i]);
	}
}
