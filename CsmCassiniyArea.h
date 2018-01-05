#ifndef _CSMCASSINIYAREA_H_
#define _CSMCASSINIYAREA_H_

#include <SkinMeshGL/SmgModel.h>

#include "ICsmArea.h"

class CsmCassiniyArea : public ICsmArea
{
public:
	CsmCassiniyArea();
	~CsmCassiniyArea();

	bool IsInArea(double x, double y);
	void OnResetCsm();
	double Getq(){ return q; }
	void SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl);
	void SetPoints(int n, nmc::Vector<POS>& commitment_point, nmc::Vector<POS>& wave_source_point);
	void DrawArea(int draw_mode);

private:

	double q;
	double a;

	SmgModel* m_pModel;
	ICsmAreaCtrl* m_pCsmAreaCtrl;
};

#endif //_CSMCASSINIYAREA_H_