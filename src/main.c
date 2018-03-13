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

void advance_state_0(State *s, Function1 *a, V1 t0, V1 dt)
{
	V1 a0 = f1_eval_at(a, t0);
	s->x = s->x0 + s->v0*dt;
	s->v = s->v0 + a0*dt;
}

void advance_state_1(State *s, Function1 *a, V1 t0, V1 dt)
{
	V1 a0 = f1_eval_at(a, t0);
	V1 a1 = f1_eval_at(a, t0 + dt);
	a0 = a0+ (a1-a0)/2.0;
	s->x = s->x0 + s->v0*dt + 0.5*a0*dt*dt;
	s->v = s->v0 +  a0*dt;
}


Function1 Q, F, A, B;
FunctionRanged Qr, Fr, Ar, Br;

void gl_init(void)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0 , 0.0, 1.0);
	
	glmath_init();
	
	view_fit(&view, &Q, 1);
	
	
	
	f1_derivative(&F, &Q);
	V1 v0 = F.ys[0];
	v0 = 0.0;
	f1_derivative(&F, &F);

	
	f1_init(&A, 64);
	f1_init(&B, 16);
	A.dx = 20.0/A.memlen;
	B.dx = 20.0/B.memlen;
	A.x0 = -10.0;
	B.x0 = -10.0;
	
	State s0, s1;
	s0.x = Q.ys[0];
	s0.v = v0;
	s1.x = Q.ys[0];
	s1.v = v0;
	
	for (int i=0; i < A.memlen; ++i) {
		V1 t0 = A.x0 + i*A.dx;
		f1_append(&A, s0.x);
		s0.v0 = s0.v;
		s0.x0 = s0.x;
		advance_state_0(&s0, &F, t0, A.dx);
		if(i%4 == 0) {
			f1_append(&B, s1.x);
			s1.v0 = s1.v;
			s1.x0 = s1.x;
			advance_state_1(&s1, &F, t0, B.dx);
		}
	}
	
	fr_init(&Qr, 1024);
	fr_init(&Fr, 1024);
	fr_init(&Ar, 1024);
	fr_init(&Br, 1024);
	
	fr_compile(&Qr, &Q, view.vps.x);
	fr_compile(&Fr, &F, view.vps.x);
	fr_compile(&Ar, &A, view.vps.x);
	fr_compile(&Br, &B, view.vps.x);
	
}


int gl_frame(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	view_navigate(&view, GW.m.btn, 1.25);
	if (view.vps.x != prev_vps) {
		//~ if (view.vps.x < B.dx)
			//~ view_zoom_at(&view, screen_to_view(&view, v2(GW.m.hx, GW.m.hy)), v2(B.dx, view.vps.y));
		fr_compile(&Qr, &Q, view.vps.x);
		fr_compile(&Fr, &F, view.vps.x);
		fr_compile(&Ar, &A, view.vps.x);
		fr_compile(&Br, &B, view.vps.x);
		prev_vps = view.vps.x;
	}

	fr_render(&Qr, &view, rgb(1.0, 1.0, 0.0), 1);
	//~ fr_render(&Fr, &view, rgb(0.0, 1.0, 0.0), 1);
	fr_render(&Ar, &view, rgb(0.0, 1.0, 1.0), 1);
	fr_render(&Br, &view, rgb(0.0, 0.0, 1.0), 1);
	
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
	
	
	f1_init(&Q, 1024*10);
	Q.x0 = -10.0;
	Q.dx = 20.0/(Q.memlen-1);
	for (int i=0; i < Q.memlen; ++i) {
		V1 t = Q.x0 + Q.dx * i;
		f1_append(&Q, sin(t+1)*(1.0+cos(M_PI*t/10.0)));
	}

	
	INFO("%f ... %f ... %f,  %f ... %f ", Q.x0, Q.dx, Q.x0 + Q.dx * (Q.len-1), Q.ys[0], Q.ys[Q.len-1]);
	
	
	//~ td_fini(&td);

	return 0;
}


