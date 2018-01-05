#include "CsmAreaCtrlPanel.h"

void CsmAreaCtrlPanel::ClearAllCtrl()
{
	this->GetSizer()->Clear();
	this->DestroyChildren();
	m_SpinCtrlMap.clear();
	m_TxtBoxMap.clear();
}

void CsmAreaCtrlPanel::SetSpinCtrlVal(int id, int val)
{
	SpinCtrlMap::iterator itr;
	itr = m_SpinCtrlMap.find(id);
	if(itr != m_SpinCtrlMap.end()){
		return itr->second->SetValue(val);
	}
	else{
#ifdef _WIN32
		::OutputDebugString("error: CsmAreaCtrlPanel::GetSpinCtrl() don't exist spin ctrl id \n");
#endif
	}
}

int CsmAreaCtrlPanel::GetSpinCtrlVal(int id)
{
	SpinCtrlMap::iterator itr;
	itr = m_SpinCtrlMap.find(id);
	if(itr != m_SpinCtrlMap.end()){
		return itr->second->GetValue();
	}
	else{
#ifdef _WIN32
		::OutputDebugString("error: CsmAreaCtrlPanel::GetSpinCtrl() don't exist spin ctrl id \n");
#endif
	}
	return 0;
}

void CsmAreaCtrlPanel::AddSpinCtrl(
	const wxString& label,
	int minVal,
	int maxVal,
	int val,
	int id)
{
	wxSpinCtrl* spin_ctrl = new wxSpinCtrl(this, id);
	wxSizer* sizer = GetSizer();
	sizer->Add(new wxStaticText(this, wxID_ANY, label));
	sizer->Add(spin_ctrl);

	spin_ctrl->SetRange(minVal, maxVal);
	spin_ctrl->SetValue(val);
	
	//sizer->Show(true);
	sizer->Layout();

	this->GetParent()->Layout();

	m_SpinCtrlMap.insert(std::pair<int , wxSpinCtrl*>(id ,spin_ctrl));
}

void CsmAreaCtrlPanel::SetTxtBoxVal(int id, wxString str)
{
	TxtBoxMap::iterator itr;
	itr = m_TxtBoxMap.find(id);
	if(itr != m_TxtBoxMap.end()){
		return itr->second->SetValue(str);
	}
	else{
#ifdef _WIN32
		::OutputDebugString("error: CsmAreaCtrlPanel::GetSpinCtrl() don't exist spin ctrl id \n");
#endif
	}
}

wxString CsmAreaCtrlPanel::GetTxtBoxVal(int id)
{
	TxtBoxMap::iterator itr;
	itr = m_TxtBoxMap.find(id);
	if(itr != m_TxtBoxMap.end()){
		return itr->second->GetValue();
	}
	else{
#ifdef _WIN32
		::OutputDebugString("error: CsmAreaCtrlPanel::GetSpinCtrl() don't exist spin ctrl id \n");
#endif
	}
	return "";
}

void CsmAreaCtrlPanel::AddTxtBox(const wxString& label, wxString str, int id)
{
	wxTextCtrl* spin_ctrl = new wxTextCtrl(this, id);
	wxSizer* sizer = GetSizer();
	sizer->Add(new wxStaticText(this, wxID_ANY, label));
	sizer->Add(spin_ctrl);

	spin_ctrl->SetValue(str);
	
	//sizer->Show(true);
	sizer->Layout();

	this->GetParent()->Layout();

	m_TxtBoxMap.insert(std::pair<int , wxTextCtrl*>(id ,spin_ctrl));
}