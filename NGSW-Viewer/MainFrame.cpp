#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include "MainFrame.h"
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
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(Minimal_Quit, MainFrame::OnQuit)
EVT_MENU(Minimal_About, MainFrame::OnAbout)
EVT_MENU(Minimal_About_VTK, MainFrame::OnAboutVTK)
wxEND_EVENT_TABLE()

// frame constructor
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
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

    m_pVTKWindow = new wxVTKRenderWindowInteractor(this, MY_VTK_WINDOW);
    m_pVTKWindow->UseCaptureMouseOn();
    //    m_pVTKWindow->DebugOn();

    ConstructVTK();
    ConfigureVTK();
}

MainFrame::~MainFrame()
{
    if (m_pVTKWindow) m_pVTKWindow->Delete();
    DestroyVTK();
}

void MainFrame::ConstructVTK()
{
    pRenderer = vtkRenderer::New();
    pConeMapper = vtkPolyDataMapper::New();
    pConeActor = vtkActor::New();
    pConeSource = vtkConeSource::New();

}

void MainFrame::ConfigureVTK()
{
    // connect the render window and wxVTK window
    pRenderWindow = m_pVTKWindow->GetRenderWindow();
    // connect renderer and render window and configure render window
    pRenderWindow->AddRenderer(pRenderer);
    // initialize cone
//    pConeSource->SetResolution(200);
    // connect pipeline
//    pConeMapper->SetInputConnection(pConeSource->GetOutputPort());
//    pConeActor->SetMapper(pConeMapper);
//    pRenderer->AddActor(pConeActor);
    // read in geometry and set renderer
    newVessel();
    // configure renderer
    pRenderer->GradientBackgroundOff();
    pRenderer->SetBackground(0.75, 0.75, 0.75);
//    pRenderer->GradientBackgroundOn();
//    pRenderer->SetBackground(1, 1, 1);
//    pRenderer->SetBackground2(0, 0, 1);
    pRenderer->GetActiveCamera()->Elevation(30.0);
    pRenderer->GetActiveCamera()->Azimuth(30.0);
    pRenderer->GetActiveCamera()->Zoom(1.0);
    pRenderer->GetActiveCamera()->SetClippingRange(1, 1000);
    pRenderer->ResetCamera();
}

void MainFrame::DestroyVTK()
{
    //http://www.vtk.org/pipermail/vtkusers/2003-September/019894.html
    if (pRenderer != 0)
        pRenderer->Delete();
    if (pConeMapper != 0)
        pConeMapper->Delete();
    if (pConeActor != 0)
        pConeActor->Delete();
    if (pConeSource != 0)
        pConeSource->Delete();
}

