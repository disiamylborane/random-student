

#include "random_student.h"


#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>


enum
{
	BUTTON_CallStudent = wxID_HIGHEST + 1,
	BUTTON_Save,
	BUTTON_Load,
	SPIN_Row,
	SPIN_Col,
	SPIN_Group,
};



MainFrame::MainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(640, 480))
{
	setup_ui();
	_load_state();
}


void MainFrame::setup_ui()
{
	mainPanel = new wxPanel(this);

	teacherPanel = new BoardPanel(mainPanel);
	studentPanel = new RandomizerPanel(mainPanel, VAL_ROWS, VAL_COLS, VAL_GROUPS);
	controlsPanel = new wxPanel(mainPanel);

	bCallStudent = new wxButton(controlsPanel, BUTTON_CallStudent, _T(STR_CALL));

	sRows = new wxSpinButton(controlsPanel, SPIN_Row);
	sColumns = new wxSpinButton(controlsPanel, SPIN_Col);
	sGroup = new wxSpinButton(controlsPanel, SPIN_Group);
	sRows->SetRange(1,10);
	sColumns->SetRange(1,10);
	sGroup->SetRange(1,10);
	sRows->SetValue(VAL_ROWS);
	sColumns->SetValue(VAL_COLS);
	sGroup->SetValue(VAL_GROUPS);

	auto lRow = new wxStaticText(controlsPanel, -1, _T(STR_TABLES));
	auto lCol = new wxStaticText(controlsPanel, -1, _T(STR_ROWS));
	auto lGr = new wxStaticText(controlsPanel, -1, _T(STR_PLACES));

	bSave = new wxButton(controlsPanel, BUTTON_Save, _T(STR_SAVE));
	bLoad = new wxButton(controlsPanel, BUTTON_Load, _T(STR_LOAD));

	spinBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	spinBoxSizer->Add(bSave, 1, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->Add(bLoad, 1, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->AddStretchSpacer(7);
	spinBoxSizer->Add(lRow, 1, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->Add(sRows, 0, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->AddStretchSpacer(1);
	spinBoxSizer->Add(lCol, 1, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->Add(sColumns, 0, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->AddStretchSpacer(1);
	spinBoxSizer->Add(lGr, 1, wxALIGN_CENTER_VERTICAL);
	spinBoxSizer->Add(sGroup, 0, wxALIGN_CENTER_VERTICAL);

	ctrlSizer = new wxBoxSizer(wxVERTICAL);
	ctrlSizer->AddStretchSpacer(1);
	ctrlSizer->Add(bCallStudent, 3, wxALIGN_CENTER_HORIZONTAL);
	ctrlSizer->AddStretchSpacer(1);
	ctrlSizer->Add(spinBoxSizer, 3, wxALIGN_CENTER_HORIZONTAL);

	controlsPanel->SetSizer(ctrlSizer);

	mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(teacherPanel, 1, wxEXPAND);
	mainSizer->Add(studentPanel, 3, wxEXPAND);
	mainSizer->Add(controlsPanel, 1, wxEXPAND);

	mainPanel->SetSizer(mainSizer);
}


void MainFrame::call_student(wxCommandEvent& event) {
	int student_count = std::count(studentPanel->disabled.begin(), studentPanel->disabled.end(), false);
	int random_student = std::rand() % student_count;

	// Effect 1. Stop the student highlighting and wait for a second
	studentPanel->activate(-1);
	wxMilliSleep(sleeptime_ms);

	// Effect 2. Highlight the random student
	for(int i=0; i<studentPanel->disabled.size(); i++){
		if(studentPanel->disabled[i])
			continue;
		if(random_student == 0) {
			studentPanel->activate(i);
			break;
		}
		random_student--;
	}
}



void MainFrame::spin_row(wxSpinEvent& event)
{
	studentPanel->disabled.resize(event.GetPosition() * studentPanel->cols * studentPanel->groupsize);

	studentPanel->Refresh();
}


void MainFrame::spin_col(wxSpinEvent& event)
{
	auto rows = studentPanel->disabled.size() / (studentPanel->cols * studentPanel->groupsize);
	studentPanel->cols = event.GetPosition();
	studentPanel->disabled.resize(rows * studentPanel->cols * studentPanel->groupsize);

	studentPanel->enable_all(true);
}


void MainFrame::spin_group(wxSpinEvent& event)
{
	auto rows = studentPanel->disabled.size() / (studentPanel->cols * studentPanel->groupsize);
	studentPanel->groupsize = event.GetPosition();
	studentPanel->disabled.resize(rows * studentPanel->cols * studentPanel->groupsize);

	studentPanel->enable_all(true);
}


void MainFrame::save_state(wxCommandEvent& event)
{
	std::ofstream myfile;
    myfile.open (FILENAME, std::ios_base::out | std::ios_base::trunc);
	myfile << studentPanel->cols << std::endl;
	myfile << studentPanel->groupsize << std::endl;
	myfile << studentPanel->disabled.size() << std::endl;
	for(auto d: studentPanel->disabled)
		myfile << d;
}


void MainFrame::_load_state()
{
	std::ifstream myfile;
	myfile.open(FILENAME);
	if (myfile.fail())
		return;

	std::string line;

	std::getline(myfile, line);
	std::istringstream iss(line);
	if (!(iss >> studentPanel->cols))
		return;

	std::getline(myfile, line);
	iss = std::istringstream(line);
	if (!(iss >> studentPanel->groupsize))
		return;

	size_t arrsize = 0;

	std::getline(myfile, line);
	iss = std::istringstream(line);
	if (!(iss >> arrsize))
		return;

	studentPanel->disabled.resize(arrsize);

	std::getline(myfile, line);

	if (line.size() != arrsize)
		return;

	for(int i=0; i<arrsize; i++)
		studentPanel->disabled[i] = line[i]=='1';

	sRows->SetValue(studentPanel->disabled.size() / (studentPanel->cols*studentPanel->groupsize));
	sColumns->SetValue(studentPanel->cols);
	sGroup->SetValue(studentPanel->groupsize);

    studentPanel->activate(-1);
}


BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
	EVT_BUTTON ( BUTTON_CallStudent, MainFrame::call_student )
	EVT_BUTTON ( BUTTON_Save, MainFrame::save_state )
	EVT_BUTTON ( BUTTON_Load, MainFrame::load_state )
	EVT_SPIN ( SPIN_Col, MainFrame::spin_col )
	EVT_SPIN ( SPIN_Row, MainFrame::spin_row )
	EVT_SPIN ( SPIN_Group, MainFrame::spin_group )
END_EVENT_TABLE()



class MyApp : public wxApp {
	public: virtual bool OnInit();
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	std::srand(unsigned(std::time(0)));

	MainFrame *window = new MainFrame(_T(STR_TITLE));
	window->Show(true);
	return true;
}
