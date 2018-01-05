#include "Csm.h"

void Csm::Calc(int n, double k, nmc::Vector<POS>& s, nmc::Vector<POS>& p)
{
	int i,j;
	A.Init(n, n);
	L.Init(n, n);
	U.Init(n, n);
	P.Init(n);
	Q.Init(n);
	f_s.Init(n);

	//S‘©ğŒ
	for(i=0; i<n; i++){
		POS t = s.GetVal(i);
		f_s.SetVal(i, f(k, t.x, t.y));
	}

	//ŒW”s—ñ
	for(i=0; i<n; i++){
		POS u = s.GetVal(i);
		for(j=0; j<n; j++){
			POS t = p.GetVal(j);
			A.SetVal(i, j, H_0(k, u.x, u.y, t.x, t.y));
		}
	}

	nmc::LUcmd::Decomp(A, L, U, P);
	nmc::LUcmd::Solve(L, U, P, f_s, Q);
}