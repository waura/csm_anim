#include "CsmEllipseArea.h"


bool CsmEllipseArea::IsInArea(double x, double y)
{
	if((x*x)/(a*a) + (y*y)/(b*b) >= 1.0){
		return true;
	}
	return false;
}

void CsmEllipseArea::OnResetCsm()
{
	a = (double)m_pCsmAreaCtrl->GetSpinCtrlVal(1);
	b = (double)m_pCsmAreaCtrl->GetSpinCtrlVal(2);

	if(a==b){
		goto error;
	}
	else{
		double rho = sqrt(fabs((a + b)/(a - b)));
		if(rho < 1.2){
			goto error;
		}
	}

	return;

error:

	wxLogMessage("use default value");
	m_pCsmAreaCtrl->SetSpinCtrlVal(1, 5);
	m_pCsmAreaCtrl->SetSpinCtrlVal(2, 2);
	a = 5.0;
	b = 2.0;
}

void CsmEllipseArea::SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl)
{
	pCsmAreaCtrl->AddSpinCtrl(_T("X"), 1, 100, 5, 1);
	pCsmAreaCtrl->AddSpinCtrl(_T("Y"), 1, 100, 2, 2);
	m_pCsmAreaCtrl = pCsmAreaCtrl;
}

void CsmEllipseArea::SetPoints(int n, nmc::Vector<POS> &commitment_point, nmc::Vector<POS> &wave_source_point)
{
	commitment_point.Init(n);
	wave_source_point.Init(n);

	double c = sqrt(fabs(a*a - b*b));
	double rho = sqrt(fabs((a + b)/(a - b)));
	if(q*rho <= 1.0){
		q = 1.0/rho + 0.1;
	}

	int i;
	//�S���_�A�d�ד_�z�u
	POS t1,t2;
	if(a > b){
		for(i=0; i<n; i++){	
			t1.x = rho * cos(i*(2.0*_PI_)/n);
			t1.y = rho * sin(i*(2.0*_PI_)/n);
			Jx(c, t1.x, t1.y, &(t2.x), &(t2.y));
			commitment_point.SetVal(i, t2);
			t1.x *= q;
			t1.y *= q;
			Jx(c, t1.x, t1.y, &(t2.x), &(t2.y));
			wave_source_point.SetVal(i, t2);
		}
	}
	else{
		for(i=0; i<n; i++){	
			t1.x = rho * cos(i*(2.0*_PI_)/n);
			t1.y = rho * sin(i*(2.0*_PI_)/n);
			Jy(c, t1.x, t1.y, &(t2.x), &(t2.y));
			commitment_point.SetVal(i, t2);
			t1.x *= q;
			t1.y *= q;
			Jy(c, t1.x, t1.y, &(t2.x), &(t2.y));
			wave_source_point.SetVal(i, t2);
		}
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

void CsmEllipseArea::DrawArea(int draw_mode)
{
	if(m_pModel){
		RenderSmgModelGL(m_pModel, draw_mode);
	}
}