#include <iostream>
#include <string>
#include "GemmMainFrame.h"
#include "GemmSurfaceMeshReader.h"
#include "GemmProstar3SurfaceMeshReader.h"

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About = 2,
    Minimal_About_VTK = 3
};

//#define MY_FRAME      101
#define MY_VTK_WINDOW 102

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(GemmMainFrame, wxFrame)
EVT_MENU(Minimal_Quit, GemmMainFrame::onQuit)
EVT_MENU(Minimal_About, GemmMainFrame::onAbout)
EVT_MENU(Minimal_About_VTK, GemmMainFrame::onAboutVTK)
wxEND_EVENT_TABLE();

// frame constructor
GemmMainFrame::GemmMainFrame(GemmApp* app, const wxString& title, const wxPoint& pos, 
    const wxSize& size) : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    // notify wxAUI which frame to use
    wxAuiMgr.SetManagedWindow(this);

#ifdef __WXMAC__
    // we need this in order to allow the about menu relocation, since ABOUT is
    // not the default id of the about menu
    wxApp::s_macAboutMenuItemId = Minimal_About;
#endif

    // set the frame icon
//    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu* menuFile = new wxMenu(_T(""), wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About GEMM...\tCtrl-A"), _T("Show About dialog"));
    helpMenu->Append(Minimal_About_VTK, _T("&About VTK...\tCtrl-V"), _T("Show About VTK dialog"));

    menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Cursor coordinates here (wxWidgets 3.1.3)"));
#endif // wxUSE_STATUSBAR

    // create several widgets
    wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
        wxDefaultPosition, wxSize(200, 150),
        wxNO_BORDER | wxTE_MULTILINE);

    wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
        wxDefaultPosition, wxSize(200, 150),
        wxNO_BORDER | wxTE_MULTILINE);

    wxSize initialSize = wxSize(720, 480);
    wxPoint initialLocation = wxPoint(0, 0);
    renderWindowInteractor = new wxVTKRenderWindowInteractor(this, 102, initialLocation, initialSize);
    wxColour backgroundColor = wxColour(200, 0, 0);
    renderWindowInteractor->SetBackgroundColour(backgroundColor);
    renderWindow = renderWindowInteractor->GetRenderWindow();
    renderer = vtkRenderer::New();
    // connect renderer and render window and configure render window
    renderWindow->SetSize(1000, 1000);
    renderWindow->AddRenderer(renderer);
    // read in geometry and set renderer
    //newVessel();
    // configure renderer
    renderer->GradientBackgroundOff();
    renderer->SetBackground(0.75, 0.75, 0.75);
    renderer->SetBackground(0, 0, 0.75);

    // add the panes to the manager
    wxAuiMgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
    wxAuiMgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
    wxAuiMgr.AddPane(renderWindowInteractor, wxCENTER, wxT("Vis Pane"));

    std::string vrtFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.vrt";
    std::string celFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.cel";
    GemmProstar3SurfaceMeshReader* gsmr1 = new GemmProstar3SurfaceMeshReader(this, vrtFileName, celFileName);
    text2->AppendText(" is here " + std::to_string((int)gsmr1->getGemmSurfaceMeshReaderType()));
    gsmr1->render(renderer);

    // tell the manager to "commit" all the changes just made
    wxAuiMgr.Update();
}

GemmMainFrame::~GemmMainFrame()
{
    // deinitialize the frame manager
    wxAuiMgr.UnInit();
}

//
// event handlers:
//
void GemmMainFrame::onQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void GemmMainFrame::onAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("This is GEMM 2020 written by Bob Brewster.It is written in C++"
        "using wxWidgets for the GUI widgets and VTK for visualization."));
    wxMessageBox(msg, _T("About GEMM"), wxOK | wxICON_INFORMATION, this);
}

void GemmMainFrame::onAboutVTK(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("GEMM 2020 uses VTK version 8.2.0. The Visualization Toolkit (VTK) is "
        "an open source, freely available system for 3D computer graphics, image processing "
        "and visualization provided by Kitware, Inc.For further information see: "
        "<a href=\"http://www.vtk.org\">www.vtk.org</a>"));
    wxMessageBox(msg, _T("About VTK"), wxOK | wxICON_INFORMATION, this);
}

//const vtkSmartPointer<vtkRenderer> GemmMainFrame::getRenderer() const
//{
//    return renderer;
//}

//void GemmMainFrame::newVessel()
//{
//    std::string vrtFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.vrt";
//    std::string celFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.cel";
//
//    if ((!vrtFileName.empty()) && (!celFileName.empty()))
//    {
//        GemmProstar3SurfaceMeshReader* model = new GemmProstar3SurfaceMeshReader(this, vrtFileName, celFileName);
//
//        renderer->AddActor(model->getActor());
//        renderer->ResetCamera();
//#if wxUSE_STATUSBAR
//        this->GetStatusBar()->SetStatusText(_T("Vessel file reads completed"));
//#endif // wxUSE_STATUSBAR
//    }
//    else
//    {
//#if wxUSE_STATUSBAR
//        this->GetStatusBar()->SetStatusText(_T("Vessel file reads canceled"));
//#endif // wxUSE_STATUSBAR
//    }
//}