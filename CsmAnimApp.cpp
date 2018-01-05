
#include "CsmAnimApp.h"

bool CsmAnimApp::OnInit()
{ 
	TopFrame *frame = new TopFrame(wxT("CSM viewer"));
	frame->SetSize(800, 600);
	frame->Show(true);
	return true;
}

IMPLEMENT_APP(CsmAnimApp)
