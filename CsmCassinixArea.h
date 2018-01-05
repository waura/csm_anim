#ifndef _CSMCASSINIXAREA_H_
#define _CSMCASSINIXAREA_H_

#include <SkinMeshGL/SmgModel.h>

#include "ICsmArea.h"

class CsmCassinixArea : public ICsmArea
{
public:
	CsmCassinixArea();
	~CsmCassinixArea();

	bool IsInArea(double x, double y);
	void OnResetCsm();
	double Getq(){ return q; }
	void SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl);
	void SetPoints(int n, nmc::Vector<POS>& commitment_point, nmc::Vector<POS>& wave_source_point);
	void DrawArea(int draw_mode);

private:

	double q;
	double a;
	int m_size;

	SmgModel* m_pModel;
	ICsmAreaCtrl* m_pCsmAreaCtrl;
};

#endif //_CSMCASSINIXAREA_H_