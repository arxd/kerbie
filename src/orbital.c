#ifndef ORBITAL_C
#define ORBITAL_C

#include "math.c"
#include <gmp.h>

#define MAX_BODIES 32
#define GCONST (6.67408e-11)

typedef struct s_Body Body;

struct s_Body {
	V1 mass;
	V1 radius;
	V1 a; // 
	V1 va; // angular velocity
	
	V2 xy; // approximate position
	mpf_t x; // precise d
	mpf_t y; // precise y
	
	V2 v; // approximate velocity
	mpf_t vx; //precise velocity x
	mpf_t vy; //precise velocity y	
	
};

struct s_Universe {
	int nbodies;
	Body bodies[MAX_BODIES];
};


void bod_init(Body *self);
void bod_fini(Body *self);


#if __INCLUDE_LEVEL__ == 0

#include <string.h>

void bod_init(Body *self)
{
	memset(self, 0, sizeof(Body));
	mpf_init2 (self->x, 256);
	mpf_init2 (self->y, 256);
	mpf_init2 (self->vx, 256);
	mpf_init2 (self->vy, 256);
}

void bod_fini(Body *self)
{
	mpf_clear(self->x);
	mpf_clear(self->y);
	mpf_clear(self->vx);
	mpf_clear(self->vy);	
}



#endif
#endif
