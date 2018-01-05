#ifndef _CSMCASSINIAREA_H_
#define _CSMCASSINIAREA_H_

#include <SkinMeshGL/SmgModel.h>

#include "ICsmArea.h"

class CsmCassiniArea : public ICsmArea
{
public:
	CsmCassiniArea(){
		q = 0.2;
		a = 1.0;
		m_pModel = NULL;
	}
	~CsmCassiniArea(){
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
	void J(double a, double x, double y, double* ret_x, double* ret_y);

	double q;
	double a;

	SmgModel* m_pModel;
	ICsmAreaCtrl* m_pCsmAreaCtrl;
};

#endif //_CSMCASSINIAREA_H_