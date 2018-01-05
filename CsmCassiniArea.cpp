#include "CsmCassiniArea.h"

bool CsmCassiniArea::IsInArea(double x, double y)
{
	return true;
}

void CsmCassiniArea::OnResetCsm()
{
	a = m_pCsmAreaCtrl->GetSpinCtrlVal(1);
}

void CsmCassiniArea::SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl)
{
	pCsmAreaCtrl->AddSpinCtrl(_T("a"), 1, 5, 1, 1);
	m_pCsmAreaCtrl = pCsmAreaCtrl;
}

void CsmCassiniArea::SetPoints(int n, nmc::Vector<POS> &commitment_point, nmc::Vector<POS> &wave_source_point)
{
	commitment_point.Init(n); //�S���_
	wave_source_point.Init(n); //��



	int i;
	//�S���_�A�d�ד_�z�u
	POS t1,t2;
	double rho;
	double theta;
	double cos2theta;
	for(i=0; i<n; i++){	
		theta = i*(2.0*_PI_)/n;
		cos2theta = cos(2.0*theta);
		rho = sqrt(cos2theta + 
			sqrt(cos2theta*cos2theta + a*a*a*a - 1));
		t1.x = rho * cos(theta);
		t1.y = rho * sin(theta);
		J(a, t1.x, t1.y, &(t2.x), &(t2.y));
		commitment_point.SetVal(i, t2);
		t1.x *= q;
		t1.y *= q;
		J(a, t1.x, t1.y, &(t2.x), &(t2.y));
		wave_source_point.SetVal(i, t2);
	}

	//create area model
	if(m_pModel){
		ReleaseSmgModel(&m_pModel);
	}

	m_pModel = CreateSmgModel();
	m_pModel->pRootFrame = CreateSmgFrame();
	SmgMesh* pMesh = m_pModel->pRootFrame->pMesh = CreateSmgMesh();

	if(n>=2){
		CreateSmgMeshVertexArr(pMesh, n*2);
		for(i=0; i<n; i++){
			t1 = commitment_point.GetVal(i);
			SetSmgMeshVertex(pMesh, i*2, t1.x, t1.y, 7);
			SetSmgMeshVertex(pMesh, i*2+1, t1.x, t1.y, -7);
		}

		CreateSmgMeshFaceArr(pMesh, n*2+n-2);
		int ver[3];
		for(i=0; i<(n-1); i++){
			ver[0] = i*2; ver[1] = i*2+1; ver[2] = i*2+2;
			SetSmgMeshFace(pMesh, i*2, 3, ver);
			ver[0] = i*2+1; ver[1] = i*2+3; ver[2] = i*2+2;
			SetSmgMeshFace(pMesh, i*2+1, 3, ver);
		}
		ver[0] = i*2; ver[1] = i*2+1; ver[2] = 0;
		SetSmgMeshFace(pMesh, i*2, 3, ver);
		ver[0] = i*2+1; ver[1] = 1; ver[2] = 0;
		SetSmgMeshFace(pMesh, i*2+1, 3, ver);

		int j;
		for(j=0; j<(n-2); j++){
			ver[0] = 0;
			ver[1] = j*2+2;
			ver[2] = j*2+4;
			SetSmgMeshFace(pMesh, i*2+2+j, 3, ver);
		}

		MakeSmgMeshVertexNormal(pMesh);
	}
}

void CsmCassiniArea::DrawArea(int draw_mode)
{
	if(m_pModel){
		RenderSmgModelGL(m_pModel, draw_mode);
	}
}

void CsmCassiniArea::J(double a, double x, double y, double *ret_x, double *ret_y)
{
	double xx,yy;
	double theta;
	xx = x*x - y*y - 1;
	yy = 2*x*y;
	theta = atan2(yy, xx);
	
	double r = sqrt(xx*xx - yy*yy);
	(*ret_x) = r*cos(theta/2.0)/a;
	(*ret_y) = r*sin(theta/2.0)/a;
}