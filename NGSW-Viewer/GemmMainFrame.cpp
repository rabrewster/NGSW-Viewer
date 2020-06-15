//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>

#include "GemmMainFrame.h"
#include "STAR3ShellModel.h"

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
EVT_MENU(Minimal_Quit, GemmMainFrame::OnQuit)
EVT_MENU(Minimal_About, GemmMainFrame::OnAbout)
EVT_MENU(Minimal_About_VTK, GemmMainFrame::OnAboutVTK)
EVT_SIZE(GemmMainFrame::OnSize)
wxEND_EVENT_TABLE();

// frame constructor
GemmMainFrame::GemmMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    // notify wxAUI which frame to use
    m_mgr.SetManagedWindow(this);

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

    // Create visualization panel and add to frame
    //visPanel = new GemmVisPanel(this, this->GetPosition(), this->GetSize());

    // create several text controls
    wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
        wxDefaultPosition, wxSize(200, 150),
        wxNO_BORDER | wxTE_MULTILINE);

    wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
        wxDefaultPosition, wxSize(200, 150),
        wxNO_BORDER | wxTE_MULTILINE);

    //wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
    //    wxDefaultPosition, wxSize(200, 150),
    //    wxNO_BORDER | wxTE_MULTILINE);

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
    newVessel();
    // configure renderer
    renderer->GradientBackgroundOff();
    renderer->SetBackground(0.75, 0.75, 0.75);
    renderer->SetBackground(0, 0, 0.75);

    // add the panes to the manager
    m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
    m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
    //m_mgr.AddPane(visPanel, wxCENTER, wxT("Vis Pane"));
    m_mgr.AddPane(renderWindowInteractor, wxCENTER, wxT("Vis Pane"));

    // tell the manager to "commit" all the changes just made
    m_mgr.Update();

}

GemmMainFrame::~GemmMainFrame()
{
    delete visPanel;
    // deinitialize the frame manager
    m_mgr.UnInit();

}

//
// event handlers:
//
void GemmMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void GemmMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("This is GEMM 2020 written by Bob Brewster.It is written in C++"
        "using wxWidgets for the GUI widgets and VTK for visualization."));
    wxMessageBox(msg, _T("About GEMM"), wxOK | wxICON_INFORMATION, this);
}

void GemmMainFrame::OnAboutVTK(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("GEMM 2020 uses VTK version 8.2.0. The Visualization Toolkit (VTK) is "
        "an open source, freely available system for 3D computer graphics, image processing "
        "and visualization provided by Kitware, Inc.For further information see: "
        "<a href=\"http://www.vtk.org\">www.vtk.org</a>"));
    wxMessageBox(msg, _T("About VTK"), wxOK | wxICON_INFORMATION, this);
}

void GemmMainFrame::OnSize(wxSizeEvent& WXUNUSED(event))
{
    int w, h, width, height;
    GetClientSize(&w, &h);
    GetSize(&width, &height);
    if (visPanel)
    {
        vtkSmartPointer<vtkRenderWindow> renderWindow = visPanel->GetRenderWindow();
        // if the size changed tell render window
        if (w != width || h != height)
        {
            // adjust our (vtkRenderWindowInteractor size)
            width = w;
            height = h;
            // and our RenderWindow's size
            renderWindow->SetSize(w, h);
#if defined(__WXMSW__)
            this->Refresh();
#endif //__WXMSW__
        }
    }
}

void GemmMainFrame::newVessel()
{
    std::string vrtFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.vrt";
    std::string celFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.cel";

    if ((!vrtFileName.empty()) && (!celFileName.empty()))
    {
        STAR3ShellModel* model = new STAR3ShellModel(vrtFileName, celFileName);

        renderer->AddActor(model->GetActor());
        //        renderer->ResetCamera();
        //        renderer->GetRenderWindow()->Render();
        //        statusBar()->message(tr("Vessel file reads completed"), 2000);
    }
    else
    {
        //        statusBar()->message(tr("Vessel file reads canceled"), 2000);
    }
}
