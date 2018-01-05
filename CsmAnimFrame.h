#ifndef _CSM_ANIM_FRAME_H
#define _CSM_ANIM_FRAME_H

// CFLAGS=`wx-config --cflags`
// LDFLAGS=`wx-config --libs gl`

#include <wx/wx.h>
#include <wx/glcanvas.h>
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif

#ifdef WIN32
	#include "Windows.h"
#else
	#include <time.h>
	#include <sys/time.h>

double gettimeofday_msec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-3;
}
#endif //WIN32

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <SmgModel.h>
#include <SmgUtil.h>

#include <nmc/LU.h>

#include "bessel.h"

#define _PRINT_FPS
#define _RENDER_GRID

#define _PI_ (3.14159265)

typedef struct _POS{
	double x;
	double y;
}POS;


class OpenGL
{
public:
	OpenGL()
		: m_pModel(NULL)
		, m_start_time(0.0)
		, m_current_time(0.0)
		, m_min_x(-20)
		, m_max_x(20)
		, m_min_y(-20)
		, m_max_y(20)
		, m_poly_width(1)
		, n(40)
		, k(1.0)
		, rho(2.0)
		, q(0.4)
		, c(7)
		, A()
		, L()
		, U()
		, P()
		, Q()
		, f_s()
		, s()
		, p()
		, m_mpos()
		, m_theta()
		, m_angle()
		, m_distance(40.0)
		, m_mousel_down(false)
	{
		m_theta[0] = 0.0; m_theta[1] = 0.0; m_theta[2] = 0.0;
		m_angle[0] = 0.0; m_angle[1] = 0.0; m_angle[2] = 0.0;
	}

	~OpenGL(){
		ReleaseSmgModel(&m_pModel);
	}

	void initialize(){
		int i,j;
		A.Init(n, n);
		L.Init(n, n);
		U.Init(n, n);
		P.Init(n);
		Q.Init(n);
		f_s.Init(n);
		s.Init(n); //�S���_
		p.Init(n); //��	
		
		// opengl �����ݒ�
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//glEnable(GL_NORMALIZE);
		//glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POLYGON_SMOOTH);
		//glShadeModel (GL_FLAT);

		glCullFace(GL_BACK); //�w�ʂ̃|���S�������

		//�d�ד_�z�u
		for(i=0; i<n; i++){
			POS t1;
			t1.x = rho * cos(i*(2.0*_PI_)/n);
			t1.y = rho * sin(i*(2.0*_PI_)/n);
			s.SetVal(i, t1);
			t1.x *= q;
			t1.y *= q;
			p.SetVal(i, t1);
		}

		//�S����
		for(i=0; i<n; i++){
			POS t = s.GetVal(i);
			f_s.SetVal(i, f(k, rho, t.x, t.y));
		}

		//�W���s��
		for(i=0; i<n; i++){
			POS u = s.GetVal(i);
			for(j=0; j<n; j++){
				POS t = p.GetVal(j);
				A.SetVal(i, j, H_0(k, u.x, u.y, t.x, t.y));
			}
		}

		printf("N=%d k=%0.2f rho=%0.2f q=%0.2f c=%0.2f \n", n, k, rho, q, c); 

		nmc::LUcmd::Decomp(A, L, U, P);
		nmc::LUcmd::Solve(L, U, P, f_s, Q);

		//Init model
		m_pModel = CreateSmgModel();
		m_pModel->pRootFrame = CreateSmgFrame();
		SmgMesh* pMesh = m_pModel->pRootFrame->pMesh = CreateSmgMesh();

		int index;
		int xrange = m_max_x - m_min_x;
		int yrange = m_max_y - m_min_y;
		nmc::Complexd un;
		CreateSmgMeshVertexArr(pMesh, (xrange/m_poly_width + 1) * (yrange/m_poly_width + 1));
		double x,y;
		for(x=m_min_x; x<=m_max_x; x+=m_poly_width){
			for(y=m_min_y; y<=m_max_y; y+=m_poly_width){
				index = ((x-m_min_x)/m_poly_width)*((m_max_x-m_min_x)/m_poly_width+1) + ((y-m_min_y)/m_poly_width);
				if(isInArea(rho, x, y)){
					un = u_N(k, Q, p, x, y);
					SetSmgMeshVertex(pMesh, index, x, y, un.Re());
				}
				else{
					SetSmgMeshVertex(pMesh, index, x, y, 0);
				}
			}
		}
		CreateSmgMeshFaceArr(pMesh, (yrange/m_poly_width) * (xrange/m_poly_width) * 2);
		int ver[3];
		int pt;
		int x_poly_num = xrange/m_poly_width;
		int y_poly_num = yrange/m_poly_width;
		for(j=0; j<y_poly_num; j++){
			for(i=0; i<x_poly_num; i++){
				pt = (i/m_poly_width) + (x_poly_num+1) * (j/m_poly_width);
				ver[0] = pt; ver[1] = pt+1; ver[2] = pt + x_poly_num +1;
				index = ((i/m_poly_width)*x_poly_num+(j/m_poly_width))*2;
				SetSmgMeshFace(pMesh, index, 3, ver);
				ver[0] = pt+1; ver[1] = pt + x_poly_num + 2; ver[2] = pt + x_poly_num + 1;
				SetSmgMeshFace(pMesh, index+1, 3, ver);
			}
		}

		//DumpSmgModel(m_pModel);
		MakeSmgMeshVertexNormal(pMesh);
	}
 
