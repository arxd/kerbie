#include <malloc.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "logging.c"
#include "gl.c"
#include "glmath.c"
#include "math.c"
#include "shaders.h"

#include <gmp.h>

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
	INFO("GMP %d.%d.%d: %s %s (%d bpl)", 
		__GNU_MP_VERSION, 
		__GNU_MP_VERSION_MINOR, 
		__GNU_MP_VERSION_PATCHLEVEL,
		__GMP_CC, __GMP_CFLAGS, mp_bits_per_limb);
	mpf_t x;
	mpf_init2 (x, 256);
	
	
	mpf_clear(x);
	return 0;
}


