#ifndef _CSMCIRCLEAREA_H_
#define _CSMCIRCLEAREA_H_

#include <SkinMeshGL/SmgModel.h>

#include "ICsmArea.h"

class CsmCircleArea : public ICsmArea
{
public:
	CsmCircleArea(){
		q=0.2;
		rho = 2.0;
		m_pModel = NULL;
	}
	~CsmCircleArea(){
		if(m_pModel){
			ReleaseSmgModel(&m_pModel);
		}
	}

	bool IsInArea(double x, double y);
	void OnResetCsm();
	double Getq(){ return q; }
	void SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl);
	void SetPoints(int n, nmc::Vector<POS>& commitment_point, nmc::Vector<POS>& wave_source_point);
	void DrawArea(int draw_mode);

private:

	double q;
	double rho;

	SmgModel* m_pModel;

	ICsmAreaCtrl* m_pCsmAreaCtrl;
};

#endif //_CSMCIRCLEAREA_H_