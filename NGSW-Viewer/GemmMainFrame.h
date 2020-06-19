#pragma once

#ifndef GEMMMAINFRAME_H
#define GEMMMAINFRAME_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/aui/aui.h>
#include "wxVTKRenderWindowInteractor.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include "GemmApp.h"

class GemmMainFrame : public wxFrame
{
public:
    // constructor(s)
    GemmMainFrame(GemmApp* app, const wxString& title, const wxPoint& pos, const wxSize& size);
    ~GemmMainFrame();

    // event handlers (these functions should _not_ be virtual)
    void onQuit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onAboutVTK(wxCommandEvent& event);

    // accessors
//    const vtkSmartPointer<vtkRenderer> getRenderer() const;

protected:

private:
    GemmApp* gemmApp;
    wxAuiManager wxAuiMgr;
    vtkSmartPointer<vtkRenderer> renderer;
    wxVTKRenderWindowInteractor* renderWindowInteractor;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    //void newVessel();

    // any class wishing to process wxWindows events must use this macro
    wxDECLARE_EVENT_TABLE();
};

#endif