#ifndef _ICSMAREACTRL_H_
#define _ICSMAREACTRL_H_

#include <wx/wx.h>

class ICsmAreaCtrl
{
public:
	virtual void ClearAllCtrl() =0;

	virtual void SetSpinCtrlVal(int id, int val)=0;
	virtual int GetSpinCtrlVal(int id)=0;
	virtual void AddSpinCtrl(const wxString& labe, int minVal, int maxVal, int val, int id) =0;

	virtual void SetTxtBoxVal(int id, wxString str)=0;
	virtual wxString GetTxtBoxVal(int id)=0;
	virtual void AddTxtBox(const wxString& labe, wxString str, int id) =0;
};

#endif