
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include <tuple>
using std::vector;
using std::tuple;


#include "labels.h"

// Settings

#define FILENAME "random_student.dat"

// Default map size
const int VAL_ROWS = 4;
const int VAL_COLS = 3;
const int VAL_GROUPS = 2;

// Sleep after "choose" button is clicked
const int sleeptime_ms = 1000;


// A panel representing a blackboard
class BoardPanel : public wxPanel
{
public:
	BoardPanel(wxPanel* parent);
	void paintEvent(wxPaintEvent & evt);
    DECLARE_EVENT_TABLE()
};



// A panel representing desks and students
class RandomizerPanel : public wxPanel
{
public:
	RandomizerPanel(wxPanel* parent, int _rows, int _cols, int _groupsize);

	void paintEvent(wxPaintEvent & evt);

	void click(wxMouseEvent & evt);
    void render(wxDC& dc);
	void enable_all(bool enabled);
	void activate(unsigned int student);

	vector<bool> disabled;  // Places that are removed from the choice
	int cols, groupsize;  // Number of columns and places on a single desk
	unsigned int currently_chosen;  // The student that is highlighted

    DECLARE_EVENT_TABLE()
};



// Main window
class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);


	// GUI helpers
	wxPanel* mainPanel;
	wxBoxSizer *ctrlSizer;
	wxBoxSizer *spinBoxSizer;
	wxBoxSizer *mainSizer;


	// GUI panels
	wxPanel* controlsPanel;
	BoardPanel* teacherPanel;
	RandomizerPanel* studentPanel;


	// Controls
    wxButton *bSave;
    wxButton *bLoad;
    wxButton *bCallStudent;
	wxSpinButton *sRows;
	wxSpinButton *sColumns;
	wxSpinButton *sGroup;

	// Events
	void spin_row(wxSpinEvent& event); // Add or remove rows
	void spin_col(wxSpinEvent& event); // Add or remove columns
	void spin_group(wxSpinEvent& event); // Add or remove an amount of students by one desk

	void call_student( wxCommandEvent& event );
	void save_state( wxCommandEvent& event );

    DECLARE_EVENT_TABLE()

private:
	void setup_ui();
	void load_state( wxCommandEvent& event ) {_load_state();};
	void _load_state();
};
