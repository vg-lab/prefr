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
	printf("FPS: %4.2f\n",fps);
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
