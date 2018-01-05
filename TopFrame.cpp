#include "TopFrame.h"

enum{
	ID_APPLY_BUTTON=1,
};

TopFrame::TopFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title)
{
	menubar_ = new wxMenuBar;
	file_ = new wxMenu;
	file_->Append(wxID_EXIT, wxT("&Quit"));
	menubar_->Append(file_, wxT("&File"));
	SetMenuBar(menubar_);

	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(TopFrame::OnQuit));

	CreateStatusBar();

	main_panel = new wxPanel(this, wxID_ANY);
	
	right_panel = new wxPanel(main_panel, wxID_ANY);
	right_panel->SetSize(100, 600);

	apply_button = new wxButton(
		right_panel,
		ID_APPLY_BUTTON,
		_T("apply"),
		wxPoint(5, 5),
		wxSize(40, wxDefaultCoord));
	csm_ctrl_panel = new CsmCtrlPanel(right_panel, wxID_ANY);
	csm_ctrl_panel->SetCsmAnimGL( &csm_anim_gl );
	csm_area_ctrl_panel = new CsmAreaCtrlPanel(right_panel, wxID_ANY);
	csm_area_ctrl_panel->SetSize(100, 600);
	csm_anim_gl.SetCsmAreaCtrl( csm_area_ctrl_panel );
	wxBoxSizer *right_sizer = new wxBoxSizer( wxVERTICAL );
	right_sizer->Add(5, 5);
	right_sizer->Add(apply_button, 0, wxALIGN_RIGHT);
	right_sizer->Add(5, 5);
	right_sizer->Add(csm_ctrl_panel, 0);
	right_sizer->Add(csm_area_ctrl_panel, 0);
	right_panel->SetSizer( right_sizer );

	gl_canvas = new GLCanvas(main_panel, NULL, wxID_ANY);
	gl_canvas->SetCsmAnimGL(&csm_anim_gl);

	wxBoxSizer *main_sizer = new wxBoxSizer( wxHORIZONTAL );
	main_sizer->Add(gl_canvas, 1, wxEXPAND);
	main_sizer->Add(10,10);
	main_sizer->Add(right_panel, 0, wxALIGN_RIGHT|wxEXPAND);
	main_panel->SetSizer( main_sizer );

	csm_anim_gl.SetCsmArea(0);
	csm_anim_gl.ResetCsm();
}

TopFrame::~TopFrame()
{
}

void TopFrame::OnQuit(wxCommandEvent& WXUNUSED(event)){
	Close(true);
}

BEGIN_EVENT_TABLE(TopFrame, wxFrame)
	EVT_BUTTON(ID_APPLY_BUTTON, TopFrame::OnButton)
END_EVENT_TABLE()

void TopFrame::OnButton(wxCommandEvent &event)
{
	switch(event.GetId()){
		case ID_APPLY_BUTTON:
#ifdef _WIN32
			::OutputDebugString("event: apply button \n");
#endif
			csm_ctrl_panel->SetValToCsmAnimGL();
			csm_anim_gl.ResetCsm();
			break;
	}
}
