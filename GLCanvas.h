#ifndef _GLCANVAS_H_
#define _GLCANVAS_H_

#include "CsmAnimGL.h"

class GLCanvas : public wxGLCanvas
{
	bool isInitialized_;
	CsmAnimGL* gl_;
 
public:
	GLCanvas(wxWindow* parent, wxGLContext* sharedContext, wxWindowID id);
 
	void OnPaint(wxPaintEvent &event); 
	void OnSize(wxSizeEvent &event); 
	void OnIdle(wxIdleEvent &event);
	void OnEraseBackground(wxEraseEvent& event){
		// Do nothing, to avoid flashing.
	}

	void OnMouseWheel(wxMouseEvent &event);
	void OnMouseLDown(wxMouseEvent &event);
	void OnMouseLUp(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);

	void SetCsmAnimGL(CsmAnimGL* gl){
		gl_ = gl;
	}

	DECLARE_EVENT_TABLE();
};

#endif //_GLCANVAS_H_