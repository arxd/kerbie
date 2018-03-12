#include <malloc.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "logging.c"
#include "gl.c"
#include "glmath.c"
#include "math.c"
#include "shaders.h"

const char *gl_name = "GRK";


void gl_init(void)
{
}


int gl_frame(void)
{

	return !(GW.cmd & KCMD_ESCAPE);
}

int main_init(int argc, char *argv[])
{
	
	return 0;
}


