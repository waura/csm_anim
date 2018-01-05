#include "CsmCircleArea.h"

bool CsmCircleArea::IsInArea(double x, double y)
{
	if(x*x + y*y >= rho*rho){
		return true;
	}
	return false;
}

void CsmCircleArea::OnResetCsm()
{
	rho = m_pCsmAreaCtrl->GetSpinCtrlVal(1);
}

void CsmCircleArea::SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl)
{
	pCsmAreaCtrl->AddSpinCtrl(_T("radius"), 1, 100, 2, 1);
	m_pCsmAreaCtrl = pCsmAreaCtrl;
}

void CsmCircleArea::SetPoints(int n, nmc::Vector<POS> &commitment_point, nmc::Vector<POS> &wave_source_point)
{
	commitment_point.Init(n);
	wave_source_point.Init(n);

	int i;
	//�S���_�A�d�ד_�z�u
	POS t1;
	for(i=0; i<n; i++){	
		t1.x = rho * cos(i*(2.0*_PI_)/n);
		t1.y = rho * sin(i*(2.0*_PI_)/n);
		commitment_point.SetVal(i, t1);
		t1.x *= q;
		t1.y *= q;
		wave_source_point.SetVal(i, t1);
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

void CsmCircleArea::DrawArea(int draw_mode)
{
	if(m_pModel){
		RenderSmgModelGL(m_pModel, draw_mode);
	}
}

