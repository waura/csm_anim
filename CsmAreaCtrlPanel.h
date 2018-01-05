#ifndef _CSM_AREA_CTRL_PANEL_H_
#define _CSM_AREA_CTRL_PANEL_H_

#include <wx/spinctrl.h>

#include "ICsmAreaCtrl.h"
#include "GLCanvas.h"

#include <map>

typedef std::map<int, wxSpinCtrl*> SpinCtrlMap;
typedef std::map<int, wxTextCtrl*> TxtBoxMap;

class CsmAreaCtrlPanel :
	public ICsmAreaCtrl,
	public wxPanel
{
public:
	CsmAreaCtrlPanel(wxWindow* parent, wxWindowID id)
		: wxPanel(parent, id)
	{
		wxGridSizer *sizer = new wxGridSizer(2);
		SetSizer( sizer );
	}

	void ClearAllCtrl();
	
	void SetSpinCtrlVal(int id, int val);
	int GetSpinCtrlVal(int id);
	void AddSpinCtrl(const wxString& labe, int minVal, int maxVal, int val, int id);

	void SetTxtBoxVal(int id, wxString str);
	wxString GetTxtBoxVal(int id);
	void AddTxtBox(const wxString& labe, wxString str, int id);

private:
	SpinCtrlMap m_SpinCtrlMap;
	TxtBoxMap m_TxtBoxMap;
};

#endif //_CSM_AREA_CTRL_PANEL_H_