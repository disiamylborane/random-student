
#include "random_student.h"



BEGIN_EVENT_TABLE ( BoardPanel, wxPanel )
	EVT_PAINT(BoardPanel::paintEvent)
END_EVENT_TABLE()



void BoardPanel::paintEvent(wxPaintEvent & evt)
{
	const int width = GetSize().x;
	const int height = GetSize().y;

    wxPaintDC dc(this);
	dc.DrawRectangle(width/3, height/6, width/3, height/4);
}



BoardPanel::BoardPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE)
{
	SetDoubleBuffered(true);
};