	void setViewport(int w, int h){
		glViewport(0, 0, w, h);

		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, (GLdouble)w / (GLdouble)h, 1.0, 200.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void idle(void)
	{
		//
	#ifdef WIN32
		m_current_time = timeGetTime();
	#else
		m_current_time = gettimeofday_msec();
	#endif
		move_model();
		//glutPostRedisplay();

	#ifdef _PRINT_FPS
		static int i;
		static double pre_t;
		if((m_current_time - pre_t) > 1000.0){
	#ifdef WIN32
			char buf[256];
			sprintf(buf, "fps = %d \n", i);
			::OutputDebugString(buf);
	#else
			printf("fps = %d \n", i);
	#endif
			pre_t = m_current_time;
			i=0;
		}
		i++;
	#endif
	}
 
    void draw(){
		static GLfloat lightpos[] = { 3.0, 4.0, 5.0, 1.0 };

		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//set light
		GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

		gluLookAt(
			m_distance, m_distance, m_distance,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

		glRotated(m_angle[0], 1.0, 0.0, 0.0);
		glRotated(m_angle[1], 0.0, 1.0, 0.0);
		glRotated(m_angle[2], 0.0, 0.0, 1.0);

		GLfloat color[4];
		SmgVertex start, end;

		//render grid
	#ifdef _RENDER_GRID
		int x,y;
		color[0] = 0.3; color[1] = 0.3; color[2] = 0.3; color[3] = 1.0;
		for(x = m_min_x*2; x <= m_max_x*2; x+=m_poly_width){
			if(x==0) continue;
			glMaterialfv( GL_FRONT,
				GL_AMBIENT_AND_DIFFUSE,
				color);
			SetSmgVertexVal(&start, x, 0, m_min_y*2);
			SetSmgVertexVal(&end, x, 0, m_max_y*2);
			RenderLine(1, &start, &end);
		}
		for(y = m_min_y*2; y <= m_max_y*2; y+=m_poly_width){
			if(y==0) continue;
			glMaterialfv( GL_FRONT,
				GL_AMBIENT_AND_DIFFUSE,
				color);
			SetSmgVertexVal(&start, m_min_x*2, 0, y);
			SetSmgVertexVal(&end, m_max_y*2, 0, y);
			RenderLine(1, &start, &end);
		}
	#endif

		//render axis
		color[0] = 1.0; color[1] = 0.0; color[2] = 0.0; color[3] = 1.0;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		SetSmgVertexVal(&start, m_min_x*2, 0, 0);
		SetSmgVertexVal(&end, m_max_x*2, 0, 0);
		RenderLine(1, &start, &end);

		color[0] = 0.0; color[1] = 1.0; color[2] = 0.0; color[3] = 1.0;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		SetSmgVertexVal(&start, 0, min(m_min_x, m_min_y)*2, 0);
		SetSmgVertexVal(&end, 0, max(m_max_x, m_max_y)*2, 0);
		RenderLine(1, &start, &end);

		color[0] = 0.0; color[1] = 0.0; color[2] = 1.0; color[3] = 1.0;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		SetSmgVertexVal(&start, 0, 0, m_min_y*2);
		SetSmgVertexVal(&end, 0, 0, m_max_y*2);
		RenderLine(1, &start, &end);

		//render model
		glPushMatrix();
		glRotated(90, 1.0, 0.0, 0.0);
		color[0] = 0.4; color[1] = 0.4; color[2] = 0.4; color[3] = 1.0;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		RenderSmgModelGL(m_pModel, DRAW_POLY);
		glPopMatrix();

		/*glutSwapBuffers();*/
	}

	void mouse(int button, int state, int x, int y)
	{
		switch (button) {
			case GLUT_LEFT_BUTTON:
				if(state == GLUT_DOWN){
					m_mpos[0] = x;
					m_mpos[1] = y;
					m_mousel_down = true;
				}
				if(state == GLUT_UP){
					m_mousel_down = false;
				}
				
				//
	#ifdef WIN32
				m_start_time = m_current_time;
	#else
				m_start_time = gettimeofday_msec();
	#endif //WIN32
				break;
			case GLUT_MIDDLE_BUTTON:
				break;
			case GLUT_RIGHT_BUTTON:
				break;
			default:
				break;
		}
	}

	void motion(int x, int y)
	{
		if(m_mousel_down){
			m_theta[0] = (double)(y-m_mpos[1])/5.0;
			m_theta[1] = (double)(x-m_mpos[0])/5.0;

			m_mpos[0] = x;
			m_mpos[1] = y;
			m_angle[0] += m_theta[0];
			m_angle[1] += m_theta[1];
		}
	}

	void wheel(int delta)
	{
		if(delta >= 0){
			m_distance--;
			if(m_distance < 0){
				m_distance = 0;
			}
		}
		else{
			m_distance++;
		}
	}

private:
	nmc::Complexd H_0(double k, double x1, double y1, double x2, double y2)
	{
		double r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
		//return gsl_sf_bessel_J0(k*r) + gsl_sf_bessel_Y0(k*r);
		nmc::Complexd ret(dbesj0(k*r), dbesy0(k*r));
		return ret;
	}

	nmc::Complexd f(double k, double rho, double x, double y)
	{
		double theta = atan2(y,x); //-PI <= atan2( y,x ) <= PI
		double tmp = k*rho*cos(theta);
		nmc::Complexd ret(cos(tmp), sin(tmp));
		return ret;
	}

	nmc::Complexd u_N(double k, nmc::Vectorcmd& Q, nmc::Vector<POS>& p, double x, double y)
	{
		int i;
		nmc::Complexd tmp;
		nmc::Complexd ret=0;
		for(i=0; i<Q.dim(); i++){
			POS t = p.GetVal(i);
			tmp = Q.GetVal(i) * H_0(k, x, y, t.x, t.y);
			ret.Add(tmp);
		}
		return ret;
	}

	bool isInArea(double rho, double x, double y){
		if(((x*x) + (y*y)) > rho){
			return true;
		}
		return false;
	}

	void move_model()
	{
		double t = (m_current_time - m_start_time)/1000.0;
		nmc::Complexd cm(cos(c*k*t), (-1.0)*sin(c*k*t));

		int i,j;
		int index;
		nmc::Complexd un;
		nmc::Complexd ver;
		nmc::Complexd ex;
		SmgMesh* pMesh = m_pModel->pRootFrame->pMesh;
		for(i=m_min_x; i<=m_max_x; i+=m_poly_width){
			ex.SetNum(cos(k*i), sin(k*i));
			for(j=m_min_y; j<=m_max_y; j+=m_poly_width){
				index = ((i-m_min_x)/m_poly_width)*((m_max_x-m_min_x)/m_poly_width+1) + ((j-m_min_y)/m_poly_width);
				if(isInArea(rho, i, j)){
					un = u_N(k, Q, p, i, j) - ex;
					ver = cm * un;
					SetSmgMeshVertex(pMesh, index, i, j, ver.Re());
				}
				else{
					SetSmgMeshVertex(pMesh, index, i, j, 100);
				}
			}
		}

		MakeSmgMeshVertexNormal(pMesh);
	}

	SmgModel* m_pModel;

	double m_start_time;
	double m_current_time;

	int m_min_x;
	int m_max_x;
	int m_min_y;
	int m_max_y;
	int m_poly_width;
	int n;
	double k;
	double rho;
	double q;
	double c;
	nmc::Matrixcmd A;
	nmc::Matrixcmd L;
	nmc::Matrixcmd U;
	nmc::Vectori P;
	nmc::Vectorcmd Q;
	nmc::Vectorcmd f_s;
	nmc::Vector<POS> s; //�S���_
	nmc::Vector<POS> p; //��	

	int m_mpos[2];
	double m_theta[3];
	double m_angle[3];
	double m_distance;
	bool m_mousel_down;
};

#endif // _CSMANIMFRAME_H