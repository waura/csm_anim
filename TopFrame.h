#ifndef _TOPFRAME_H_
#define _TOPFRAME_H_

#include "GLCanvas.h"
#include "CsmCtrlPanel.h"
#include "CsmAreaCtrlPanel.h"

class TopFrame : public wxFrame
{
public:
	TopFrame(const wxString& title);
	~TopFrame();

	void OnQuit(wxCommandEvent& WXUNUSED(event));
	
	DECLARE_EVENT_TABLE()
	
	void OnButton( wxCommandEvent &event );

private:
	wxButton* apply_button;
	wxPanel *main_panel;
	wxPanel *right_panel;
	GLCanvas *gl_canvas;

	CsmCtrlPanel *csm_ctrl_panel;
	CsmAreaCtrlPanel *csm_area_ctrl_panel;

	CsmAnimGL csm_anim_gl;

	wxMenuBar *menubar_;
	wxMenu *file_;	
};

#endif //TOPFRAME_H_