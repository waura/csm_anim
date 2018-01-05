#ifndef _CSM_ANIM_GL_H
#define _CSM_ANIM_GL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <SkinMeshGL/SmgModel.h>
#include <SkinMeshGL/SmgUtil.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include "Csm.h"
#include "CsmCircleArea.h"
#include "CsmEllipseArea.h"
#include "CsmCassinixArea.h"
#include "CsmCassiniyArea.h"

#define CSM_DEFAULT_n 40
#define CSM_DEFAULT_k 1.0
#define CSM_DEFAULT_c 7.0
#define CSM_DEFAULT_q 0.4

#define CSM_DEFAULT_POLY_SIZE 1.0
#define CSM_DEFAULT_POLY_SIZE_STR "1.0"

enum{
	CSM_AREA_CIRCLE = 0,
	CSM_AREA_ELLIPSE = 1,
	CSM_AREA_CASSINIX = 2,
	CSM_AREA_CASSINIY = 3,
};

class CsmAnimGL
{
public:
	CsmAnimGL();
	~CsmAnimGL();

	void ResetCsm();
	void Init(int csm_area_id=CSM_AREA_CIRCLE);
	
	void SetViewport(int w, int h);
	void Idle();
	void Draw();
	void MouseButton(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void MouseWheel(int rot, int delta);

	void SetCsmAreaCtrl(ICsmAreaCtrl* csm_area_ctrl);
	void SetCsmArea(int id);

	void SetDrawMode(int draw_mode){
		m_draw_mode = draw_mode;
	}
	void SetPolySize(double size){
		m_poly_width = size;
	}
	void SetXSize(int xsize){
		m_max_x = xsize/2;
		m_min_x = -m_max_x;
	}
	void SetYSize(int ysize){
		m_max_y = ysize/2;
		m_min_y = -m_max_y;
	}
	void SetN(int n){
		this->n = n;
	}
	void Setk(double k){
		this->k = k;
	}
	void Setc(double c){
		this->c = c;
	}
	void Setq(double q){
		this->q = q;
	}

private:
	
	void MoveModel(double delta_t);

	Csm m_csm;
	ICsmArea* m_csm_area;
	ICsmArea* m_csm_area_new;
	ICsmAreaCtrl* m_pCsmAreaCtrl;

	nmc::Vector<POS> s; //�S���_
	nmc::Vector<POS> p; //�g���_

	double * m_im_map;

	SmgModel* m_pModel;

	int m_draw_mode;
	double m_start_time;
	double m_current_time;

	int m_min_x;
	int m_max_x;
	int m_min_y;
	int m_max_y;
	double m_poly_width;

	int n;
	double k;
	double c;
	double q;

	int m_mpos[2];
	double m_theta[3];
	double m_angle[3];
	double m_distance;
	bool m_mousel_down;
};

#endif // _CSMANIMFRAME_H