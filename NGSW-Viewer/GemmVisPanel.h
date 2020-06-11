#pragma once
#ifndef GEMMVISPANEL_H
#define GEMMVISPANEL_H

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

class GemmVisPanel : public wxPanel
{
public:
    // constructor(s)
    GemmVisPanel();
    GemmVisPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size);
    ~GemmVisPanel();

    // getters and setters
    vtkSmartPointer<vtkRenderer> GetRenderer();
    vtkSmartPointer<vtkRenderWindow> GetRenderWindow();
    wxVTKRenderWindowInteractor* GetRenderWindowInteractor();

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

    STAR3ShellModel* model;

    wxVTKRenderWindowInteractor* renderWindowInteractor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;

//private:
//    // any class wishing to process wxWindows events must use this macro
//    wxDECLARE_EVENT_TABLE();
};

#endif