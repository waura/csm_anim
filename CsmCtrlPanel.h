#ifndef _CSM_CTRL_PANEL_H_
#define _CSM_CTRL_PANEL_H_

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "CsmAnimGL.h"

class CsmCtrlPanel :
	public wxPanel
{
public:
	CsmCtrlPanel(wxWindow* parent, wxWindowID id);
	~CsmCtrlPanel(){};

	void SetCsmAnimGL(CsmAnimGL* pCsmAnimGL){
		m_pCsmAnimGL = pCsmAnimGL;
	}

	void SetValToCsmAnimGL();
	
	DECLARE_EVENT_TABLE()

	void OnSpinCtrl( wxSpinEvent &event );
	void OnSpinCtrlUp( wxSpinEvent &event );
	void OnSpinCtrlDown( wxSpinEvent &event );
	void OnSpinCtrlText( wxCommandEvent &event );
	void OnCombo( wxCommandEvent &event );

	
	
private:
	bool m_isSpin;
	bool m_isEditSpinText;
	
	int m_draw_mode_id;
	wxComboBox* m_draw_mode_combo;
	wxTextCtrl* m_poly_size_txtbox;
	wxSpinCtrl* m_xsize_spinctrl;
	wxSpinCtrl* m_ysize_spinctrl;
	wxSpinCtrl* m_N_spinctrl;
	wxSpinCtrl* m_k_spinctrl;
	wxSpinCtrl* m_c_spinctrl;
	wxComboBox* m_area_select_combo;

	CsmAnimGL* m_pCsmAnimGL;
};

#endif //_CSM_CTRL_PANEL_H_