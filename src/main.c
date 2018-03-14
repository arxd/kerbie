#include <malloc.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "logging.c"
#include "gl.c"
#include "glmath.c"
#include "math.c"
#include "shaders.h"
#include "trades.c"
#include "function.c"

#include <gmp.h>

const char *gl_name = "GRK";
V1 prev_vps = 0.0;

typedef struct s_State State;
struct s_State {
	V1 x, v;
	V1 x0, v0;
};

V1 wiggle(V1 t) {
	if (t <= -8.0 || t >= 8.0)
		return 0.5;
	V1 y = - 92274688.0;
	V1 x = t;
	y += x * 786432000.0; x *= t; //x
	y += x * 253624320.0; x *= t; //x^2
	y += x * - 126976000.0; x *= t; //x^3
	y += x * - 34867200.0; x *= t; //x^4
	y += x * 5644800.0; x *= t; //x^5
	y += x * 1445920.0; x *= t; //x^6
	y += x * - 95400.0; x *= t; //x^7
	y += x * - 23490.0; x *= t; //x^8
	y += x * 550.0; x *= t; //x^9
	y += x * 132.0; x *= t; //x^10
	return y;
}



void advance_state_0(State *s, V1 t0, V1 dt)
{
	V1 a0 = wiggle(t0);
	s->x = s->x0 + s->v0*dt;
	s->v = s->v0 + a0*dt;
}

void advance_state_1(State *s, V1 t0, V1 dt)
{
	V1 a0 = wiggle(t0);
	s->x = s->x0 + s->v0*dt + 0.5*a0*dt*dt;
	s->v = s->v0 +  a0*dt;
}
void advance_state_2(State *s, V1 t0, V1 dt)
{
	V1 a0 = wiggle(t0);
	V1 a1 = wiggle( t0 + dt);
	a0 = a0 + (a1-a0)/2.0;
	s->x = s->x0 + s->v0*dt + 0.5*a0*dt*dt;
	s->v = s->v0 +  a0*dt;
}

void advance_state_3(State *s, V1 t0, V1 dt)
{
	V1 a0 = wiggle(t0);
	V1 a1 = wiggle( t0 + dt);
	V1 da = (a1-a0)/dt;
	s->x = s->x0 + s->v0*dt + a0/2.0*dt*dt + da/6.0*dt*dt*dt;
	s->v = s->v0 +  a0*dt + 0.5*da*dt*dt;
}


Function1 Q, F, A, B, V;
//~ FunctionRanged Qr, Fr, Ar, Br;

void gl_init(void)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0 , 0.0, 1.0);
	
	glmath_init();
	
	view_fit(&view, &Q, 1);
	
	
	
	//~ f1_derivative(&F, &Q);
	//~ V1 v0 = F.ys[0];
	//~ v0 = 0.0;
	//~ f1_derivative(&F, &F);

	// TEAL
	f1_init(&A, 40);
	A.dx = (F.len-1)*F.dx/(A.memlen-1);
	A.x0 = F.x0;
	State s;
	s.v = s.x = 0.0;
	for (int i=0;i <A.memlen; ++i) {
		V1 t0 = A.x0 + i*A.dx;
		f1_append(&A, s.x);
		s.v0 = s.v;
		s.x0 = s.x;
		advance_state_2(&s, t0, A.dx);
	}
	
	
	// BLUE
	f1_init(&B, 40);
	B.dx = (F.len-1)*F.dx/(B.memlen-1);
	B.x0 = F.x0;
	s.v = s.x = 0.0;
	for (int i=0;i < B.memlen; ++i) {
		V1 t0 = B.x0 + i*B.dx;
		f1_append(&B, s.x);
		s.v0 = s.v;
		s.x0 = s.x;
		advance_state_3(&s, t0, B.dx);
	}

	
	
	
	fr_register(&Q, rgb(1.0,1.0,0.0));
	fr_register(&A, rgb(0.0, 1.0, 1.0));
	fr_register(&B, rgb(0.5, 0.5, 1.0));
	//~ fr_register(&F, rgb(0.5, 0.5, 0.5));
	//~ fr_register(&F, rgb(0.5, 0.5, 0.5));
	
	//~ fr_init(&Qr, 1024);
	//~ fr_init(&Fr, 1024);
	//~ fr_init(&Ar, 1024);
	//~ fr_init(&Br, 1024);
	
	//~ fr_compile(&Qr, &Q, view.vps.x);
	//~ fr_compile(&Fr, &F, view.vps.x);
	//~ fr_compile(&Ar, &A, view.vps.x);
	//~ fr_compile(&Br, &B, view.vps.x);
	
}


