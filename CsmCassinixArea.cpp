#include "CsmCassinixArea.h"


CsmCassinixArea::CsmCassinixArea()
{
	q = 3.0;
	a = 1.1;
	m_pModel = NULL;
}

CsmCassinixArea::~CsmCassinixArea(){
	if(m_pModel){
		ReleaseSmgModel(&m_pModel);
	}
}

bool CsmCassinixArea::IsInArea(double x, double y)
{
	if(((x+1)*(x+1)+y*y)*((x-1)*(x-1)+y*y) >= a*a*a*a){
		return true;
	}
	return false;
}

void CsmCassinixArea::OnResetCsm()
{
	//m_size = m_pCsmAreaCtrl->GetSpinCtrlVal(1);
	a = atof(m_pCsmAreaCtrl->GetTxtBoxVal(1).c_str());
}

void CsmCassinixArea::SetCsmAreaCtrl(ICsmAreaCtrl* pCsmAreaCtrl)
{
	//pCsmAreaCtrl->AddSpinCtrl(_T("サイズ"), 1, 100, 1, 1);
	pCsmAreaCtrl->AddTxtBox(_T("a"), "1.1", 1);
	m_pCsmAreaCtrl = pCsmAreaCtrl;
}

void CsmCassinixArea::SetPoints(int n, nmc::Vector<POS> &commitment_point, nmc::Vector<POS> &wave_source_point)
{
	commitment_point.Init(n); //拘束点
	wave_source_point.Init(n); //極


	int i;

	POS t1, t2, t3;

	//拘束点配置
	double r;
	double theta;
	double cos2theta;
	for(i=0; i<n; i++){
		theta = i*(2.0*_PI_)/n;
		cos2theta = cos( 2.0*theta );
		r = sqrt( cos2theta + sqrt( cos2theta*cos2theta + a*a*a*a - 1));
		t1.x = r * cos(theta);
		t1.y = r * sin(theta);
		commitment_point.SetVal(i, t1);
	}
	//波原点配置
	POS ws;
	for(i=0; i<n; i++){
		if(i==0){
			t1 = commitment_point.GetVal(n-1);
			t2 = commitment_point.GetVal(0);
			t3 = commitment_point.GetVal(1);
		}
		else if(i==(n-1)){
			t1 = commitment_point.GetVal(n-2);
			t2 = commitment_point.GetVal(n-1);
			t3 = commitment_point.GetVal(0);
		}
		else{
			t1 = commitment_point.GetVal(i-1);
			t2 = commitment_point.GetVal(i);
			t3 = commitment_point.GetVal(i+1);
		}
		ws.x = t2.x - 0.5 * q * (t3.y - t1.y);
		ws.y = t2.y + 0.5 * q * (t3.x - t1.x);
		wave_source_point.SetVal(i, ws);
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

void CsmCassinixArea::DrawArea(int draw_mode)
{
	if(m_pModel){
		RenderSmgModelGL(m_pModel, draw_mode);
	}
}