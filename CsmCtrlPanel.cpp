#include "CsmCtrlPanel.h"

enum{
	ID_DRAW_MODE_COMBO=1,
	ID_POLY_SIZE_TXTBOX,
	ID_XSIZE_SPINCTRL,
	ID_YSIZE_SPINCTRL,
	ID_N_SPINCTRL,
	ID_K_SPINCTRL,
	ID_C_SPINCTRL,
	ID_AREA_SELECT_COMBO,
};

CsmCtrlPanel::CsmCtrlPanel(wxWindow* parent, wxWindowID id)
	: wxPanel(parent, id)
{
	m_pCsmAnimGL = NULL;

	wxString draw_mode_choices[] =
	{
		_T("Polygon"),
		_T("Wire")
	};

	m_draw_mode_id = DRAW_POLY;
	m_draw_mode_combo = new wxComboBox(
		this,
		ID_DRAW_MODE_COMBO,
		_T("Polygon"),
		wxPoint(5,5),
		wxSize(wxDefaultCoord, wxDefaultCoord),
		2,
		draw_mode_choices,
		wxTE_PROCESS_ENTER);

	m_poly_size_txtbox = new wxTextCtrl(
		this,
		ID_POLY_SIZE_TXTBOX);
	m_poly_size_txtbox->SetValue(CSM_DEFAULT_POLY_SIZE_STR);

	m_xsize_spinctrl = new wxSpinCtrl(
		this,
		ID_XSIZE_SPINCTRL);
	m_xsize_spinctrl->SetRange(10, 200);
	m_xsize_spinctrl->SetValue(40);

	m_ysize_spinctrl = new wxSpinCtrl(
		this,
		ID_YSIZE_SPINCTRL);
	m_ysize_spinctrl->SetRange(10, 200);
	m_ysize_spinctrl->SetValue(40);

	m_N_spinctrl = new wxSpinCtrl(
		this,
		ID_N_SPINCTRL);
	m_N_spinctrl->SetRange(16, 80);
	m_N_spinctrl->SetValue(CSM_DEFAULT_n);

	m_k_spinctrl = new wxSpinCtrl(
		this,
		ID_K_SPINCTRL);
	m_k_spinctrl->SetRange(1, 20);
	m_k_spinctrl->SetValue(CSM_DEFAULT_k);

	m_c_spinctrl = new wxSpinCtrl(
		this,
		ID_C_SPINCTRL);
	m_c_spinctrl->SetRange(1, 20);
	m_c_spinctrl->SetValue(CSM_DEFAULT_c);

	wxString area_select_choices[] =
	{
		_T("Circle"),
		_T("Ellipse"),
		_T("Cassini X"),
		_T("Cassini Y")
	};

	m_area_select_combo = new wxComboBox(
		this,
		ID_AREA_SELECT_COMBO,
		_T("Circle"),
		wxPoint(5, 5),
		wxSize(wxDefaultCoord, wxDefaultCoord),
		4,
		area_select_choices,
		wxTE_PROCESS_ENTER);

	wxGridSizer *sizer = new wxGridSizer(2);

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("Display Mode")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_draw_mode_combo,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("Polygon Size")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_poly_size_txtbox,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("Range of X-Direction")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_xsize_spinctrl,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("Range of Y-Direction")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_ysize_spinctrl,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("N")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_N_spinctrl,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("k")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_k_spinctrl,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("c")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_c_spinctrl,
		wxSizerFlags().Align(wxALIGN_LEFT));

	sizer->Add(new wxStaticText(this, wxID_ANY, _T("Area Type")),
		wxSizerFlags().Align(wxALIGN_LEFT));
	sizer->Add( m_area_select_combo,
		wxSizerFlags().Align(wxALIGN_LEFT));

	SetSizer( sizer );
}