int gl_frame(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	view_navigate(&view, GW.m.btn, 1.25);
	//~ if (view.vps.x != prev_vps) {
		//~ if (view.vps.x < B.dx)
			//~ view_zoom_at(&view, screen_to_view(&view, v2(GW.m.hx, GW.m.hy)), v2(B.dx, view.vps.y));
		//~ fr_compile(&Qr, &Q, view.vps.x);
		//~ fr_compile(&Fr, &F, view.vps.x);
		//~ fr_compile(&Ar, &A, view.vps.x);
		//~ fr_compile(&Br, &B, view.vps.x);
		//~ prev_vps = view.vps.x;
	//~ }

	//~ fr_render(&Qr, &view, rgb(1.0, 1.0, 0.0), 1);
	//~ fr_render(&Fr, &view, rgb(0.0, 1.0, 0.0), 1);
	//~ fr_render(&Ar, &view, rgb(0.0, 1.0, 1.0), 1);
	//~ fr_render(&Br, &view, rgb(0.0, 0.0, 1.0), 1);
	fr_render();
	
	glLineWidth(1.0);
	color = rgb(1.0,1.0,1.0);
	draw_line_strip(v2(0.0, 0.0), v2(1.0, 1.0), 0.0, 2, (GLfloat[]){view.ll.x, 0.0, view.ur.x, 0.0});
	
	return !(GW.cmd & KCMD_ESCAPE);
}

int main_init(int argc, char *argv[])
{
	INFO("GMP %d.%d.%d: %s %s (%d bpl)", 
		__GNU_MP_VERSION, 
		__GNU_MP_VERSION_MINOR, 
		__GNU_MP_VERSION_PATCHLEVEL,
		__GMP_CC, __GMP_CFLAGS, mp_bits_per_limb);

	TradeData td;
	ASSERT(argc >= 3, "kerbie smooth somefile.bin");
	td_init(&td);
	V1 smooth = strtod(argv[1], 0);
	
	INFO("LOAD %f %s", smooth, argv[2]);
	//~ td_read(&td, argv[2]);
	//~ td_bin(&td, &Q, smooth*3600.0/32.0, W_BLACKHARRIS , smooth*3600.0, 0, td.data[0].t, td.data[td.len-1].t);
	//~ V2 mm = f1_minmax(&Q);
	//~ V1 f_log(V1 y)  {return log(y) - log(mm.y) + (log(mm.y) - log(mm.x))/2.0;}
	//~ f1_map(&Q, f_log);
	
	
	V1 wiggle_pos(Function1 *self, int i, V1 t) {
		if (t <= -8.0 || t >= 8.0)
			return 0.0;
		V1 y = -603979776.0;
		V1 x = t;
		y += x * - 754974720.0; x *= t; //x
		y += x * - 46137344.0; x *= t; //x^2
		y += x * 131072000.0; x *= t; //x^3
		y += x * 21135360.0; x *= t; //x^4
		y += x * - 6348800.0; x *= t; //x^5
		y += x * - 1162240.0; x *= t; //x^6
		y += x * 134400.0; x *= t; //x^7
		y += x * 25820.0; x *= t; //x^8
		y += x * - 1325.0; x *= t; //x^9
		y += x * - 261.0; x *= t; //x^10
		y += x * 5.0; x *= t; //x^11
		y += x * 1.0; x *= t; //x^12
		

		
		return y;
	}
	
	V1 wiggle1(Function1 *self, int i, V1 t) {
		if (t <= -8.0 || t >= 8.0)
			return 100.0;
		V1 y = - 754974720.0;
		V1 x = t;
		y += x * - 92274688.0; x *= t; //x
		y += x * 393216000.0; x *= t; //x^2
		y += x * 84541440.0; x *= t; //x^3
		y += x * - 31744000.0; x *= t; //x^4
		y += x * - 6973440.0; x *= t; //x^5
		y += x * 940800.0; x *= t; //x^6
		y += x * 206560.0; x *= t; //x^7
		y += x * - 11925.0; x *= t; //x^8
		y += x * - 2610.0; x *= t; //x^9
		y += x * 55.0; x *= t; //x^10
		y += x * 12.0; x *= t; //x^11
		return y;
			
	}

	V1 f_wiggle(Function1 *self, int i, V1 t) {return wiggle(t);}
	
	f1_init_map(&Q, 1024*10, v2(-11, 11), wiggle_pos);
	f1_init_map(&F, 1024*10, v2(-11, 11), f_wiggle);
	
	INFO("%f ... %f ... %f,  %f ... %f ", Q.x0, Q.dx, Q.x0 + Q.dx * (Q.len-1), Q.ys[0], Q.ys[Q.len-1]);
	
	
	//~ td_fini(&td);

	return 0;
}