// event handlers

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("This is GEMM 2020 written by Bob Brewster.It is written in C++"
        "using wxWidgets for the GUI widgets and VTK for visualization."));
    wxMessageBox(msg, _T("About GEMM"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnAboutVTK(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("This program uses VTK version 8.2.0. The Visualization Toolkit (VTK) is "
        "an open source, freely available system for 3D computer graphics, image processing "
        "and visualization provided by Kitware, Inc.For further information see: "
        "<a href=\"http://www.vtk.org\">www.vtk.org</a>"));
    wxMessageBox(msg, _T("About VTK"), wxOK | wxICON_INFORMATION, this);
}


vtkSmartPointer<vtkRenderer> MainFrame::GetRenderer()
{
    return pRenderer;
}

void MainFrame::newFile()
{
}

void MainFrame::newVessel()
{
    std::string vrtFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.vrt";
    std::string celFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.cel";

    if ((!vrtFileName.empty()) && (!celFileName.empty()))
    {
        model = new STAR3ShellModel(vrtFileName, celFileName);

        pRenderer->AddActor(model->GetActor());
//        pRenderer->ResetCamera();
//        pRenderer->GetRenderWindow()->Render();

//        statusBar()->message(tr("Vessel file reads completed"), 2000);
    }
    else
    {
//        statusBar()->message(tr("Vessel file reads canceled"), 2000);
    }
}


void MainFrame::eraseVessel()
{
}

void MainFrame::activateVessel()
{
    model->SetActivated(true);
    pRenderer->AddActor(model->GetActor());
    pRenderer->GetRenderWindow()->Render();
}


void MainFrame::deactivateVessel()
{
    model->SetActivated(false);
    pRenderer->RemoveActor(model->GetActor());
    pRenderer->GetRenderWindow()->Render();
}

//
//void MainFrame::about()
//{
//    QMessageBox::about(this, tr("About NGSW-Viewer"),
//        tr("<h3>This is NGSW-Viewer 2007.000</h3>"
//            "<p>Written by Bob Brewster of the "
//            "Northrop Grumman Corporation"
//            "<p>As the name of the program implies, "
//            "this is a viewer for NG-SHIPWAVE models "
//            "and results. It is written in C++ using "
//            "the Qt library for the GUI widgets and "
//            "the VTK library for visualization.</p>"
//        ));
//}
//
//void MainFrame::aboutVTK()
//{
//    QMessageBox::information(this, tr("About VTK"),
//        tr("<h3>About VTK</h3>"
//            "<p>This program uses VTK version 5.0.3</p>"
//            "<p>The Visualization Toolkit (VTK) is "
//            "an open source, freely available system "
//            "for 3D computer graphics, image processing "
//            "and visualization provided by Kitware, Inc. "
//            "For further information see: "
//            "<a href=\"http://www.vtk.org\">www.vtk.org</a></p>"
//        ));
//    //  setIcon(QIcon("./images/vtk-logo-high-res.jpg"));
//}
//
//void MainFrame::createActions()
//{
//    newAction = new QAction(tr("&New"), this);
//    newAction->setIcon(QIcon("./images/new.png"));
//    newAction->setShortcut(tr("Ctrl+N"));
//    newAction->setStatusTip(tr("Create a new model"));
//    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
//
//    quitAction = new QAction(tr("&Quit"), this);
//    quitAction->setShortcut(tr("Ctrl+Q"));
//    quitAction->setStatusTip(tr("Quit the application"));
//    connect(quitAction, SIGNAL(activated()), qApp, SLOT(closeAllWindows()));
//
//    newVesselAction = new QAction(tr("&New"), this);
//    newVesselAction->setShortcut(tr("Ctrl+N"));
//    newVesselAction->setStatusTip(tr("Open existing vessel files"));
//    connect(newVesselAction, SIGNAL(triggered()), this, SLOT(newVessel()));
//
//    eraseVesselAction = new QAction(tr("&Erase"), this);
//    eraseVesselAction->setShortcut(tr("Ctrl+E"));
//    eraseVesselAction->setStatusTip(tr("Erase the current vessel"));
//    connect(eraseVesselAction, SIGNAL(triggered()), this, SLOT(eraseVessel()));
//
//    activateVesselAction = new QAction(tr("&Activate"), this);
//    activateVesselAction->setShortcut(tr("Ctrl+A"));
//    activateVesselAction->setStatusTip(tr("Activate the current vessel"));
//    connect(activateVesselAction, SIGNAL(triggered()), this, SLOT(activateVessel()));
//
//    deactivateVesselAction = new QAction(tr("&Deactivate"), this);
//    deactivateVesselAction->setShortcut(tr("Ctrl+D"));
//    deactivateVesselAction->setStatusTip(tr("Deactivate the current vessel"));
//    connect(deactivateVesselAction, SIGNAL(triggered()), this, SLOT(deactivateVessel()));
//
//    aboutAction = new QAction(tr("&About"), this);
//    aboutAction->setStatusTip(tr("Show the application's About box"));
//    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
//
//    aboutQtAction = new QAction(tr("About &Qt"), this);
//    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
//    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//
//    aboutVTKAction = new QAction(tr("About &VTK"), this);
//    aboutVTKAction->setStatusTip(tr("Show the VTK library's About box"));
//    connect(aboutVTKAction, SIGNAL(triggered()), this, SLOT(aboutVTK()));
//}
//
//void MainFrame::createMenus()
//{
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(newAction);
//    fileMenu->addSeparator();
//    fileMenu->addAction(quitAction);
//
//    vesselsMenu = menuBar()->addMenu(tr("&Vessels"));
//    vesselsMenu->addAction(newVesselAction);
//    vesselsMenu->addAction(eraseVesselAction);
//    vesselsMenu->addAction(activateVesselAction);
//    vesselsMenu->addAction(deactivateVesselAction);
//
//    optionsMenu = menuBar()->addMenu(tr("&Options"));
//
//    menuBar()->addSeparator();
//
//    helpMenu = menuBar()->addMenu(tr("&Help"));
//    helpMenu->addAction(aboutAction);
//    helpMenu->addAction(aboutQtAction);
//    helpMenu->addAction(aboutVTKAction);
//}
//
//
//void MainFrame::createToolBars()
//{
//    fileToolBar = addToolBar(tr("&File"));
//    fileToolBar->addAction(newAction);
//}
//
//
//void MainFrame::createStatusBar()
//{
//    QLabel* label = new QLabel(this);
//    statusBar()->addWidget(label, 1);
//}
