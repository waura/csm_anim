#ifndef _CSMELLIPSEAREA_H_
#define _CSMELLIPSEAREA_H_

#include <SkinMeshGL/SmgModel.h>

#include "ICsmArea.h"

class CsmEllipseArea : public ICsmArea
{
public:
	CsmEllipseArea(){
		q = 0.7;
		a = 5;
		b = 2;
		m_pModel = NULL;
	}
	~CsmEllipseArea(){
		if(m_pModel){
			ReleaseSmgModel(&m_pModel);
		}
	}

	bool IsInArea(double x, double y);
	void OnResetCsm();
	double Getq(){ return q; }
	void SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl);
	void SetPoints(int n, nmc::Vector<POS>& commiment_point, nmc::Vector<POS>& wave_source_point);
	void DrawArea(int draw_mode);

private:

	void SpinCtrlCallBack(int id, int val);
	void Jx(double c, double x, double y, double* ret_x, double* ret_y)
	{
		double r = sqrt(x*x + y*y);
		(*ret_x) = (c/2.0)*(r + 1.0/r)*(x/r);
		(*ret_y) = (c/2.0)*(r - 1.0/r)*(y/r);
	}
	void Jy(double c, double x, double y, double* ret_x, double* ret_y)
	{
		double r = sqrt(x*x + y*y);
		(*ret_x) = (c/2.0)*(r - 1.0/r)*(x/r);
		(*ret_y) = (c/2.0)*(r + 1.0/r)*(y/r);
	}

	double q;
	double a;
	double b;
	SmgModel* m_pModel;

	ICsmAreaCtrl* m_pCsmAreaCtrl;

};

#endif //_CSMELLIPSEAREA_H_