BEGIN_EVENT_TABLE(CsmCtrlPanel, wxPanel)
	EVT_COMBOBOX(ID_DRAW_MODE_COMBO, CsmCtrlPanel::OnCombo)
	EVT_SPINCTRL(ID_N_SPINCTRL, CsmCtrlPanel::OnSpinCtrl)
	EVT_SPIN_UP(ID_N_SPINCTRL, CsmCtrlPanel::OnSpinCtrlUp)
	EVT_SPIN_DOWN(ID_N_SPINCTRL, CsmCtrlPanel::OnSpinCtrlDown)
	EVT_TEXT(ID_N_SPINCTRL, CsmCtrlPanel::OnSpinCtrlText)
	EVT_SPINCTRL(ID_K_SPINCTRL, CsmCtrlPanel::OnSpinCtrl)
	EVT_SPIN_UP(ID_K_SPINCTRL, CsmCtrlPanel::OnSpinCtrlUp)
	EVT_SPIN_DOWN(ID_K_SPINCTRL, CsmCtrlPanel::OnSpinCtrlDown)
	EVT_TEXT(ID_K_SPINCTRL, CsmCtrlPanel::OnSpinCtrlText)
	EVT_SPINCTRL(ID_C_SPINCTRL, CsmCtrlPanel::OnSpinCtrl)
	EVT_SPIN_UP(ID_C_SPINCTRL, CsmCtrlPanel::OnSpinCtrlUp)
	EVT_SPIN_DOWN(ID_C_SPINCTRL, CsmCtrlPanel::OnSpinCtrlDown)
	EVT_TEXT(ID_C_SPINCTRL, CsmCtrlPanel::OnSpinCtrlText)
	EVT_COMBOBOX(ID_AREA_SELECT_COMBO, CsmCtrlPanel::OnCombo)
END_EVENT_TABLE()

void CsmCtrlPanel::OnSpinCtrl(wxSpinEvent &event)
{
#ifdef _WIN32
	::OutputDebugString( wxString::Format(
		_T("Spin ctrl changed: (from event: %d)\n"),
		event.GetInt()) );
#endif
}

void CsmCtrlPanel::OnSpinCtrlUp(wxSpinEvent &event)
{
#ifdef _WIN32
	::OutputDebugString( wxString::Format(
		_T("Spin up: (from event: %d)\n"),
		event.GetInt()) );
#endif

	m_isSpin = true;
}

void CsmCtrlPanel::OnSpinCtrlDown(wxSpinEvent &event)
{
#ifdef _WIN32
	::OutputDebugString( wxString::Format(
		_T("Spin down: (from event: %d)\n"),
		event.GetInt()) );
#endif

	m_isSpin = true;
}

void CsmCtrlPanel::OnSpinCtrlText(wxCommandEvent &event)
{
#ifdef _WIN32
	::OutputDebugString( wxString::Format(
		_T("Spin ctrl text: (from event: %s)\n"),
		event.GetString().c_str()) );
#endif
}

void CsmCtrlPanel::OnCombo(wxCommandEvent &event)
{
	int index = event.GetInt();
	switch(event.GetId()){
		case ID_DRAW_MODE_COMBO:
			m_draw_mode_id = index;
			break;
		case ID_AREA_SELECT_COMBO:
			m_pCsmAnimGL->SetCsmArea( index );
			break;
	}
}

void CsmCtrlPanel::SetValToCsmAnimGL(){
	m_pCsmAnimGL->SetDrawMode(m_draw_mode_id);
	m_pCsmAnimGL->SetPolySize( atof(m_poly_size_txtbox->GetValue().c_str()) );
	m_pCsmAnimGL->SetXSize( m_xsize_spinctrl->GetValue() );
	m_pCsmAnimGL->SetYSize( m_ysize_spinctrl->GetValue() );
	m_pCsmAnimGL->SetN( m_N_spinctrl->GetValue() );
	m_pCsmAnimGL->Setk( m_k_spinctrl->GetValue() );
	m_pCsmAnimGL->Setc( m_c_spinctrl->GetValue() );
}