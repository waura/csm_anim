#include "GLCanvas.h"

GLCanvas::GLCanvas(wxWindow* parent, wxGLContext* sharedContext, wxWindowID id)
	: wxGLCanvas(parent, sharedContext, id), isInitialized_(false)
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(GLCanvas::OnSize));
	Connect(wxEVT_IDLE, wxIdleEventHandler(GLCanvas::OnIdle));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground));
}

void GLCanvas::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	if(!GetContext()){
		return;
	}

	if(!isInitialized_){
		SetCurrent();
		gl_->Init();
		int w;
		int h;
		GetClientSize(&w, &h);
		gl_->SetViewport(w, h);
		isInitialized_=true;
	}

	gl_->Draw();

	glFlush();
	SwapBuffers();
}

void GLCanvas::OnSize(wxSizeEvent &event)
{
	wxGLCanvas::OnSize(event);

	if(GetContext()){
		SetCurrent();
		gl_->SetViewport(event.GetSize().GetWidth(), event.GetSize().GetHeight());
		Refresh();
	}
}

void GLCanvas::OnIdle(wxIdleEvent &event)
{
	event.RequestMore(true);	
	gl_->Idle();
	Refresh(false);
}

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_MOUSEWHEEL(GLCanvas::OnMouseWheel)
	EVT_LEFT_DOWN(GLCanvas::OnMouseLDown)
	EVT_LEFT_UP(GLCanvas::OnMouseLUp)
	EVT_MOTION(GLCanvas::OnMouseMove)
END_EVENT_TABLE()

void GLCanvas::OnMouseWheel(wxMouseEvent &event)
{
	gl_->MouseWheel(event.GetWheelRotation(), event.GetWheelDelta());
}

void GLCanvas::OnMouseLDown(wxMouseEvent &event)
{
	this->SetFocus();
	gl_->MouseButton(GLUT_LEFT_BUTTON, GLUT_DOWN, event.m_x, event.m_y);
}

void GLCanvas::OnMouseLUp(wxMouseEvent &event)
{
	gl_->MouseButton(GLUT_LEFT_BUTTON, GLUT_UP, event.m_x, event.m_y);
}

void GLCanvas::OnMouseMove(wxMouseEvent &event)
{
	gl_->MouseMove(event.m_x, event.m_y);
}