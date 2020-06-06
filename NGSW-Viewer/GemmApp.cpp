#include "GemmApp.h"
#include "MainFrame.h"

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP(GemmApp);

// 'Main program' equivalent: the program execution "starts" here
bool GemmApp::OnInit()
{
    // create the main application window
    wxPoint initialLocation = wxPoint(50, 50);
    wxSize initialSize = wxSize(900, 600);
    MainFrame* frame = new MainFrame(_T("GEMM"), initialLocation, initialSize);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}