#include "CsmAnimGL.h"

#ifdef WIN32
	#include "Windows.h"

double gettimeofday_msec()
{
	return timeGetTime();
}

#else
        #include <time.h>
        #include <sys/time.h>

double gettimeofday_msec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return static_cast<double>(tv.tv_sec) * 1000.0 + static_cast<double>(tv.tv_usec) / 1000.0;
}
#endif //WIN32

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define _PRINT_FPS
#define _RENDER_GRID

CsmAnimGL::CsmAnimGL()
{
	m_csm_area = NULL;
	m_csm_area_new = NULL;

	m_im_map = NULL;
	m_pModel=NULL;

	m_draw_mode = DRAW_POLY;

	m_min_x = -20;
	m_max_x = 20;
	m_min_y = -20;
	m_max_y = 20;
	m_poly_width = 1.0;

	n = CSM_DEFAULT_n;
	k = CSM_DEFAULT_k;
	c = CSM_DEFAULT_c;
	q = CSM_DEFAULT_q;

	m_theta[0] = 0.0; m_theta[1] = 0.0; m_theta[2] = 0.0;
	m_angle[0] = 0.0; m_angle[1] = 0.0; m_angle[2] = 0.0;
	m_distance = 40.0;
	m_mousel_down = false;
}

CsmAnimGL::~CsmAnimGL()
{
	if(m_csm_area){
		delete m_csm_area;
		m_csm_area = NULL;
	}
	if(m_csm_area_new){
		delete m_csm_area_new;
		m_csm_area_new = NULL;
	}

	ReleaseSmgModel(&m_pModel);
}

void CsmAnimGL::ResetCsm()
{
	if(m_csm_area_new){
		m_csm_area = m_csm_area_new;
		m_csm_area_new = NULL;
	}

	if(m_csm_area){
		m_csm_area->OnResetCsm();
		m_csm_area->SetPoints(n, s, p);
		q = m_csm_area->Getq();
		m_csm.Calc(n, k, s, p);
	}
	else{
#ifdef _WIN32
		::OutputDebugString("error: CsmAnimGL::Init() m_csm_are is NULL\n");
#endif
	}

	//Init model
	if(m_pModel){
		ReleaseSmgModel(&m_pModel);
	}
	m_pModel = CreateSmgModel();
	m_pModel->pRootFrame = CreateSmgFrame();
	SmgMesh* pMesh = m_pModel->pRootFrame->pMesh = CreateSmgMesh();

	int i,j;
	int index;
	int xrange = m_max_x - m_min_x;
	int yrange = m_max_y - m_min_y;
	nmc::Complexd ex;
	nmc::Complexd un;
	int vertex_x = xrange/m_poly_width + 1;
	int vertex_y = yrange/m_poly_width + 1;
	int vertex_size = vertex_x * vertex_y;
	CreateSmgMeshVertexArr(pMesh, vertex_size);

	if(m_im_map){
		free(m_im_map);
	}
	m_im_map = (double*)malloc(sizeof(double) * vertex_size);

	double x,y;
	int x_idx,y_idx;
	for(x_idx=0; x_idx<vertex_x; x_idx++){
		x = m_min_x + m_poly_width * x_idx;
		ex.SetNum(cos(x*k), sin(x*k));
		for(y_idx=0; y_idx<vertex_y; y_idx++){
			y = m_min_y + m_poly_width * y_idx;
			index = x_idx * vertex_y + y_idx;
			if(m_csm_area->IsInArea(x, y)){
				un = m_csm.u_N(n, k, p, x, y) - ex;
			}
			else{
				un.SetNum(0.0, 0.0);
			}
			SetSmgMeshVertex(pMesh, index, x, y, un.Re());
			m_im_map[index] = un.Im();
		}
	}
	CreateSmgMeshFaceArr(pMesh, (yrange/m_poly_width) * (xrange/m_poly_width) * 2);
	int ver[3];
	int pt;
	for(x_idx=0; x_idx<vertex_x-1; x_idx++){
		for(y_idx=0; y_idx<vertex_y-1; y_idx++){
			pt = vertex_y * x_idx + y_idx;
			ver[0] = pt; ver[1] = pt + vertex_y; ver[2] = pt+1;
			index = ((vertex_y-1) * x_idx + y_idx)*2;
			SetSmgMeshFace(pMesh, index, 3, ver);
			ver[0] = pt+1; ver[1] = pt + vertex_y; ver[2] = pt + vertex_y + 1;
			SetSmgMeshFace(pMesh, index+1, 3, ver);
		}
	}

	//DumpSmgModel(m_pModel);
	MakeSmgMeshVertexNormal(pMesh);
}

void CsmAnimGL::Init(int csm_area_id)
{
	// opengl ‰ŠúÝ’è
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	//glShadeModel (GL_FLAT);

	glCullFace(GL_BACK); //”w–Ê‚Ìƒ|ƒŠƒSƒ“‚ðÁ‹Ž

	m_start_time = m_current_time = gettimeofday_msec();
}

void CsmAnimGL::SetViewport(int w, int h)
{
	glViewport(0, 0, w, h);

	//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLdouble)w / (GLdouble)h, 1.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
}

