#ifndef MAINFRAME_H
#define MAINFRAME_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxVTKRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include <vtkSmartPointer.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include <cstring>

// Custom class forward declarations
class STAR3ShellModel;

// VTK forward class declarations
class vtkRenderer;

class MainFrame : public wxFrame
{
public:
    // constructor(s)
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MainFrame();

    // getters and setters
    vtkSmartPointer<vtkRenderer> GetRenderer();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAboutVTK(wxCommandEvent& event);

protected:
    void ConstructVTK();
    void ConfigureVTK();
    void DestroyVTK();

private:
    void newFile();
    void newVessel();
    void eraseVessel();
    void activateVessel();
    void deactivateVessel();
//    void about();
//    void aboutVTK();

//    void createActions();
//    void createMenus();
//    void createToolBars();
//    void createStatusBar();
//    bool loadFile(const std::string& fileName);
//    bool saveFile(const std::string& fileName);

    // Old Qt menus and actions
 //   QMenu* fileMenu;
 //   QMenu* vesselsMenu;
 //   QMenu* optionsMenu;
 //   QMenu* helpMenu;
 //   QToolBar* fileToolBar;
 //   QAction* newAction;
 //   QAction* quitAction;
 //   QAction* newVesselAction;
 //   QAction* eraseVesselAction;
 //   QAction* activateVesselAction;
 //   QAction* deactivateVesselAction;
 //   QAction* aboutAction;
 //   QAction* aboutQtAction;
 //   QAction* aboutVTKAction;

    STAR3ShellModel* model;

    wxVTKRenderWindowInteractor* m_pVTKWindow;

    vtkSmartPointer<vtkRenderer> pRenderer;
    vtkSmartPointer<vtkRenderWindow> pRenderWindow;
    vtkSmartPointer<vtkPolyDataMapper> pConeMapper;
    vtkSmartPointer<vtkActor> pConeActor;
    vtkSmartPointer<vtkConeSource> pConeSource;

private:
    // any class wishing to process wxWindows events must use this macro
    wxDECLARE_EVENT_TABLE();
};

#endif