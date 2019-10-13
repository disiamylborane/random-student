
#include "random_student.h"


#define RANDOMIZER_CONSTS    \
	const int width = GetSize().x;     \
	const int height = GetSize().y;     \
	const int rows = disabled.size() / cols / groupsize;     \
	const int elems_in_row = (groupsize+1)*cols - 1;     \
	const int elem_width = width/elems_in_row;     \
	const int elem_height = height/rows;     \
	const int radius = std::min(width/elems_in_row, height/rows) * 7 / 16 - 1;


BEGIN_EVENT_TABLE ( RandomizerPanel, wxPanel )
    EVT_LEFT_UP(RandomizerPanel::click)
	EVT_PAINT(RandomizerPanel::paintEvent)
END_EVENT_TABLE()


RandomizerPanel::RandomizerPanel(wxPanel* parent, int _rows, int _cols, int _groupsize)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE),
    cols(_cols), groupsize(_groupsize), currently_chosen(-1)
{
    disabled.resize(_rows*_cols*_groupsize);
    SetDoubleBuffered(true);
};


void RandomizerPanel::enable_all(bool enabled)
{
	for(auto&& d : disabled)
		d = !enabled;

	Refresh();
}


void RandomizerPanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);

	RANDOMIZER_CONSTS

	for (int row=0; row<rows; row++){
		for (int col=0; col<cols; col++){
			dc.SetBrush(*wxWHITE_BRUSH);
			dc.DrawRectangle(
				(groupsize+1)*col * elem_width,
				row * elem_height+1,
				groupsize*elem_width-2,
				elem_height-2);

			for (int gr=0; gr<groupsize; gr++){
				const int pos_x = gr + (groupsize+1)*col;
				const int center_x = pos_x * elem_width + elem_width/2;
				const int center_y = row * elem_height + elem_height/2;

				int elem_index = row * (cols * groupsize) + col * groupsize + gr;
				if(disabled[elem_index])
					dc.SetBrush(*wxWHITE_BRUSH);
				else if(elem_index==currently_chosen)
					dc.SetBrush(*wxRED_BRUSH);
				else
					dc.SetBrush(*wxGREEN_BRUSH);

				dc.DrawCircle(center_x, center_y, radius);
			}
		}
	}
}


void RandomizerPanel::activate(unsigned int student)
{
    currently_chosen = student;
    Refresh();
};


void RandomizerPanel::click(wxMouseEvent & evt)
{
	RANDOMIZER_CONSTS

	const int row = evt.GetY() * rows / height;
	const int elem_x = evt.GetX() / elem_width;

	if(elem_x % (groupsize+1) != groupsize){
		const int centerx = elem_x * elem_width + elem_width/2;
		const int centery = row * elem_height + elem_height/2;

		auto dx = centerx-evt.GetX();
		auto dy = centery-evt.GetY();
		auto d2 = dx*dx + dy*dy;

		if(d2 < radius*radius){
            const int col = elem_x / (groupsize+1);
            const int gr = elem_x % (groupsize+1);
            int elem_index = row * (cols * groupsize) + col * groupsize + gr;
            disabled[elem_index] = !disabled[elem_index];
		}
	}

    currently_chosen = -1;

	Refresh();
}