void CsmAnimGL::Idle()
{
	double t = gettimeofday_msec();
	double delta_t = t - m_current_time;
	m_current_time = t;
	MoveModel(delta_t);
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
			//printf("fps = %d \n", i);
	#endif
			pre_t = m_current_time;
			i=0;
		}
		i++;
	#endif
}

void CsmAnimGL::Draw()
{
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
	for(x = m_min_x*2; x <= m_max_x*2; x+=1){
		if(x==0) continue;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		SetSmgVertexVal(&start, x, 0, m_min_y*2);
		SetSmgVertexVal(&end, x, 0, m_max_y*2);
		RenderLine(1, &start, &end);
	}
	for(y = m_min_y*2; y <= m_max_y*2; y+=1){
		if(y==0) continue;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		SetSmgVertexVal(&start, m_min_x*2, 0, y);
		SetSmgVertexVal(&end, m_max_x*2, 0, y);
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
	glRotated(-90, 1.0, 0.0, 0.0);
	color[0] = 0.4; color[1] = 0.4; color[2] = 0.4; color[3] = 1.0;
	glMaterialfv( GL_FRONT,
		GL_AMBIENT_AND_DIFFUSE,
		color);
	RenderSmgModelGL(m_pModel, m_draw_mode);
	
	//render csm area
	if(m_csm_area){
		color[0] = 0.3; color[1] = 0.3; color[2] = 0.3; color[3] = 1.0;
		glMaterialfv( GL_FRONT,
			GL_AMBIENT_AND_DIFFUSE,
			color);
		m_csm_area->DrawArea(m_draw_mode);
	}
	glPopMatrix();
	/*glutSwapBuffers();*/
}

void CsmAnimGL::MouseButton(int button, int state, int x, int y)
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
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		default:
			break;
	}
}

void CsmAnimGL::MouseMove(int x, int y)
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

void CsmAnimGL::MouseWheel(int rot, int delta)
{
	if(rot >= 0){
		m_distance -= 3*(delta/100);
		if(m_distance < 0){
			m_distance = 0;
		}
	}
	else{
		m_distance += 3*(delta/100);
	}
}

void CsmAnimGL::SetCsmAreaCtrl(ICsmAreaCtrl* csm_area_ctrl)
{
	m_pCsmAreaCtrl = csm_area_ctrl;
	if(m_csm_area){
		m_csm_area->SetCsmAreaCtrl(csm_area_ctrl);
	}
}

void CsmAnimGL::SetCsmArea(int id)
{
	switch(id){
		case CSM_AREA_CIRCLE:
			if(m_csm_area_new){
				delete m_csm_area_new;
			}
			m_csm_area_new = new CsmCircleArea;
			break;
		case CSM_AREA_ELLIPSE:
			if(m_csm_area_new){
				delete m_csm_area_new;
			}
			m_csm_area_new = new CsmEllipseArea;
			break;
		case CSM_AREA_CASSINIX:
			if(m_csm_area_new){
				delete m_csm_area_new;
			}
			m_csm_area_new = new CsmCassinixArea;
			break;
		case CSM_AREA_CASSINIY:
			if(m_csm_area_new){
				delete m_csm_area_new;
			}
			m_csm_area_new = new CsmCassiniyArea;
			break;
		default:
#ifdef _WIN32
			::OutputDebugString("error: CsmAnimGL::SetCsmArea() unknow csm area id");
#endif
			break;
	}

	//reset csm area ctrl
	m_pCsmAreaCtrl->ClearAllCtrl();
	m_csm_area_new->SetCsmAreaCtrl(m_pCsmAreaCtrl);
}

void CsmAnimGL::MoveModel(double delta_t)
{
	if(m_pModel == NULL) return;

	//double t = (m_current_time - m_start_time)/1000.0;
	//nmc::Complexd cm(cos(c*k*t), (-1.0)*sin(c*k*t));
	delta_t /= 1000.0;
	nmc::Complexd cm(cos(c*k*delta_t), (-1.0)*sin(c*k*delta_t));

	int i,j;
	double d;
	int index;
	nmc::Complexd un;
	nmc::Complexd ver;
	//nmc::Complexd ex;
	SmgMesh* pMesh = m_pModel->pRootFrame->pMesh;

	int xrange = m_max_x - m_min_x;
	int yrange = m_max_y - m_min_y;
	int vertex_x = xrange/m_poly_width + 1;
	int vertex_y = yrange/m_poly_width + 1;
	int vertex_size = vertex_x * vertex_y;

	double x,y;
	int x_idx,y_idx;
	for(x_idx=0; x_idx<vertex_x; x_idx++){
		x = m_min_x + m_poly_width * x_idx;
		for(y_idx=0; y_idx<vertex_y; y_idx++){
			y = m_min_y + m_poly_width * y_idx;
			if(m_csm_area->IsInArea(x, y)){
				index = x_idx * vertex_y + y_idx;
				d = pMesh->arrVertex[index]._d[2];				
				un.SetNum(d, m_im_map[index]);
				//un = m_csm.u_N(n, k, p, i, j) - ex;
				ver = cm * un;
				SetSmgMeshVertex(pMesh, index, x, y, ver.Re());
				m_im_map[index] = ver.Im();
			}
			else{
				SetSmgMeshVertex(pMesh, index, x, y, 0.0);
			}
		}
	}

	MakeSmgMeshVertexNormal(pMesh);
}