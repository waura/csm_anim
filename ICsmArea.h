#ifndef _ICSM_AREA_H_
#define _ICSM_AREA_H_

#include "stdafx.h"
#include "ICsmAreaCtrl.h"

class ICsmArea
{
public:
	ICsmArea(){};
	virtual ~ICsmArea(){};

	virtual bool IsInArea(double x, double y)=0;
	virtual void OnResetCsm()=0;
	virtual void SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl)=0;
	virtual double Getq()=0;
	virtual void SetPoints(int n, nmc::Vector<POS>& commitment_point, nmc::Vector<POS>& wave_source_point)=0;
	virtual void DrawArea(int draw_mode)=0;
};

#endif //_ICSM_AREA_H_