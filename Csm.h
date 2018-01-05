#ifndef _NMC_CSM_H_
#define _NMC_CSM_H_


#include "stdafx.h"

#include <nmc/LU.h>

#include "ICsmAreaCtrl.h"
#include "bessel.h"

class Csm
{
public:
	Csm(){}
	~Csm(){}

	void Calc(int n, double k, nmc::Vector<POS>& s, nmc::Vector<POS>& p);

	nmc::Complexd u_N(int n, double k, nmc::Vector<POS>& p, double x, double y){
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

	void SetCsmCtrl(ICsmAreaCtrl* csm_area_ctrl){
		m_pCsmAreaCtrl = csm_area_ctrl;
	}

private:
	nmc::Complexd H_0(double k, double x1, double y1, double x2, double y2){
		double r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
		//return gsl_sf_bessel_J0(k*r) + gsl_sf_bessel_Y0(k*r);
		nmc::Complexd ret(dbesj0(k*r), dbesy0(k*r));
		return ret;
	}
	nmc::Complexd f(double k, double x, double y){
		double theta = atan2(y,x); //-PI <= atan2( y,x ) <= PI
		double tmp = k*x;
		nmc::Complexd ret(cos(tmp), sin(tmp));
		return ret;
	}

	ICsmAreaCtrl* m_pCsmAreaCtrl;

	nmc::Matrixcmd A;
	nmc::Matrixcmd L;
	nmc::Matrixcmd U;
	nmc::Vectori P;
	nmc::Vectorcmd Q;
	nmc::Vectorcmd f_s;
};

#endif //_NMC_CSM_H_